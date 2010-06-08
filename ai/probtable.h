#ifndef _PROB_TABLE_H
#define _PROB_TABLE_H

#include "shotutil.h"
#include "velocitytable.h"

struct ProbEntry
{
	float sinkProb;
};

class ProbTable
{
	static const int NUM_TRIALS = 200;
	//static const int NUM_TRIALS = 100;

	// was: 10x10x10x20
	// was: 30x30x15x30
	static const int NUM_CUT = 30;
	static const int NUM_DIST = 30;
	static const int NUM_CORNER_ANGLE = 30;
	static const int NUM_SIDE_ANGLE = 30;

	static const double CUT_INC = 90.0f / NUM_CUT;
	static const double DIST_INC = 2.75f / NUM_DIST;
	static const double CORNER_ANGLE_INC = 45.0f / NUM_CORNER_ANGLE;
	static const double SIDE_ANGLE_INC = 90.0f / NUM_SIDE_ANGLE;

	static const double VELOCITY_BUMP = 0.2f;

	public:
	ProbTable();
	~ProbTable();

	bool load(std::string file);
	bool save(std::string file);

	void dump();

	void fill(VelocityTable* velTable, fizGTable& table, fizGShot& shot);

	double getProb(double d1, double d2, double cut, double pocketAngle, bool isCorner);
	double getProb(const ShotData& s);

	private:
	ProbEntry**** cornerTable;
	ProbEntry**** sideTable;

	int getDistIndex(double dist);
	int getCutIndex(double ang);
	int getSideAngleIndex(double ang);
	int getCornerAngleIndex(double ang);

	bool calcProb(double velocity, ProbEntry& entry, bool isCorner, fizGShot& gshot, fizGTable& table, fizGTableState& state);

	bool setInitialState(double d1, double d2, double cut, double pocketAngle, bool isCorner, fizGTable& table, fizGTableState& state);
};

#endif
