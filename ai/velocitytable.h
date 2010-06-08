#ifndef _VELOCITY_TABLE_H
#define _VELOCITY_TABLE_H

#include "shotutil.h"


class VelocityTable
{
	static const int NUM_DIST = 80;
	static const int NUM_CUT = 135;

	static const double DIST_INC = 2.75f / 80;
	static const double ANGLE_INC = 90.0f / 135;

	public:
	VelocityTable();

	bool load(std::string file);
	bool save(std::string file);

	void fill(fizGTable& table, fizGShot& shot);

	double getMinVelocity(double cueToObjectDistance, double objectToPocketDistance, double cutAngle);

	private:

	double minVelocity[NUM_DIST][NUM_DIST][NUM_CUT];

	double findMinVelocity(fizGShot& gshot, fizGTable& table, fizGTableState& state);
	bool setInitialState(double a, double b, double c, fizGTable& table, fizGTableState& state);

	int getDistIndex(double dist);
	int getAngleIndex(double ang);
};

#endif
