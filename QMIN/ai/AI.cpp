#include "AI.h"

AI::AI()
{

}

AI::~AI()
{

}


/****************************************************************
*
*	echo():
*
*	This method is called when a shot is taken by the opponent.
*	If desired, you may use it to perform analysis on the
* 	opponent's play. If not, it can be left blank.
*
*	No fields need to be set by echo().
*
****************************************************************/
void AI::echo()
{
}

/****************************************************************
*
*	breakshot():
*
*	Called when a cue ball placement and shot parameters are
*	needed for a break shot. The fizTableState can be accessed
*	using the field currentState.
*
*	When a shot is selected, set the following fields to the
*	desired values: a, b, theta, phi, v, cue_x (x-coordinate of
*	cue), cue_y (y-coordinate of cue).
*
****************************************************************/
void AI::breakshot()
{
}

/****************************************************************
*
*	shoot():
*
*	Called when shot parameters are needed and the cue ball	is not
*	in hand.
*
*	The most recent fizShot, previous fizTableState and current
*	fizTableState can be accessed using	the fields shot,
*	previousState and currentState respectively.
*
*	When a shot is selected, set the following fields to the
*	desired values: a, b, theta, phi, v, ball (the called ball),
*	and pocket (the called pocket). If you would like this
*	to be a safety shot, set the boolean isSafety to true.
*
****************************************************************/
void AI::shoot()
{
}

/****************************************************************
*
*	reshoot():
*
*	Called when your selected shot parameters are not physically
* 	possible on the table. You are given an array of booleans
*	corresponding to the parameters a, b, theta, phi, v, with
*	elements that are true if the corresponding shot parameter was
*	invalid. These parameters, in addition to the called ball and
*	pocket, are retained from your previous shot.
*
*	Example: if bad_params[2] is true, then params[2] - that is,
*	the theta value you supplied for the last shot - is invalid.
*
*	The most recent fizShot, previous fizTableState and current
*	fizTableState can be accessed using the fields shot,
*	previousState and currentState respectively.
*
*	When a new shot is selected, modify the fields a, b, theta,
*	phi, v, ball (the called ball),	and pocket (the called pocket)
*	as needed. If you leave a field	unmodified, it will remain the
*	same as in the previous shot.
*
****************************************************************/
void AI::reshoot(bool bad_params[5])
{
}

/****************************************************************
*
*	positionAndShoot():
*
*	Called when shot parameters are needed and the cue ball
*	is in hand. If the ball must be placed behind the headstring,
*	the boolean parameter place_behind_line is true.
*
*	The most recent fizShot, previous fizTableState and current
*	fizTableState can be accessed using	the fields shot,
*	previousState and currentState respectively.
*
*	When a shot is selected, set the following fields to the
*	desired values: a, b, theta, phi, v, cue_x (x-coordinate of
*	cue), cue_y (y-coordinate of cue), ball (the called ball),
*	and pocket (the called pocket).
*
****************************************************************/
void AI::positionAndShoot(bool place_behind_line)
{
}

/****************************************************************
*
*	repositionAndShoot():
*
*	Called when the given cue ball placement is invalid. If the
*	boolean not_possible is true, the ball placement is not
*	physically possible on the table. If not_behind_line is true,
*	the ball was not placed behind the headstring as required.
*
*	If the ball must be placed behind the headstring, the boolean
*	parameter place_behind_line is true.
*
*	The most recent fizShot, previous fizTableState and current
*	fizTableState can be accessed using	the fields shot,
*	previousState and currentState respectively.
*
*	When a shot is selected, set the following fields to the
*	desired values: cue_x (x-coordinate of cue ball), cue_y
*	(y-coordinate of cue ball). You may also change a, b, theta,
*	phi, v, ball, and pocket if desired.
*
****************************************************************/
void AI::repositionAndShoot(bool not_possible, bool not_behind_line, bool place_behind_line)
{
}

/****************************************************************
*
*	setFlags():
*
*	Sets flags for the game settings based on an
*	integer bitmask. Do not modify this function.
*
****************************************************************/
void AI::setFlags(int bitmask)
{
	if ((bitmask&1) == 1) {
		onRobot = true;
	}
}
