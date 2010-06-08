#include "shotutil.h"

float ranf()
{
	return (float)rand() / RAND_MAX;
}

ShotData::ShotData() {reset();}

ShotStatus execute_shot(ShotData& s, fizShot& fs, fizTable& table, fizTableState& state)
{
	//fs.execute(table, state, s.a, s.b, s.theta, s.phi, s.v, false);
	return UNKNOWN_ERROR;
}

ShotStatus execute_gshot(ShotData& s, fizGShot& fs, fizGTable& table, fizGTableState& state)
{
	//s.print();
	return fs.execute(table, state, s.a, s.b, s.theta, s.phi, s.v);
}

vec2 getPocketPos(fizGTable& table, int pocket)
{
	fizPoint p;
	switch(pocket)
	{
		case 0: p = table.getSWpocket(); break;
		case 1: p = table.getWpocket(); break;
		case 2: p = table.getNWpocket(); break;
		case 3: p = table.getNEpocket(); break;
		case 4: p = table.getEpocket(); break;
		case 5: p = table.getSEpocket(); break;
	}

	return vec2(p.x, p.y);
}

bool doesRailMatchPocket(int pocket, int rail)
{
	switch(rail)
	{
		case 0: if(pocket <= 2) return true;
			break;
		case 1: if(pocket == 2 || pocket == 3) return true;
			break;
		case 2: if(pocket >=3 && pocket <= 5) return true;
			break;
		case 3: if(pocket == 0 || pocket == 5) return true;
			break;
	}
	return false;
}

void getPocketCorners(fizGTable& table, int pocket, vec2& one, vec2& two)
{
	fizPoint a,b;
	switch(pocket)
	{
		case 0: a = table.getSWpocketLeft(); b = table.getSWpocketRight(); break;
		case 1: a = table.getWpocketLeft(); b = table.getWpocketRight(); break;
		case 2: a = table.getNWpocketLeft(); b = table.getNWpocketRight(); break;
		case 3: a = table.getNEpocketLeft(); b = table.getNEpocketRight(); break;
		case 4: a = table.getEpocketLeft(); b = table.getEpocketRight(); break;
		case 5: a = table.getSEpocketLeft(); b = table.getSEpocketRight(); break;
	}

	one = vec2(a);
	two = vec2(b);

	return;
}

double getPocketAimingSize(fizGTable& table, vec2 position, int pocket)
{
	vec2 pocketPos = getPocketPos(table, pocket);

	vec2 corner1;
	vec2 corner2;

	getPocketCorners(table,pocket,corner1,corner2);

	// direction from position to pocket
	vec2 pocketToPos = pocketPos - position;
	vec2 cornerToCorner = corner2 - corner1;

	double angle = pocketToPos.angleDegrees(cornerToCorner);
	if(angle > 90) angle = 180 - angle;

	double angleRad = angle * 3.14159 / 180;
	double width = sin(angleRad) * cornerToCorner.length();

	return width;
}

double getPocketAngle(fizGTable& table, vec2 position, int pocket)
{
	vec2 pocketPos = getPocketPos(table, pocket);
	vec2 pocketToBall = position - pocketPos;

	vec2 pocketDir;
	switch(pocket)
	{
		case 0: pocketDir = vec2(1,1); break;
		case 1: pocketDir = vec2(1,0); break;
		case 2: pocketDir = vec2(1,-1); break;
		case 3: pocketDir = vec2(-1,-1); break;
		case 4: pocketDir = vec2(-1,0); break;
		case 5: pocketDir = vec2(-1, 1); break;
	}

	return fabs(pocketToBall.angleDegrees(pocketDir));
}

bool isBallPocketed(fizGTableState& state,int ball)
{
	double x,y;
	BallState ballstate;
	state.getBall((BallType)ball, ballstate,x,y);

	return isStatePocketed(ballstate);
}

