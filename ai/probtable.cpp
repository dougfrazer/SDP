#include "probtable.h"
#include <fstream>

const int ProbTable::NUM_TRIALS;
const int ProbTable::NUM_CUT;
const int ProbTable::NUM_DIST;
const int ProbTable::NUM_CORNER_ANGLE;
const int ProbTable::NUM_SIDE_ANGLE;
const double ProbTable::CUT_INC;
const double ProbTable::DIST_INC;
const double ProbTable::CORNER_ANGLE_INC;
const double ProbTable::SIDE_ANGLE_INC;
const double ProbTable::VELOCITY_BUMP;

ProbTable::ProbTable()
{
	cornerTable = new ProbEntry***[NUM_DIST];
	sideTable = new ProbEntry***[NUM_DIST];
	int i,j,k;
	for(i = 0; i < NUM_DIST; ++i)
	{
		cornerTable[i] = new ProbEntry**[NUM_DIST];
		sideTable[i] = new ProbEntry**[NUM_DIST];
		for(j = 0; j < NUM_DIST; ++j)
		{
			cornerTable[i][j] = new ProbEntry*[NUM_CUT];
			sideTable[i][j] = new ProbEntry*[NUM_CUT];
			for(k = 0; k < NUM_CUT; ++k)
			{
				cornerTable[i][j][k] = new ProbEntry[NUM_CORNER_ANGLE];
				sideTable[i][j][k] = new ProbEntry[NUM_SIDE_ANGLE];
			}
		}
	}
}

ProbTable::~ProbTable()
{
	int i, j, k;
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j)
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				delete[] cornerTable[i][j][k];
				delete[] sideTable[i][j][k];
			}
			delete[] cornerTable[i][j];
			delete[] sideTable[i][j];
		}
		delete[] cornerTable[i];
		delete[] sideTable[i];
	}
	delete[] cornerTable;
	delete[] sideTable;
}

bool ProbTable::load(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if(!file.is_open()) return false;
	
	int i,j,k,l;
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j)
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				for(l = 0; l < NUM_CORNER_ANGLE; ++l)
				{
					file.read((char*)&cornerTable[i][j][k][l], sizeof(ProbEntry));
				}
				for(l = 0; l < NUM_SIDE_ANGLE; ++l)
				{
					file.read((char*)&sideTable[i][j][k][l], sizeof(ProbEntry));
				}
			}
		}
	}
	
	file.close();
	
	if(file.bad()) return false;
	return true;
}

bool ProbTable::save(std::string filename)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	if(!file.is_open()) return false;
	
	int i,j,k,l;
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j)
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				for(l = 0; l < NUM_CORNER_ANGLE; ++l)
				{
					file.write((char*)&cornerTable[i][j][k][l], sizeof(ProbEntry));
				}
				for(l = 0; l < NUM_SIDE_ANGLE; ++l)
				{
					file.write((char*)&sideTable[i][j][k][l], sizeof(ProbEntry));
				}
			}
		}
	}
	
	file.close();
		
	if(file.bad()) return false;
	return true;
}

void ProbTable::fill(VelocityTable* velTable, fizGTable& table, fizGShot& shot)
{
	double d1, d2, cut, size;
	
	fizGWindow aWindow;
	const double ballRadius = .028575; //in meters
	fizGTableState state(aWindow, table, ballRadius);
	
	int i,j,k,l;
	
	double velocity;
		
	// for each entry
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j) // for each distance i
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				d1 = i * DIST_INC + DIST_INC;
				d2 = j * DIST_INC + DIST_INC;
				cut = k * CUT_INC;
				
				velocity = velTable->getMinVelocity(d1,d2,cut) + VELOCITY_BUMP;
				double pocketAngle;
				
				for(l = 0; l < NUM_CORNER_ANGLE; ++l)
				{
					pocketAngle = l * CORNER_ANGLE_INC;
					
					//printf("d1: %f d2: %f cut: %f\n",d1,d2,cut);
					
					ProbEntry entry;
					
					if(velocity > 0 && setInitialState(d1, d2, cut, pocketAngle, true, table, state))
					{
						//state.dump();
						calcProb(velocity,entry,true,shot,table,state);
						printf("probability: %f\n", entry.sinkProb);
					}
					else
					{
						entry.sinkProb = 0;
					}
					cornerTable[i][j][k][l] = entry;
				}
				for(l = 0; l < NUM_SIDE_ANGLE; ++l)
				{
					pocketAngle = l * SIDE_ANGLE_INC;
					
					//printf("d1: %f d2: %f cut: %f\n",d1,d2,cut);
					
					ProbEntry entry;
					
					if(velocity > 0 && setInitialState(d1, d2, cut, pocketAngle, false, table, state))
					{
						calcProb(velocity,entry,false,shot,table,state);
						printf("probability: %f\n", entry.sinkProb);
					}
					else
					{
						entry.sinkProb = 0;
					}
					
					sideTable[i][j][k][l] = entry;
				}
			}
		}
	}
}

