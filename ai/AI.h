#include "poolfiz.h"
#include "velocitytable.h"
#include "probtable.h"
#include <set>

using namespace std;

#ifndef AI_H
#define AI_H

const double ballRadius = .028575; //in meters
const double ballRadiusInMM = ballRadius * 1000;
const double ballDiameter = 2 * ballRadius;
const double lengthOfTable = 75*.0254; //in meters
const double widthOfTable = lengthOfTable/2; //in meters
const double oneThirdLength = lengthOfTable / 3;
const double twoThirdsLength = 2 * oneThirdLength;

const double length = lengthOfTable;               //the length of the table in metres
const double width = widthOfTable;                //the width of the table in metres
const double cornerPocketWidth = 4.625*.0254;    //the width of corner pockets (horn to horn) in metres
const double sidePocketWidth = 5.75*.0254;        //the width of the side pockets (horn to horn) in metres
const double muS = 0.2;                     //the coefficient of sliding friction (dimensionless)
const double muR = 0.01;                    //the coefficient of rolling friction (dimensionless)
const double muSp = 0.044;                  //the coefficient of spinning friction (dimensionless)
const double railHeight = 0.05715;          //the height of the top of the rail above the table in metres
const double cueLength = 1.0;               //the length of the cue in metres
const double railVelDamping = 0.6;          //damping factor of the banks
const double railSpinDamping = 0.1;         //spin damping factor of the banks
const double railZSpinAngleAdj = 0.0005;    // z-spin angle of deflection factor of the banks
const double railVelAngleAdj = 0.001;       //velocity deflection factor of the banks
//onRobot indicates whether the game is being played on the robotic table (optional, default = false)

typedef enum{
	easy,
	medium,
	hard
} Difficulty_T;

typedef enum{
	stripes,
	solids,
	table_open
} turn_T;

class shot{
public:
	double a; //left right offset
	double b; //up down offset
	double theta; //vertical angle
	double phi; //azimuth angle
	double v; //velocity
	double d1; //ball-pocket distance
	double d2; //cue-ghost distance
	double cutAngle; //cut angle
	double pocketAngle; //ball-pocket angle
	BallType ball;
	BoundaryId pocket;
	double probability; //probability of success

	bool operator<(const shot& otherShot) const {return probability < otherShot.probability;}
};

class AI{

public:
	AI();
	double chooseShot(fizGTableState state, int depth, shot& bestShot, turn_T turn);
	void generateMoves(fizGTableState state, set<shot>& moves, turn_T turn);

//private:
	bool inPlay(fizBall ball);
	bool shotPossible(fizPoint cueBallPos, fizPoint ballPos, fizPoint pocketPos);
	double pointDistance(fizPoint a, fizPoint b);
	double pointLineDistance(double a, double b, double c, fizPoint p);
	double angleBetweenLines(double yDiff, double xDiff, double yDiff2, double xDiff2);
	double scoreState(fizGTableState state);
	double scoreState(fizGTableState state, set<shot>& moves);
	void perturbShot(shot& theShot);
	bool shotSuccess(fizGTableState& state, const shot& theShot);
	double evaluateState(fizGTableState state, turn_T turn);

	//to tell if balls are in way of shot (path between two balls)
	bool ballsInWay(fizGTableState& state, fizPoint a, fizPoint b);
	fizGTable* theTable;
	VelocityTable vTable;
	ProbTable pTable;

};


#endif //AI_H
