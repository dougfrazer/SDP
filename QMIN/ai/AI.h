#include "poolfiz.h"

#ifndef AI_H
#define AI_H

#ifdef __cplusplus
extern "C" {
#endif

enum Pocket { SW, W, NW, NE, E, SE, UNKNOWN_POCKET };

class AI
{
	public:

	fizGTable table;
	fizGShot shot;
	fizGTableState previousState, currentState;
	double a, b, theta, phi, v, cue_x, cue_y;
	BallType ball;
	Pocket pocket;
	bool isOpenTable, onLowBalls, isSafety, onRobot;

	AI();
	~AI();
	
	void echo();
	void shoot();
	void breakshot();
	void reshoot(bool[5]);
	void positionAndShoot(bool);
	void repositionAndShoot(bool, bool, bool);
	void setFlags(int);
};

#ifdef __cplusplus
}
#endif

#endif