void ProbTable::dump()
{
	int i,j,k,l;
	for(i = 0; i < NUM_DIST; ++i)
	{
		for(j = 0; j < NUM_DIST; ++j)
		{
			for(k = 0; k < NUM_CUT; ++k)
			{
				printf("side table:\n");
				for(l = 0; l < NUM_SIDE_ANGLE; ++l)
				{
					printf("%f\n",sideTable[i][j][k][l].sinkProb);
				}
				printf("corner table:\n");
				for(l = 0; l < NUM_CORNER_ANGLE; ++l)
				{
					printf("%f\n",cornerTable[i][j][k][l].sinkProb);
				}
			}
		}
	}
}

bool ProbTable::calcProb(double velocity, ProbEntry& entry, bool isCorner, fizGShot& gshot, fizGTable& table, fizGTableState& state)
{
	BallState ballstate;
	double x,y;

	state.getBall(CUE, ballstate,x,y);
	vec2 cuePos(x,y);
	
	state.getBall((BallType)1, ballstate,x,y);
	vec2 ballPos(x,y);

	vec2 pocketPos;
	if(isCorner)
	{	
		pocketPos = getPocketPos(table,2);
	}
	else
	{
		pocketPos = getPocketPos(table,1);
	}
	
	fizGTableState copyState;
	
	// calculate the shot angle
	ShotData shot;
	ShotData errorShot;

	vec2 aimingTarget = calculateGhostBallPosition(ballPos, pocketPos);
	shot.phi = calculateStraightAimingAngle(cuePos, aimingTarget);
	shot.v = velocity;
	
	int sinkCount = 0;

	int i;	
	for(i = 0; i < NUM_TRIALS; ++i)
	{
		errorShot = shot;
		errorShot.perturb();
		
		if(errorShot.v > 4.5) errorShot.v = 4.5;
		
		copyState = state;
		execute_gshot(errorShot, gshot, table, copyState);
		
		// is ball 1 pocketed in the correct pocket?
		copyState.getBall((BallType)1,ballstate,x,y);
		
		if(isCorner)
		{
			if(ballstate == POCKETED_NW) sinkCount++;
		}
		else
		{
			if(ballstate == POCKETED_W) sinkCount++;
		}
	}
	
	double prob = (double)sinkCount / NUM_TRIALS + .001; //.001 added by tim
	entry.sinkProb = prob;
	
	return true;
}

