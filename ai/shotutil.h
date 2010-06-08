#ifndef _SHOTUTIL_H_
#define _SHOTUTIL_H_

#include <assert.h>

#include <string.h>
#include "poolfiz.h"
#include "vecmath.h"

#define RANDOM(n) (ranf() * n)

#define ITALY_ERROR

extern float box_muller(float m, float s);

const double radius = 0.028575f;

//const double side_pocket_width = 0.1;
//const double corner_pocket_width = 0.0375;

//const double Y_LENGTH = 2.4384; OLD VALUE
//const double X_LENGTH = 1.2192; OLD VALUE
const double Y_LENGTH = 2.236;
const double X_LENGTH = 1.116;
//const double HEADSTRING_Y = 1.828800 OLD VALUE
const double HEADSTRING_Y = 1.677;
const double DEFAULT_THETA = 5.0f;
const double THETA_BUMP = 1.0f;

const double PI = 3.14159f;

#ifdef TAIWAN_ERROR
const float sigma_a = 0.8f;
const float sigma_b = 0.8f;
const float sigma_theta = 0.03f;
const float sigma_phi = 0.185f;
const float sigma_v = 0.085f;
#endif

#ifdef TAIWAN_ERROR_BY4
const float sigma_a = 0.8f / 4.0f;
const float sigma_b = 0.8f / 4.0f;
const float sigma_theta = 0.03f / 4.0f;
const float sigma_phi = 0.185f / 4.0f;
const float sigma_v = 0.085f / 4.0f;
#endif

#ifdef TAIWAN_ERROR_BY10
const float sigma_a = 0.8f / 10.0f;
const float sigma_b = 0.8f / 10.0f;
const float sigma_theta = 0.03f / 10.0f;
const float sigma_phi = 0.185f / 10.0f;
const float sigma_v = 0.085f / 10.0f;
#endif

#ifdef TAIWAN_ERROR_TIMES2
const float sigma_a = 0.8f * 2.0f;
const float sigma_b = 0.8f * 2.0f;
const float sigma_theta = 0.03f * 2.0f;
const float sigma_phi = 0.185f * 2.0f;
const float sigma_v = 0.085f * 2.0f;
#endif

#ifdef TAIWAN_ERROR_TIMES4
const float sigma_a = 0.8f * 4.0f;
const float sigma_b = 0.8f * 4.0f;
const float sigma_theta = 0.03f * 4.0f;
const float sigma_phi = 0.185f * 4.0f;
const float sigma_v = 0.085f * 4.0f;
#endif

#ifdef ITALY_ERROR
const float sigma_a = 0.5f;
const float sigma_b = 0.5f;
const float sigma_theta = 0.1f;
const float sigma_phi = 0.125f;
const float sigma_v = 0.075f;
#endif

// returns random value 0...1
float ranf();

typedef std::vector<fizEvent> EventList;

struct ShotProperties
{
	// mandatory properties
	double cutAngle;
	double pocketAngle;
	double cueToObjectDistance;
	double objectToPocketDistance;

	ShotProperties() { reset(); }

	void reset()
	{
		cutAngle = 0;
		pocketAngle = 0;
		cueToObjectDistance = 1;
		objectToPocketDistance = 1;
	}
};

class ShotData;

double evaluateShot(const ShotData& shot);

class ShotData
{
	public:
	ShotData();

	double phi;
	double v;
	double a;
	double b;
	double theta;

	std::string label;

	ShotProperties props[2];
	bool isCombo;

	int ball;
	int pocket;

	bool safety;

	double prob;

	vec2 cuePos;

	ShotData(const ShotData& rhs)
	{
		deepcopy(rhs);
	}

	void operator=(const ShotData& rhs)
	{
		deepcopy(rhs);
	}

	void deepcopy(const ShotData& rhs)
	{
		phi = rhs.phi;
		v = rhs.v;
		a = rhs.a;
		b = rhs.b;
		theta = rhs.theta;
		label = rhs.label;
		ball = rhs.ball;
		pocket = rhs.pocket;
		cuePos = rhs.cuePos;
		safety = rhs.safety;
		prob = rhs.prob;
		memcpy(props, rhs.props, sizeof(ShotProperties) * 2);
	}

