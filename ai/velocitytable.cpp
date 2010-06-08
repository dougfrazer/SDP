#include "velocitytable.h"
#include <fstream>

const int VelocityTable::NUM_DIST;
const int VelocityTable::NUM_CUT;
const double VelocityTable::DIST_INC;
const double VelocityTable::ANGLE_INC;

VelocityTable::VelocityTable()
{
}

bool VelocityTable::load(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if(!file.is_open()) return false;
	
	file.read((char*)minVelocity, sizeof(double) * NUM_DIST * NUM_DIST * NUM_CUT);
	file.close();
	
	if(file.bad()) return false;
	
	return true;
}

bool VelocityTable::save(std::string filename)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	if(!file.is_open()) return false;
	
	file.write((char*)minVelocity, sizeof(double) * NUM_DIST * NUM_DIST * NUM_CUT);
	file.close();
	
	if(file.bad()) return false;
	
	return true;
}

void VelocityTable::fill(fizGTable& table, fizGShot& shot)
{
	double a, b, cut;
	int i,j,k;
	// for each entry
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j)
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				a = i * DIST_INC + DIST_INC;
				b = j * DIST_INC + DIST_INC;
				cut = k * ANGLE_INC;
			
				printf("filling %f %f %f\n",a,b,cut);
					
				// create a new table state
				fizGWindow aWindow;
				const double ballRadius = .028575; //in meters
				fizGTableState state(aWindow, table, ballRadius);
				
				if(setInitialState(a,b,cut,table,state))
				{
					
					//state.dump();
					
					minVelocity[i][j][k] = findMinVelocity(shot,table,state);
					
					printf("minVel: %f\n",minVelocity[i][j][k]);
				}
				else
				{
					minVelocity[i][j][k] = -1;
				}
			}
		}
	}
}

double VelocityTable::findMinVelocity(fizGShot& gshot, fizGTable& table, fizGTableState& state)
{
	// simple method: work up from 0 until ball is pocketed
	double inc_amt = 0.1f;
	double velocity = 0.1f;
	bool done = false;
	
	BallState ballstate;
	double x,y;

	// ensure that this is a valid starting state
	state.getBall(CUE, ballstate,x,y);
	vec2 cuePos(x,y);
	
	state.getBall((BallType)1, ballstate,x,y);
	vec2 ballPos(x,y);
	
	vec2 pocketPos = getPocketPos(table,2);
	
	fizGTableState copyState;
	
	// calculate the shot angle
	ShotData shot;
	//calculateShotAngle(shot, table, state, 1, 2);
	vec2 aimingTarget = calculateGhostBallPosition(ballPos, pocketPos);
	shot.phi = calculateStraightAimingAngle(cuePos, aimingTarget);
	
	while(velocity <= 4.5f && !done)
	{
		shot.v = velocity;
		
		copyState = state;
		execute_gshot(shot, gshot, table, copyState);
		
		// is ball 1 pocketed in the corner pocket?
		copyState.getBall((BallType)1,ballstate,x,y);
		
		if(ballstate == POCKETED_NW) done = true;
		else velocity += inc_amt;
	}
	
	if(done) return velocity;
	return -1;
}

bool VelocityTable::setInitialState(double a, double b, double c, fizGTable& table, fizGTableState& state)
{
	for(int ball = 2; ball <= 15; ++ball)
	{
 		state.setBall((BallType)ball, NOTINPLAY, 0,0);
	}
	
	vec2 pocket = getPocketPos(table, 2);
	vec2 oppositePocket = getPocketPos(table, 5);
	
	vec2 pocketToPocket(0,-1);
	pocketToPocket.normalize(b);
		
	vec2 ballPos = pocket + pocketToPocket;
	
	//ballPos.print();
	
	// position object ball b cm from pocket
	state.setBall((BallType)1, STATIONARY, ballPos.x, ballPos.y);
	
	vec2 ghostBallPos = calculateGhostBallPosition(ballPos, pocket);
	
	pocketToPocket = vec2(0,-1);
	pocketToPocket = pocketToPocket.rotateDegrees(c);
	pocketToPocket.normalize(a);
	
	vec2 cuePos = ghostBallPos + pocketToPocket;
	
	//cuePos.print();
	
	state.setBall(CUE, STATIONARY, cuePos.x, cuePos.y);
	
	if(cuePos.x < radius || cuePos.x > X_LENGTH - radius || cuePos.y < radius || cuePos.y > Y_LENGTH - radius || ballPos.x < radius || ballPos.x > X_LENGTH - radius || ballPos.y < radius || ballPos.y > Y_LENGTH - radius)
	{
		printf("using fallback position\n");
	
		pocketToPocket = oppositePocket - pocket;
		pocketToPocket.normalize(b);
		ballPos = pocket + pocketToPocket;
		
		pocketToPocket.normalize(a);
		pocketToPocket.rotateDegrees(c);
		ghostBallPos = calculateGhostBallPosition(ballPos, pocket);
		cuePos = ghostBallPos + pocketToPocket;
		
		state.setBall((BallType)1, STATIONARY, ballPos.x, ballPos.y);
		state.setBall(CUE, STATIONARY, cuePos.x, cuePos.y);
	}

	if(cuePos.x < radius || cuePos.x > X_LENGTH - radius || cuePos.y < radius || cuePos.y > Y_LENGTH - radius || ballPos.x < radius || ballPos.x > X_LENGTH - radius || ballPos.y < radius || ballPos.y > Y_LENGTH - radius)
	{
		return false;	
	}
	
	return true;
}

int VelocityTable::getDistIndex(double dist)
{
	double numIncs = dist / DIST_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return max(0,(int)floor(numIncs) - 1);
	}
	else
	{
		return min(NUM_DIST -1,(int)floor(numIncs));
	}
}

int VelocityTable::getAngleIndex(double ang)
{
	double numIncs = ang / ANGLE_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return (int)floor(numIncs);
	}
	else
	{
		return min(NUM_CUT - 1,(int)floor(numIncs) + 1);
	}
}

double VelocityTable::getMinVelocity(double a, double b, double cut)
{
	// convert a,b,cut to table indices
	if(a < 0 || a > 2.75f) return -1;
	if(b < 0 || b > 2.75f) return -1;
	if(cut < 0 || cut > 90) return -1;
	
	int i = getDistIndex(a);
	int j = getDistIndex(b);
	int k = getAngleIndex(cut);

	//printf("abcijk %f %f %f %d %d %d\n",a,b,cut,i,j,k);
	double mv = minVelocity[i][j][k];
	if(mv < 0 && i > 0)
	{
		mv = minVelocity[i-1][j][k];
	}
	if(mv < 0 && i > 1)
	{
		mv = minVelocity[i-2][j][k];
	}
	if(mv < 0 && i > 2)
	{
		mv = minVelocity[i-3][j][k];
	}
	return mv;
}