bool ProbTable::setInitialState(double d1, double d2, double cut, double pocketAngle, bool isCorner, fizGTable& table, fizGTableState& state)
{
	for(int ball = 2; ball <= 15; ++ball)
	{
		state.setBall((BallType)ball, NOTINPLAY, 0,0);
	}

	vec2 pocketDir;
	if(isCorner)
	{
		pocketDir = vec2(1, -1);
	}
	else
	{
		pocketDir = vec2(1, 0);
	}
	
	vec2 pocketPos;
	if(isCorner)
	{
		pocketPos = getPocketPos(table,2);
	}
	else
	{
		pocketPos = getPocketPos(table, 1);
	}
	pocketDir.normalize(d2);
	pocketDir = pocketDir.rotateDegrees(-pocketAngle);
	
	vec2 ballPos = pocketPos + pocketDir;
	/*
	if(!isOnTable(ballPos))
	{
		pocketDir = pocketDir.rotateDegrees(-2 * pocketAngle);
		ballPos = pocketPos + pocketDir;
		
		if(!isOnTable(ballPos)) return false;
	}
 	*/
	
	vec2 ghostBallPos = calculateGhostBallPosition(ballPos, pocketPos);
	
	pocketDir = pocketDir.rotateDegrees(cut);
	pocketDir.normalize(d1);
	
	vec2 cuePos = ghostBallPos + pocketDir;
	
	if(!isOnTable(cuePos))
	{
		pocketDir = pocketDir.rotateDegrees(-2 * cut);
		cuePos = ghostBallPos + pocketDir;
		
		if(!isOnTable(cuePos)) return false;
	}
	
	// position object ball b cm from pocket
	state.setBall(CUE, STATIONARY, cuePos.x, cuePos.y);
	state.setBall((BallType)1, STATIONARY, ballPos.x, ballPos.y);
	
	return true;
}

int ProbTable::getDistIndex(double dist)
{
	double numIncs = dist / DIST_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return min(NUM_DIST -1, (int)floor(numIncs) - 1);
	}
	else
	{
		return min(NUM_DIST -1,(int)floor(numIncs));
	}
}

int ProbTable::getCutIndex(double ang)
{
	double numIncs = ang / CUT_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return min(NUM_DIST -1, (int)floor(numIncs));
	}
	else
	{
		return min(NUM_CUT - 1,(int)floor(numIncs) + 1);
	}
}

int ProbTable::getSideAngleIndex(double ang)
{
	double numIncs = ang / SIDE_ANGLE_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return min(NUM_SIDE_ANGLE - 1, (int)floor(numIncs));
	}
	else
	{
		return min(NUM_SIDE_ANGLE - 1,(int)floor(numIncs) + 1);
	}
}

int ProbTable::getCornerAngleIndex(double ang)
{
	double numIncs = ang / CORNER_ANGLE_INC;
	if(numIncs - floor(numIncs) < 0.5f)
	{
		return min(NUM_CORNER_ANGLE - 1, (int)floor(numIncs));
	}
	else
	{
		return min(NUM_CORNER_ANGLE - 1,(int)floor(numIncs) + 1);
	}
}

double ProbTable::getProb(const ShotData& s)
{
	bool isCorner;
	if(s.pocket == 1 || s.pocket == 4) isCorner = false;
	else isCorner = true;

	return getProb(s.props[0].cueToObjectDistance, s.props[0].objectToPocketDistance, s.props[0].cutAngle, s.props[0].pocketAngle, isCorner);
}

double ProbTable::getProb(double d1, double d2, double cut, double pocketAngle, bool isCorner)
{
	int a,b,c,d;
	a = getDistIndex(d1);
	b = getDistIndex(d2);
	c = getCutIndex(cut);
	if(isCorner)
	{
		d = getCornerAngleIndex(pocketAngle);
	}
	else
	{
		d = getSideAngleIndex(pocketAngle);
	}
	
	if(a < 0)
	{
		//printf("error: d1 = %f\n",d1);
		return 0;
	}
	if(b < 0) b = 0;
	
	ProbEntry entry;
	
	if(isCorner)
		entry = cornerTable[a][b][c][d];
	else
		entry = sideTable[a][b][c][d];
	
	if(entry.sinkProb < 0.0001f && a > 0)
	{
		if(isCorner)
			entry = cornerTable[a -1][b][c][d];
		else
			entry = sideTable[a-1][b][c][d];
	}
	if(entry.sinkProb < 0.0001f && a > 1)
	{
		if(isCorner)
			entry = cornerTable[a -2][b][c][d];
		else
			entry = sideTable[a-2][b][c][d];
	}
	//printf("probTable lookup: %f %f %f %f %d %d %d %d %f\n", d1, d2, cut, pocketAngle, a,b,c,d,entry.sinkProb);

	return entry.sinkProb;
}

