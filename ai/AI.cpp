#include "AI.h"
#include <math.h>
#include "velocitytable.h"
#include "shotutil.h"


extern float box_muller(float m, float s);

AI::AI(){

	theTable = new fizGTable(length,
			                width,
			                cornerPocketWidth,
			                sidePocketWidth,
			                muS,
			                muR,
			                muSp,
			                railHeight,
			                cueLength,
			                railVelDamping,
			                railSpinDamping,
			                railZSpinAngleAdj,
			                railVelAngleAdj);

	if(!vTable.load("velocity.tbl"))
		printf("Could not load velocity table.\n");

	if(!pTable.load("probtable.tbl"))
		printf("Could not load probability table.\n");
}

double AI::evaluateState(fizGTableState state, turn_T turn){
	set<shot> moves;

	generateMoves(state, moves, turn);

	double w[] = {1.0, .33, .15};

	double sum = 0;

	set<shot>::iterator iter = moves.begin();
	if(moves.empty()) return 1;

	for(int i = 0; iter != moves.end(); i++, iter++){
		sum += iter->probability * w[i];
	}

	return sum;
}

bool AI::shotSuccess(fizGTableState& state, const shot& theShot){

	fizBall ball = state.getBall(theShot.ball);
	BallState bs = ball.getState();

	//make sure we didn't scratch
	if(!state.getBall(CUE).isInPlay()) return false;

	if(!state.getBall(theShot.ball).isInPlay()){
		return true;
	}

	return false;
}

double AI::chooseShot(fizGTableState state, int depth, shot& bestShot, turn_T turn){
	//MC search

	if(depth == 0){
		return evaluateState(state, turn);
	}

	set<shot> moves;

	generateMoves(state, moves, turn);

	if(moves.empty()) {
		//if no shots were found return -2
		return -2;
	}

	set<shot>::iterator iter = moves.begin();

	//for each possible shot
	//evaluate score of that shot (generate all moves, take 3 easiest moves)
	//choose best scoring shot

	double num_samples = 10;

	double bestScore = -1;
	//shot bestShot = 0;

	fizGShot theShot;
	bestShot = (shot)*iter;

	for(;iter != moves.end(); iter++){
		double sum = 0;
		for(int j = 0; j < num_samples; j++){
			shot nextShot = (shot)*iter;
			perturbShot(nextShot);
			fizGTableState nextState = state;

			//double x = nextState.getBall(CUE).getPos().x;
			ShotStatus status = theShot.execute(*theTable, nextState, nextShot.a, nextShot.b, nextShot.theta, nextShot.phi, nextShot.v);
			//theShot.save("shotViz_shot.txt", false);
			//theShot.saveDynamics(.1, "dynamics.txt");
			//x = nextState.getBall(CUE).getPos().x;
			//x = nextState.getBall(TWO).getPos().x;
			BallState bs = nextState.getBall(TWO).getState();

			if(status != OK) continue;
			if(!shotSuccess(nextState, *iter)) continue;
			shot tempShot;

			double stateScore = chooseShot(nextState, depth - 1, tempShot, turn);
			sum += (stateScore == -2 ? 0:stateScore);
		}

		double score = sum / num_samples * iter->probability;
		if(score > bestScore){
			bestScore = score;
			bestShot = *iter;
		}
	}

	//maybe increase search depth after this works
	return bestScore; //just for now
}

void AI::perturbShot(shot& theShot)
	{
		theShot.phi += box_muller(0, sigma_phi);
		theShot.v += box_muller(0, sigma_v);
		theShot.a += box_muller(0, sigma_a);
		theShot.b += box_muller(0, sigma_b);
		theShot.theta += box_muller(0, sigma_theta);

		if(theShot.v > 4.5f) theShot.v = 4.5f;
		if(theShot.v < 0.0f) theShot.v = 0.1f;
		if(theShot.phi < 0.0f) theShot.phi += 360.0f;
		if(theShot.phi > 360.0f) theShot.phi -= 360.0f;
		if(theShot.a > ballRadiusInMM) theShot.a = ballRadiusInMM;
		if(theShot.a < -ballRadiusInMM) theShot.a = -ballRadiusInMM;
		if(theShot.b > ballRadiusInMM) theShot.b = ballRadiusInMM;
		if(theShot.b < -ballRadiusInMM) theShot.b = -ballRadiusInMM;
	}