bool isBallInPocket(fizGTableState& state,int ball, int pocket)
{
	double x,y;
	BallState ballstate;
	state.getBall((BallType)ball, ballstate,x,y);

	//printf("pocket: %d state: %d",pocket,ballstate);

	if(pocket == 0 && ballstate == POCKETED_SW) return true;
	if(pocket == 1 && ballstate == POCKETED_W) return true;
	if(pocket == 2 && ballstate == POCKETED_NW) return true;
	if(pocket == 3 && ballstate == POCKETED_NE) return true;
	if(pocket == 4 && ballstate == POCKETED_E) return true;
	if(pocket == 5 && ballstate == POCKETED_SE) return true;

	return false;
}

bool isStatePocketed(BallState state)
{
	if(state == POCKETED_W) return true;
	if(state == POCKETED_NW) return true;
	if(state == POCKETED_NE) return true;
	if(state == POCKETED_E) return true;
	if(state == POCKETED_SE) return true;
	if(state == POCKETED_SW) return true;
	return false;
}

bool isStateInPlay(BallState state)
{
	if(state > 0 && state < 5) return true;
	return false;
}

bool isShotPhysicallyPossible(fizGTable& table, fizGTableState& state, ShotData& shot)
{
	if(table.isPhysicallyPossible(state, shot.a, shot.b, shot.theta, shot.phi, shot.v) == OK_PRECONDITION) return true;
	else return false;
}

double findMinTheta(fizGTable& table, fizGTableState& state, ShotData& shot)
{
	double theta = shot.theta;
	double max_theta = 70.0f;
	double inc_theta = 1.0f;
	while(theta <= max_theta)
	{
		if(table.isPhysicallyPossible(state, shot.a, shot.b, theta, shot.phi, shot.v) == OK_PRECONDITION) return theta;
		else theta += inc_theta;
	}

	return -1;
}

void setNumActiveBalls(fizGTableState& state, int num)
{
	int numBalls = state.getNumBalls();
	//void setBall(BallType btype, BallState state, double x, double y );
	for(int i = num; i < numBalls; ++i)
	{
		state.setBall((BallType)i, POCKETED_SW, 0, 0);
	}
}

double pointToLineDistance(vec2 p1, vec2 p2, vec2 point)
{
	vec2 p3 = point;
	vec2 diffP = p2 - p1;

	// ignore balls that are exactly in the same position as start or end point; these are self-collisions
	if(p3 == p1) return 0;
	if(p3 == p2) return 0;

	double u = ((p3.x - p1.x) * (p2.x - p1.x) + (p3.y - p1.y) * (p2.y - p1.y)) / pow(diffP.length(),2);

	if(u < 0 || u > 1) return -1;

	double x,y;
	x = p1.x + u * (p2.x - p1.x);
	y = p1.y + u * (p2.y - p1.y);

	vec2 p(x,y);
	vec2 ballToPoint = p3 - p;
	double distance = ballToPoint.length();

	return distance;
}

vec2 pointToLineNearestPoint(vec2 p1, vec2 p2, vec2 point)
{
	vec2 p3 = point;
	vec2 diffP = p2 - p1;

	// ignore balls that are exactly in the same position as start or end point; these are self-collisions
	if(p3 == p1) return vec2(-1,-1);
	if(p3 == p2) return vec2(-1,-1);

	double u = ((p3.x - p1.x) * (p2.x - p1.x) + (p3.y - p1.y) * (p2.y - p1.y)) / pow(diffP.length(),2);

	if(u < 0 || u > 1) return vec2(-1,-1);

	double x,y;
	x = p1.x + u * (p2.x - p1.x);
	y = p1.y + u * (p2.y - p1.y);

	vec2 p(x,y);
	return p;
}

bool ballInterceptsTrajectory(vec2 p1, vec2 p2, fizGTableState& state, int ball, double distanceThresh)
{
	BallState ballState;
	double ballX, ballY;
	double x,y;

	state.getBall((BallType)ball, ballState, ballX, ballY);

	if(isStatePocketed(ballState)) return false;


	vec2 p3(ballX, ballY);
	vec2 diffP = p2 - p1;

	// ignore balls that are exactly in the same position as start or end point; these are self-collisions
	if(p3 == p1) return false;
	if(p3 == p2) return false;

	double distance = pointToLineDistance(p1,p2,p3);

	if(distance > distanceThresh || distance < 0)
	{
		return false;
	}

	return true;
}