	void reset()
	{
		phi = 0;
		v = 1;
		a = 0;
		b = 0;
		theta = DEFAULT_THETA;
		isCombo = false;
		cuePos = vec2(0,0);
		ball = 0;
		pocket = 0;
		prob = -1;
		safety = false;
		label = "";
	}

	void randomize()
	{
		phi = RANDOM(360);
		v = RANDOM(5) + 0.5;
		a = RANDOM(4) - 2;
 		b = RANDOM(4) - 2;
		theta = DEFAULT_THETA;
	}

	void perturb()
	{
		phi += box_muller(0, sigma_phi);
		v += box_muller(0, sigma_v);
		a += box_muller(0, sigma_a);
		b += box_muller(0, sigma_b);
		theta += box_muller(0, sigma_theta);

		if(v > 4.5f) v = 4.5f;
		if(v < 0.0f) v = 0.1f;
		if(phi < 0.0f) phi += 360.0f;
		if(phi > 360.0f) phi -= 360.0f;
	}

	void print()
	{
		printf("phi: %f\tv: %f\ta: %f\tb: %f\ttheta: %f\n",phi,v,a,b,theta);
		printf("cue pos: ");
		cuePos.print();
		printf("\n");
	}

	bool operator<(const ShotData& rhs) const
	{
		if(evaluateShot(*this) > evaluateShot(rhs)) return true;

		return false;
	}
};

ShotStatus execute_shot(ShotData& s, fizShot& fs, fizTable& table, fizTableState& state);
ShotStatus execute_gshot(ShotData& s, fizGShot& fs, fizGTable& table, fizGTableState& state);

bool isOnly8Left(fizGTableState& state, bool side);
bool isBallPocketed(fizGTableState& state,int ball);
bool isBallInPocket(fizGTableState& state,int ball, int pocket);
bool isStatePocketed(BallState state);
bool isStateInPlay(BallState state);
bool isShotPhysicallyPossible(fizGTable& table, fizGTableState& state, ShotData& shot);
double findMinTheta(fizGTable& table, fizGTableState& state, ShotData& shot);

void setNumActiveBalls(fizGTableState& state, int num);

double pointToLineDistance(vec2 p1, vec2 p2, vec2 point);
vec2 pointToLineNearestPoint(vec2 p1, vec2 p2, vec2 point);
bool ballInterceptsTrajectory(vec2 p1, vec2 p2, fizGTableState& state,int ball, double distanceThresh = (radius * 2));
bool isTrajectoryClear(vec2 p1, vec2 p2, fizGTableState& state, int ball = 0);
bool isPathToPocket(int ball, int pocket, fizGTableState& state, fizGTable& table);

double calculateStraightAimingAngle(vec2 source, vec2 dest);
vec2 calculateBankAimingPoint(vec2 source, vec2 dest, int rail);

vec2 getCuePos(fizGTableState& state);
vec2 getBallPos(fizGTableState& state, int ball);
vec2 getPocketPos(fizGTable& table, int pocket);
bool isOnTable(vec2 pos);

bool doesRailMatchPocket(int pocket, int rail);

void getPocketCorners(fizGTable& table, int pocket, vec2& one, vec2& two);
double getPocketAimingSize(fizGTable& table, vec2 position, int pocket);
double getPocketAngle(fizGTable& table, vec2 position, int pocket);

vec2 calculateGhostBallPosition(vec2 ball, vec2 target);
double calculateCutAngle(vec2 start, vec2 contact, vec2 target);

ShotData computeBreak(fizGShot& fshot, fizGTable& table);

// calculate the distance of a point from the specified rail
double getDistanceFromRail(vec2 point, int rail);
// calculate the distance between two points along the specified rail
double getDistanceAlongRail(vec2 a, vec2 b, int rail);

#endif