void AI::generateMoves(fizGTableState state, set<shot>& moves, turn_T turn){
	//for each ball

	fizBall cueBall = state.getBall(CUE);

	//must have been a scratch, ball in hand...figure out what to do
	if(!inPlay(cueBall)) return;

	fizPoint cueBallPos = cueBall.getPos();


	BallType firstBall, lastBall;
	switch(turn){
		case stripes:
			firstBall = EIGHT;
			lastBall = FIFTEEN;
			break;
		case solids:
			firstBall = ONE;
			lastBall = EIGHT;
			break;
		case table_open:
			firstBall = ONE;
			lastBall = FIFTEEN;
			break;
	}

	for(BallType ballNum = firstBall; ballNum <= lastBall; ballNum = (BallType)(ballNum + 1)){

		if(ballNum == EIGHT && !isOnly8Left(state, (turn == solids))) continue;
		fizBall ball = state.getBall(ballNum);

		//if ball is not in play
		if(!inPlay(ball)) continue;

		fizPoint ballPos = ball.getPos();

		//for each pocket, generate a straight in shot
		int numOfPockets = 6;
		BoundaryId pockets[] = {SW_POCKET, W_POCKET, NW_POCKET, NE_POCKET, E_POCKET, SE_POCKET};

		for(int i = 0; i < numOfPockets; i++){
			//get ball position, cue ball position, and pocket position
			//calculate ghost ball position
			//calculate theta, a, b, phi, and minVelocity

			fizPoint pocketPos = theTable->getPocketCenter(pockets[i]);

			//make sure shot is possible
			//if(!shotPossible(cueBallPos, ballPos, pocketPos)) continue;


			//get ball-pocket line and cue-ghostball line
			double yDiff = fabs(pocketPos.y - ballPos.y);
			double xDiff = fabs(pocketPos.x - ballPos.x);
			double ballPocketAngle = atan( yDiff / xDiff);
			double deltaX = ballDiameter * cos(ballPocketAngle);
			double deltaY = ballDiameter * sin(ballPocketAngle);

			if(ballPos.x < pocketPos.x) deltaX = -deltaX;
			if(ballPos.y < pocketPos.y) deltaY = -deltaY;

			double ghostX = ballPos.x + deltaX;
			double ghostY = ballPos.y + deltaY;

			fizPoint ghostPos(ballPos.x + deltaX, ballPos.y + deltaY);

			//make sure no balls are in the way
			//this should also make sure that the shot is possible because
			//if it is not possible then the target ball will be in the way of the cue and ghost
			vec2 p1;
			p1.x = cueBallPos.x;
			p1.y = cueBallPos.y;
			vec2 p2;
			p2.x = ghostPos.x;
			p2.y = ghostPos.y;
			if(!isTrajectoryClear(p1, p2, state, -1)) continue;

			p1.x = ballPos.x;
			p1.y = ballPos.y;
			p2.x = pocketPos.x;
			p2.y = pocketPos.y;
			if(!isTrajectoryClear(p1, p2, state, -1)) continue;

			//ok create the shot
			//set a, b to zero
			//find minimum velocity
			//find cue angle and elevation angle

			shot theShot;
			theShot.a = 0; //positive is to right of center (mm)
			theShot.b = 0; //positive is up from center (mm)
			//theShot.v = 1; //have to calculate the minimum required velocity (m/s) max = 4.5

			xDiff = fabs(cueBallPos.x - ghostPos.x);
			yDiff = fabs(cueBallPos.y - ghostPos.y);
			theShot.phi = atan(yDiff / xDiff);

			//convert to degrees
			theShot.phi *= (180.0 / M_PI);

			//find true angle
			if(cueBallPos.x < ghostPos.x){
				if(cueBallPos.y > ghostPos.y){
					theShot.phi = -theShot.phi;
				}
				else{
					//have correct angle
				}
			}
			else{
				if(cueBallPos.y < ghostPos.y){
					theShot.phi = 180 - theShot.phi;//90 + theShot.phi;
				}
				else{
					theShot.phi = 180 + theShot.phi; //180 + theShot.phi;
				}
			}

			//distance from ball to pocket
			theShot.d1 = pointDistance(ballPos, pocketPos);

			//distance from cue to ghost
			theShot.d2 = pointDistance(cueBallPos, ghostPos);

			//slopes of lines, to determine angles
			yDiff = ballPos.y - pocketPos.y;
			xDiff = ballPos.x - pocketPos.x;
			double yDiff2 = cueBallPos.y - ghostPos.y;
			double xDiff2 = cueBallPos.x - ghostPos.x;

			theShot.cutAngle = angleBetweenLines(yDiff, xDiff, yDiff2, xDiff2);

			//determine the pocket angle
			bool isCorner = true;
			switch(pockets[i]){
			case W_POCKET:
			case E_POCKET:
				yDiff2 = 0;
				xDiff2 = 1;
				isCorner = false;
				break;

			case SW_POCKET:
				yDiff2 = 1;
				xDiff2 = 1;
				break;

			case NW_POCKET:
				yDiff2 = -1;
				xDiff2 = 1;
				break;

			case NE_POCKET:
				yDiff2 = -1;
				xDiff2 = -1;
				break;

			case SE_POCKET:
				yDiff2 = 1;
				xDiff2 = -1;
				break;
			}

			theShot.pocketAngle = angleBetweenLines(yDiff, xDiff, yDiff2, xDiff2);

			theShot.v = vTable.getMinVelocity(theShot.d2, theShot.d1, theShot.cutAngle) + .2;
			if(theShot.v < 0){
				
				continue;
			}

			ShotData shotCopy;
			shotCopy.a = theShot.a;
			shotCopy.b = theShot.b;
			shotCopy.v = theShot.v;
			shotCopy.theta = 2;
			shotCopy.phi = theShot.phi;

			theShot.theta = findMinTheta(*theTable, state, shotCopy); //elevation angle (degrees)

			theShot.ball = ballNum;
			theShot.pocket = pockets[i];

			theShot.probability = pTable.getProb(theShot.d1, theShot.d2, theShot.cutAngle, theShot.pocketAngle, isCorner);
			if(theShot.probability == 0) theShot.probability = .001;

			//add shot to the list
			moves.insert(theShot);
		}
	}
}