bool isTrajectoryClear(vec2 p1, vec2 p2, fizGTableState& state, int ball)
{
	// compare shortest distance between line and each ball to ball radius

	// ignore cue ball; start at 1
 	for(int i = 1; i < state.getNumBalls(); ++i)
	{
		if(i == ball) continue;

		if(ballInterceptsTrajectory(p1,p2,state,i))
		{
			return false;
		}
	}

	return true;
}

bool isPathToPocket(int ball, int pocket, fizGTableState& state, fizGTable& table)
{
	vec2 ballPos = getBallPos(state, ball);
	vec2 pocketPos = getPocketPos(table, pocket);
	bool ok = isTrajectoryClear(ballPos,pocketPos, state, 0);
	return ok;
}

vec2 calculateGhostBallPosition(vec2 ball, vec2 target)
{
	// calculate aiming point behind object ball
	vec2 ballToTarget = target - ball;
	ballToTarget.normalize(radius * -2);

	// calculate cue ball aiming vector
	return ballToTarget + ball;
}

double calculateCutAngle(vec2 start, vec2 contact, vec2 target)
{
	vec2 contactToTarget = target - contact;
	vec2 startToContact = contact - start;

	double cutAngle = startToContact.angleDegrees(contactToTarget);
	return cutAngle;
}

double calculateStraightAimingAngle(vec2 source, vec2 target)
{
	double phi;
	vec2 sourceToTarget = target - source;

	vec2 axis(1,0);
	phi = sourceToTarget.angleDegrees(axis);

	if(phi < 0) phi += 360;
	if(sourceToTarget.y < 0) phi = phi * -1 + 360;

	return phi;
}

double getDistanceFromRail(vec2 point, int rail)
{
	if(rail == 0)
	{
		return point.x - radius;
	}
	if(rail == 1)
	{
		return Y_LENGTH - point.y - radius;
	}
	if(rail == 2)
	{
		return X_LENGTH - point.x - radius;
	}
	if(rail == 3)
	{
		return point.y - radius;
	}
}

double getDistanceAlongRail(vec2 a, vec2 b, int rail)
{
	if(rail == 0 || rail == 2)
	{
		return b.y - a.y;
	}
	else
	{
		return b.x - a.x;
	}
}

vec2 getBallPos(fizGTableState& state, int ball)
{
	double x,y;
	BallState ballState;

	state.getBall((BallType)ball, ballState,x,y);

	return vec2(x,y);
}

vec2 getCuePos(fizGTableState& state)
{
	double cueX, cueY;
	BallState ballState;

	state.getBall(CUE, ballState, cueX, cueY);

	return vec2(cueX, cueY);
}

bool isOnTable(vec2 pos)
{
	if(pos.x < radius || pos.x > X_LENGTH - radius) return false;
	if(pos.y < radius || pos.y > Y_LENGTH - radius) return false;

	return true;
}