bool AI::shotPossible(fizPoint cueBallPos, fizPoint ballPos, fizPoint pocketPos){
	//check if shot is possible
	if((cueBallPos.x < ballPos.x && ballPos.x < pocketPos.x) ||
	   (cueBallPos.x > ballPos.x && ballPos.x > pocketPos.x)){

		if((cueBallPos.y < ballPos.y && ballPos.y < pocketPos.y) ||
		   (cueBallPos.y > ballPos.y && ballPos.y > pocketPos.y)){
			return true;
		}
	}

	return false;
}

bool AI::inPlay(fizBall ball){
	BallState state = ball.getState();

	if(state == STATIONARY) return true;

	return false;
}

double AI::pointLineDistance(double a, double b, double c, fizPoint p){
	//Then distance from point (x0,y0) to line is
	//|ax0 + by0 + c| / (sqrt(a^2 + b^2))

	double top = fabs(a * p.x + b * p.y + c);
	double bottom = sqrt( pow(a,2) + pow(b,2));

	return top/bottom;
}

double AI::pointDistance(fizPoint a, fizPoint b){
	double xDiff = fabs(a.x - b.x);
	double yDiff = fabs(a.y - b.y);

	return sqrt( pow(xDiff, 2) + pow(yDiff, 2));
}

double AI::angleBetweenLines(double yDiff, double xDiff, double yDiff2, double xDiff2){
	if(xDiff == 0){
		if(xDiff2 == 0){
			return 0;
		}
		else if(yDiff2 == 0){
			return 90;
		}
		else{
			double slopeInverse = xDiff2 / yDiff2;
			double angle = atan(fabs(slopeInverse));
			return angle * 180 / M_PI;
		}
	}

	if(xDiff2 == 0){
		if(yDiff == 0){
			return 90;
		}
		else{
			double slopeInverse = xDiff / yDiff;
			double angle = atan(fabs(slopeInverse));
			return angle * 180 / M_PI;
		}
	}

	double slope1 = yDiff / xDiff;
	double slope2 = yDiff2 / xDiff2;
	double top = slope1 - slope2;
	double bottom = 1 - slope1 * slope2;

	if(bottom == 0) return 0;

	double angle = atan(fabs(top / bottom));

	return angle * 180 / M_PI;
}