ShotData computeBreak(fizGShot& fshot, fizGTable& table)
{
	double y_inc = 0.3f;
	double x_inc = 0.1f;
	double y = HEADSTRING_Y;
	int num_samples = 100;

	fizGTableState state;

	ShotData shot;
	shot.ball = 1;
	shot.pocket = 1;
	shot.a = 0;
	shot.b = 0;
	shot.theta = 10;

	ShotData best_shot;
	double best_prob = -1;

	// sample positions on the table
	double x = 0.4f;
	while(x < X_LENGTH)
	{
		if(!isOnTable(vec2(x,y)))
		{
			x += x_inc;
			continue;
		}

		//printf("x, y: %f %f \n",x,y);

		vec2 cuePos = vec2(x,y);
		shot.cuePos = cuePos;

		state.rack8ball();
		state.setBall(CUE, STATIONARY, x, y);

		// calculate phi
		vec2 footSpot = vec2(X_LENGTH / 2, Y_LENGTH - HEADSTRING_Y);
		double phi = calculateStraightAimingAngle(cuePos, footSpot);

		shot.phi = phi;

		// sample velocities

		double vel_inc = 0.1f;

		double phi_mod = -0.05f;
		while(phi_mod <= 0.05)
		{
			double velocity = 2.0f;
			while(velocity < 4.5f)
			{
				shot.v = velocity;
				shot.phi = phi + phi_mod;

				int num_good = 0;

				for(int i = 0; i < num_samples; ++i)
				{
					//printf("sample!\n");

					fizGTableState copyState;
					copyState.rack8ball();
					state.setBall(CUE, STATIONARY, x, y);
					ShotData thisShot = shot;
					thisShot.perturb();
					execute_gshot(thisShot, fshot, table, copyState);

					// check if valid
					if(!isBallPocketed(copyState,0) && !isBallPocketed(copyState,8))
					{
						for(int ball = 1; ball < 15; ++ball)
						{

							if(isBallPocketed(copyState,ball))
							{
								num_good++;
								break;
							}
						}
					}
					if(isBallPocketed(copyState,0))
					{
						num_good -= 5;
					}
				}

				double prob_success = double(num_good) / num_samples;

				shot.print();
				printf("prob: %f\n",prob_success);

				if(prob_success > best_prob)
				{
					best_prob = prob_success;
					best_shot = shot;
				}

				velocity += vel_inc;
			}
			phi_mod += 0.025f;
		}
		x += x_inc;
	}

	printf("best prob = %f\n", best_prob);

	best_shot.print();

	return best_shot;
}

// calculate aiming angle from source to target via rail
vec2 calculateBankAimingPoint(vec2 source, vec2 target, int rail)
{
	assert(rail >= 0 && rail <= 4);

	// distance from source to rail
	double a = getDistanceFromRail(source, rail);
	// distance from target to rail
	double b = getDistanceFromRail(target, rail);

	// distance along rail between source and target
	double c = getDistanceAlongRail(source, target, rail);

	//printf("a: %f b: %f c: %f\n",a,b,c);

	// use similar triangles to calculate aiming distance along rail
	double aiming_distance = a * c / (a + b);

	//printf("aiming distance: %f\n",aiming_distance);

	// HACK: compensate for deadness of the rails
	//aiming_distance *= 0.90f;

	vec2 aiming_dest;
	if(rail == 0)
	{
		aiming_dest.x = 0 + radius;
		aiming_dest.y = source.y + aiming_distance;
	}
	else if(rail == 1)
	{
		aiming_dest.x = source.x + aiming_distance;
		aiming_dest.y = Y_LENGTH - radius;
	}
	else if(rail == 2)
	{
		aiming_dest.x = X_LENGTH - radius;
		aiming_dest.y = source.y + aiming_distance;
	}
	else if(rail == 3)
	{
		aiming_dest.x = source.x + aiming_distance;
		aiming_dest.y = 0 + radius;
	}

	//printf("aiming point: ");
	//aiming_dest.print();
	printf("\n");

	vec2 axis(0,1);
	vec2 destToStart = source - aiming_dest;
	vec2 destToTarget = target - aiming_dest;

	float sourceAngle = axis.angleDegrees(destToStart);
	float destAngle = axis.angleDegrees(destToTarget);

	//printf("source angle: %f dest angle: %f\n",sourceAngle, destAngle);

	return aiming_dest;
}


bool isOnly8Left(fizGTableState& state, bool isSolids)
{
	int pocketedCount = 0;
	for(int ball = 1; ball < 16; ++ball)
	{
		if(isSolids && ball < 8)
		{
			if(isBallPocketed(state,ball)) pocketedCount++;
		}
		else if(!isSolids && ball > 8)
		{
			if(isBallPocketed(state,ball)) pocketedCount++;
		}
	}
	if(pocketedCount == 7) return true;
	return false;
}

