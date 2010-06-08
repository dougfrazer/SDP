#ifndef POOLFIZ_H
#define POOLFIZ_H

#include <math.h>
#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;


/** Returns the library version number and build date. */
string getPoolfizVersion();



/** Enumerated types and constants used throughout the poolfiz library. */
#ifndef ENUMS_H
#define ENUMS_H

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes). It 
*	indicates that all shot parameters are valid; the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int OK_PRECONDITION = 0;
 	
/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that shot parameter 'a' is invalid or not physically possible;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_A_VAL = 1; 	

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that shot parameter 'b' is invalid or not physically possible;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_B_VAL = 2;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that shot parameter 'theta' is invalid or not physically possible;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_THETA_VAL = 4;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that shot parameter 'phi' is invalid or not physically possible;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_PHI_VAL = 8;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that shot parameter 'V' is invalid or not physically possible;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_V_VAL = 16; 	

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that the cue ball placement x-coordinate is out of range;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_X_VAL = 32;	

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that the cue ball placement y-coordinate is out of range;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BAD_Y_VAL = 64;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that the cue will collide with a ball before striking the cue ball;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int CUE_STICK_COLLISION = 128;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	and fizTable::isValidBallPlacement() (as well as the fizG versions of those classes).  It 
*	indicates that the ball placement x-y coordinates result in an overlap with another ball on table;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int BALL_OVERLAP = 256;

/** This integer is used within the bitmask integer returned by fizTable::isPhysicallyPossible()
*	(as well as the fizG versions of that class).  It 
*	indicates that the supplied shot parameters will result in the robot striking the table;
*	the actual integer returned by these methods 
*	is simply the OR'ed combinations of the all the possibilities from the list under the heading
*	'Variables' in the HTML documentation or listed below in the header file; you can AND the result 
*	returned by these methods with any of the possibilities from the list below to test if that 
*	particular possibility is true.  */
const int ROBOT_TABLE_COLLISION = 512;
												
/** Indicates the outcome of the shot. 
*	Can be:  OK (normal shot),
*			 MISSED_BALL (provided shot parameters resulted in cue missing cue ball),
*			 INVALID_CUE_PARAMS (provided shot parameters are out of range or not physically possible),
*			 POOLFIZ_ERROR (an error condition resulted within poolfiz),
*			 UNKNOWN_ERROR (an unknown error occurred). */				 
enum ShotStatus { OK, MISSED_BALL, INVALID_CUE_PARAMS, POOLFIZ_ERROR, UNKNOWN_ERROR };

/** Indicates the type of event.
*	Can be:  NO_EVENT (nothing happened),
*			 STATE_CHANGE (a ball transitioned to a different motion state),
*			 BALL_COLLISION (two balls collided),
*			 RAIL_COLLISION (a ball collided with a rail),
*			 POCKETED (a ball was pocketed),
*			 CUE_STRIKE (the cue ball was struck by the cue successfully),
*			 MISCUE (the cue ball was struck by the cue unsuccessfully), 
*			 UNKNOWN_EVENT (an unknown event). */      
enum EventType { NO_EVENT, STATE_CHANGE, BALL_COLLISION, RAIL_COLLISION, POCKETED,
				 CUE_STRIKE, MISCUE, UNKNOWN_EVENT };

/** Indicates the rail/pocket. 
*   Can be:  SW_POCKET, SW_RAIL, W_POCKET, NW_RAIL, NW_POCKET, N_RAIL, NE_POCKET,
		     NE_RAIL, E_POCKET, SE_RAIL, SE_POCKET, S_RAIL, UNKNOWN_BOUNDARY */
enum BoundaryId { SW_POCKET, SW_RAIL, W_POCKET, NW_RAIL, NW_POCKET, N_RAIL, NE_POCKET,
				  NE_RAIL, E_POCKET, SE_RAIL, SE_POCKET, S_RAIL, UNKNOWN_BOUNDARY };
		
/** The present state of the ball.
*	Can be:  NOTINPLAY (not on the table),
*			 STATIONARY (in play but not moving),
*			 SPINNING (not moving translationally, but spinning in place about the vertical axis),
*			 SLIDING (sliding across the surface of the table, not necessarily in a straight trajectory),
*			 ROLLING (rolling across the surface of the table, necessarily in a straight trajectory),
*            POCKETED_SW (pocketed in the SW pocket),
*			 POCKETED_W (pocketed in the W pocket),
*			 POCKETED_NW (pocketed in the NW pocket),
*			 POCKETED_NE (pocketed in the NE pocket),
*			 POCKETED_E (pocketed in the E pocket),
*			 POCKETED_SE (pocketed in the SE pocket),
*			 UNKNOWN_STATE (in an unknown state) */
enum BallState { NOTINPLAY, STATIONARY, SPINNING, SLIDING, ROLLING, 
                 POCKETED_SW, POCKETED_W, POCKETED_NW, POCKETED_NE,
                 POCKETED_E, POCKETED_SE, SLIDING_SPINNING, ROLLING_SPINNING, UNKNOWN_STATE };

/** The type (number) of the ball.
*   Can be:  CUE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, 
			 ELEVEN, TWELVE, THIRTEEN, FOURTEEN, FIFTEEN, UNKNOWN_ID */
enum BallType { CUE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, 
                NINE, TEN, ELEVEN, TWELVE, THIRTEEN, FOURTEEN,
                FIFTEEN, UNKNOWN_ID };
         
#endif // ENUMS_H



/** A simple two-dimensional point class.  */
#ifndef FIZPOINT_H
#define FIZPOINT_H

#include <iostream>
using namespace std;

/** A simple two-dimensional point class.  */
class fizPoint
{
	friend ostream &operator<<(ostream &out, fizPoint &rhs);

public:

	double x;
	double y;
	
	fizPoint();
	fizPoint(double xx, double yy);
	virtual ~fizPoint();
};
#endif  // FIZPOINT_H



/** A simple three-dimensional vector class.  */
#ifndef FIZVECTOR_H
#define FIZVECTOR_H

#include <iostream>
using namespace std;

/** A simple three-dimensional vector class.  */
class fizVector
{
	friend ostream &operator<<(ostream &out, fizVector &rhs);

public:

	double x;
	double y;
	double z;
	
	fizVector();
	fizVector(double xx, double yy, double zz);
	virtual ~fizVector();
};
#endif  // FIZVECTOR_H



/** Wrapper for Ball class */
#ifndef FIZBALL_H
#define FIZBALL_H

#ifndef FIZPOINT_H
#include "fizPoint.h"
#endif
#ifndef FIZVECTOR_H
#include "fizVector.h"
#endif

class PBall;

/** The fizBall class contains state information for a billiard ball, including
*	identification, position, and motion state. */
class fizBall
{
	friend ostream &operator<<(ostream &out, fizBall &rhs);

public:

	/** Constructor */
	fizBall();
	
	/** Constructor with initialization */
	fizBall(PBall b);

	/** Destructor */
	virtual ~fizBall();

	/* State variable access methods */

	/** Returns the type (number) of the ball. */
	BallType getID();

	/** Returns a string representing the type (number) of the ball. */
	string getIDString();

	/** Returns the present motion state of the ball. */
	BallState getState();

	/** Returns a string representing the current motion state of the ball. */
	string getStateString();
	
	/** Returns the current position of the ball. */
	fizPoint getPos();

	/** Returns the current velocity of the ball. 
	*	@since v2.2.0 */
	fizVector getVelocity();
	
	/** Returns the current angular velocity (spin) of the ball. 
	*	@since v2.2.0 */
	fizVector getSpin();	

	/** Returns true if the ball's state is STATIONARY, SPINNING, SLIDING, ROLLING; false otherwise.	*/
	bool isInPlay();

	/** Returns the radius of the ball (in metres). */
	double getRadius();

protected:
	fizVector _th;	
	fizVector _v;	
	fizVector _w;  
	fizPoint _r;
	double _rad;
	BallType _id;
	BallState _state;
	  
};
#endif



/** Wrapper for (non-graphical) table state class */ 
#ifndef FIZTABLESTATE_H
#define FIZTABLESTATE_H

class TableState;
class fizTable;
class fizGTableState;

/** The fizTableState contains a snapshot of all the balls at a given time. */
class fizTableState
{
	/** Writes a text representation of the TableState to the given output stream */
	friend ostream & operator<<(ostream &out, fizTableState &rhs);

public:

	/** Default constructor */
	fizTableState();
	
	/** Constructs a table state for the given table.
	*	@param forTable the table
	*	@since v2.5.0 */
	fizTableState( fizTable& forTable );
	
	/** Constructs a table state for the given table.
	*	@param forTable the table
	*	@param ballRadius the radius of the ball in metres
	*	@since v2.5.0 */
	fizTableState( fizTable& forTable, double ballRadius );

	/** Copy constructor */
	fizTableState( fizTableState &rhs );

	/** Assignment operator */
	fizTableState operator= ( fizTableState &rhs );

	/** Destructor */
	virtual ~fizTableState();
	
	/** Populates the given fizGTableState with the ball state data from this fizTableState.
	*	@param state the fizGTableState to populate */
	void toFizGTableState(fizGTableState& state);

	/** Returns the number of balls. */
	int getNumBalls();
	
	/** Returns the specified ball. If num is invalid, simply returns the CUE ball. 
	*	@since v2.0.0*/
	fizBall getBall(BallType num);
	
	/** DEPRECATED.  USE fizTableState::getBall(BallType num) INSTEAD.
	*	Returns the state and position of the specified ball.
	*	If num is greater than 16, simply returns the CUE ball.
	*   @deprecated since v2.0.0 */
	void getBall(BallType btype, int &state, double &x, double &y );
	
	/** Sets a ball to a certain position and state. 
	*	@since v2.0.0 */
	void setBall(BallType num, BallState state, fizPoint r );
	
	/** Sets the state and position of the specified ball.
	*	@since v2.0.0 */
	void setBall(BallType btype, BallState state, double x, double y );
	
	/** Randomizes the locations of all 16 balls on the table. */
	void randomize();
	
	/** Racks the balls for a game of 8 ball. */
	void rack8Ball();
	
	/** Racks the balls for a game of 9 ball. */
	void rack9Ball();

	/** DEPRECATED. USE operator << INSTEAD.
	*	Writes a text representation of the table state to the given output stream.
	*	@deprecated since v2.0.0; use operator << instead. */
	void dump();
	
	TableState* getTableState();

private :

	TableState* _pTableState;
};
#endif



/** Wrapper for table class */
#ifndef FIZTABLE_H
#define FIZTABLE_H

class Table;

/** A billiard table. You should only ever need one instance of a fizTable,
*	but we have provided a copy constructor or assignment operator. */
class fizTable
{
public:
	
	/** Constructs a table with the default parameters */
	fizTable();
	
	/** Constructs a table with the given parameters and automatically sets the pockets.
	*	@since v2.5.0
	*	@param onRobot indicates whether the game is being played on the robotic table */
	fizTable( bool onRobot );
	
	/** Constructs a table with the given parameters and automatically sets the pockets.
	*	@since v2.5.0
	*	@param length  the length of the table in metres
	*	@param width the width of the table in metres
	*	@param cornerPocketWidth the width of corner pockets (horn to horn) in metres 
	*	@param sidePocketWidth the width of the side pockets (horn to horn) in metres
	*	@param muS the coefficient of sliding friction (dimensionless)
	*	@param muR the coefficient of rolling friction (dimensionless)
	*	@param muSp the coefficient of spinning friction (dimensionless)
	*	@param railHeight the height of the top of the rail above the table in metres
	*	@param cueLength the length of the cue in metres 
	*	@param railVelDamping velocity damping factor of the banks
	*	@param railSpinDamping spin damping factor of the banks
	*	@param railZSpinAngleAdj z-spin angle of deflection factor of the banks
	*	@param double railVelAngleAdj velocity deflection factor of the banks
	*	@param onRobot indicates whether the game is being played on the robotic table (optional, default = false) */
	fizTable( double length, double width, double cornerPocketWidth, double sidePocketWidth, double muS, double muR, double muSp, double railHeight, double cueLength, double railVelDamping, double railSpinDamping, double railZSpinAngleAdj, double railVelAngleAdj, bool onRobot = false );

	/** Copy constructor.
	*	@since v2.5.0 */
	fizTable( const fizTable &rhs );

	/** Assignment operator
	*	@since v2.5.0 */
	fizTable operator= ( const fizTable &rhs );
	
	/** Destructor */
	virtual ~fizTable();
	
	/* Validity checkers */
	
	/** Returns BAD_X_VAL or BAD_Y_VAL if any ball that is in play is off the table or
	*	BALL_OVERLAP if any ball is overlapping with another ball.  
	*	Used to check cue ball placement validity in the case of Ball In Hand, for example.
	*	@param state the table state to check
	*	@since v2.2.0 */
	int isValidBallPlacement( fizTableState& state );

	/** Checks physical validity of the given shot parameters and table state.  Used before calling
	*	fizShot::execute() to double-check that an error condition will not result due to invalid
	*	parameters.
	*	@since v2.2.0 */
	int isPhysicallyPossible(fizTableState& state, double a, double b, double theta, double phi, double V);

	/* Convenience methods for the table parameters */

	/** Returns the length of the table (in metres). */
	double getLength();

	/** Returns the width of the table (in metres). */
	double getWidth();
	
	/** Returns the y-coordinate of the headstring of the table (in metres) */
	double getHeadString();
	
	/** Returns the x-y coordinate of the footspot of the table (in metres) */
	fizPoint getFootSpot();
	
	/** Sets the onRobot flag to tell the table if the game is being played on the robot. 
	*	@since v2.5.0 */
	void playingOnRobot(bool onRobot);	
	
	/** Returns true if the game is being played on the robot. 
	*	@since v2.5.0 */
	bool onRobot();
	
	/** Sets the length of the cue stick. 
	*	@param length the length of the cue in metres.
	*	@since v2.5.0 */
	void setCueLength(double length);

	/** Returns the length of the cue stick. 
	*	@since v2.5.0 */
	double getCueLength();
	
	/** Sets the height of the rails around the table.
	*	@param height the height of the rails in metres. 
	*	@since v2.5.0 */
	void setRailHeight(double height);
	
	/** Returns the height of the rails around the table.
	*	@since v2.5.0 */
	double getRailHeight();
	
	/** Sets the coefficient of sliding friction.
	*	@param mu the coefficient of sliding friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuSliding(double mu);
	
	/** Returns the coefficient of sliding friction.
	*	@since v2.5.0 */
	double getMuSliding();
	
	/** Sets the coefficient of rolling friction.
	*	@param mu the coefficient of rolling friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuRolling(double mu);
	
	/** Returns the coefficient of rolling friction.
	*	@since v2.5.0 */
	double getMuRolling();
	
	/** Sets the coefficient of spinning friction.
	*	@param mu the coefficient of spinning friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuSpinning(double mu);
	
	/** Returns the coefficient of spinning friction.
	*	@since v2.5.0 */
	double getMuSpinning();
	
	/** Returns the aiming point of the given pocket (in metres).
	*	@since v2.5.0 */
	fizPoint getPocketCenter(BoundaryId pocket);
	
	/** Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.5.0 */
	fizPoint getPocketRight(BoundaryId pocket);
	
	/** Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.5.0 */
	fizPoint getPocketLeft(BoundaryId pocket);
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocketRight();
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocketRight();
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocketRight();
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocketRight();
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocketRight();
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocket();
	
	/** DEPRECATED.  USE fizTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocketLeft();
	
	/** DEPRECATED.  USE fizTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocketRight();

	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getSWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getNWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getNEpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getEpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the aiming point of the given pocket (in metres).
	*	@deprecated	since v2.0.0 */
	void getSEpocket( double &x, double &y );

	class Table *getTable() { return( _pTable ) ; }
	
private:

	class Table *_pTable ;

};
#endif



/** Wrapper for event class.  */
#ifndef FIZEVENT_H
#define FIZEVENT_H

class Event;

/** The fizEvent captures information relevant to a single event such as a ball-ball collision,
*	ball motion state transition, or ball-rail collision. 
*	@since v2.0.0	*/
class fizEvent
{
	friend ostream & operator<<(ostream &out, fizEvent &rhs);
	
public:
	
	/** Constructor */
	fizEvent();
	
	/** Constructor */
	fizEvent(Event e);
	
	/** Destructor */
	virtual ~fizEvent();
	
	/* Accessors -- indicate characteristics of the event */
	
	/** Returns the time (in seconds) at which the event occured.  
	*	By definitiion, every shot begins with a CUE_STRIKE (or possibly a MISCUE)
	*	event at t = 0. */
	double getTime();
	
	/** Returns the type of event. */
	EventType getType();
	
	/** Returns a string representing the type of event. */
	string getTypeString();
	
	/** Returns the ID of the first ball involved in the event. */
	BallType getBall1();
	
	/** Returns the ID of the other ball involved in the event.  
	*	If the event only involves one ball, returns UNKNOWN_ID. */
	BallType getBall2();
	
	/** Returns the first ball involved in the event. */
	fizBall getBall1Data();
	
	/** Returns the other ball involved in the event.  
	*	If the event only involves one ball, returns UNKNOWN_ID. */
	fizBall getBall2Data();
	
	/** Returns the boundary (rail/pocket) involved in the event.  
	*   If the event does not involve a boundary, returns UNKNOWN_BOUNDARY. */
	BoundaryId getBoundary();
					
protected:
	double _time;
	EventType _type;
	BoundaryId _bnd;
	fizBall _ball1;
	fizBall _ball2;
	double _a, _b, _theta, _phi, _V;
};
#endif



/** Wrapper for (non-graphical) shot class.  */
#ifndef FIZSHOT_H
#define FIZSHOT_H

#ifndef FIZTABLE_H
#include "fizTable.h"
#endif
#ifndef FIZTABLESTATE_H
#include "fizTableState.h"
#endif
#ifndef FIZEVENT_H
#include "fizEvent.h"
#endif

/** Executes a shot and stores an event list representing the outcome of the shot.
*	You should only ever need one instance of a fizShot, but we have provided a 
*	copy constructor and assignment operator. */
class fizShot
{
	friend ostream & operator<<(ostream& out, fizShot& rhs);

public:

	/** Constructor */
	fizShot();
	
	/** Copy constructor.
	*	@since v2.5.0 */
	fizShot( const fizShot &rhs );

	/** Assignment operator
	*	@since v2.5.0 */
	fizShot operator= ( const fizShot &rhs );

	/** Destructor */
	virtual ~fizShot() ;

	/** Executes a shot on the table.
	*	@param tab reference to the fizTable on which to execute the shot
	*	@param state reference to the fizTableState with which to execute the shot
	*	@param a right/left english in mm
	*	@param b top/bottom english in mm
	*	@param theta cue stick elevation in degrees
	*	@param phi cue stick azimuth angle (heading) in degrees
	*	@param V pre-impact cue stick velocity in m/s (max. 4.5 m/s)
	*	@param verbose set to true for console output of shot data (optional, default = false) */
	ShotStatus execute(fizTable& onTable, fizTableState& state, double a, double b, double theta, double phi, double V, bool verbose = false);
	
	/** Returns the time-sorted list of events that occured on the last shot, in the form of a 
	*   Standard Template Library vector (array) of fizEvent objects. */
	vector<fizEvent> getEventList();
	
	/** Saves a text representation of the shot to the working directory.
	*	If autogen is not supplied or is supplied and is true, the filename is automatically generated and contains
	*	the optional string tag. Otherwise if autogen is false the string tag itself is used for the filename. */
	bool save(string tag = "", bool autogen = true);

	/** Loads a shot from the specified file. 
	*	@param fname the filename, including extension, of the shot to load
	*	@param state reference to the initial table state to be populated from the file
	*	@param a reference to the right/left english to be populated from the file
	*	@param b reference to the top/bottom english to be populated from the file
	*	@param theta reference to the cue stick elevation to be populated from the file
	*	@param phi reference to the cue stick azimuth angle (heading) to be populated from the file
	*	@param V reference to the pre-impact cue stick velocity to be populated from the file */
	bool load(string fname, fizTableState& state, double &a, double &b, double &theta, double &phi, double &V);
	
	/** Dumps a text representation of the shot to standard output.
	*	@deprecated Use operator << instead! */
	void dump();
	
private:

	class Shot *_pShot;
};
#endif



/** Wrapper for graphical window class */
#ifndef FIZGWINDOW_H
#define FIZGWINDOW_H

class GWindow;

/** A graphics window.  Used with the graphics versions of the poolfiz classes.
*	@see fizGTable
*	@see fizGTableState
*	@see fizGShot */
class fizGWindow
{
public:

	/** Constructor */
	fizGWindow();
	
	/** Constructor that initializes the window. */
	fizGWindow(int w, int h, float buff);
	
	/** Copy constructor.
	*	@since v2.5.0 */
	fizGWindow( const fizGWindow &rhs );

	/** Assignment operator
	*	@since v2.5.0 */
	fizGWindow operator= ( const fizGWindow &rhs );
	
	/** Destructor */
	~fizGWindow();

	/** Returns the width of the window in pixels. */
	int getWidth();
	
	/** Returns the height of the window in pixels. */
	int getHeight();
	
	GWindow *getWin();
	
private:
	GWindow *_pGWindow ;
};
#endif



/** Wrapper for graphical table class */
#ifndef FIZGTABLE_H
#define FIZGTABLE_H

#ifndef FIZTABLE_H
#include "fizTable.h"
#endif
#ifndef FIZGWINDOW_H
#include "fizGWindow.h"
#endif

class GTable;

/** A billiard table for use with graphical output. You should only ever need one instance of a fizGTable,
*	but we have provided a copy constructor or assignment operator.
*	@see fizGWindow
*	@see fizGTableState
*	@see fizGShot */
class fizGTable
{
	friend class fizGWindow;
	
public:

	/** Constructs a table with the default parameters */
	fizGTable();
	
	/** Constructs a table with the given parameters and automatically sets the pockets.
	*	@since v2.5.0
	*	@param onRobot indicates whether the game is being played on the robotic table */
	fizGTable( bool onRobot );
	
	/** Constructs a table with the given parameters and automatically sets the pockets.
	*	@since v2.5.0
	*	@param length  the length of the table in metres
	*	@param width the width of the table in metres
	*	@param cornerPocketWidth the width of corner pockets (horn to horn) in metres 
	*	@param sidePocketWidth the width of the side pockets (horn to horn) in metres
	*	@param muS the coefficient of sliding friction (dimensionless)
	*	@param muR the coefficient of rolling friction (dimensionless)
	*	@param muSp the coefficient of spinning friction (dimensionless)
	*	@param railHeight the height of the top of the rail above the table in metres
	*	@param cueLength the length of the cue in metres 
	*	@param railVelDamping velocity damping factor of the banks
	*	@param railSpinDamping spin damping factor of the banks
	*	@param railZSpinAngleAdj z-spin angle of deflection factor of the banks
	*	@param double railVelAngleAdj velocity deflection factor of the banks
	*	@param onRobot indicates whether the game is being played on the robotic table (optional, default = false) */
	fizGTable( double length, double width, double cornerPocketWidth, double sidePocketWidth, double muS, double muR, double muSp, double railHeight, double cueLength, double railVelDamping, double railSpinDamping, double railZSpinAngleAdj, double railVelAngleAdj, bool onRobot = false );

	/** Copy constructor.
	*	@since v2.5.0 */
	fizGTable( const fizGTable &rhs );

	/** Assignment operator
	*	@since v2.5.0 */
	fizGTable operator= ( const fizGTable &rhs );
	
	/** Destructor */
	virtual ~fizGTable();

	/** Draws the table on the given graphics window */
	void draw(fizGWindow& win);
	
	/* Validity checkers */
	
	/** Returns BAD_X_VAL or BAD_Y_VAL if any ball that is in play is off the table or
	*	BALL_OVERLAP if any ball is overlapping with another ball.  Used to check cue ball 
	*	placement validity in the case of Ball In Hand, for example.
	*	@param state the table state to check
	*	@since v2.2.0 */
	int isValidBallPlacement( fizGTableState& state );

	/** Checks physical validity of the given shot parameters and table state.  Used before calling
	*	fizShot::execute() to double-check that an error condition will not result due to invalid
	*	parameters.
	*	@since v2.2.0 */
	int isPhysicallyPossible(fizGTableState& state, double a, double b, double theta, double phi, double V);
	
	/* Convenience methods for the table parameters */
	
	/** Returns the (x,y) OpenGL pixel coordinates of the top left corner of the table.
	*	@param win reference to the graphics window
	*	@since v2.0.0 */
	fizPoint getTopLeftCorner(fizGWindow& win);
	
	/** Returns the (x,y) OpenGL pixel coordinates of the bottom right corner of the table.
	*	@param win reference to the graphics window
	*	@since v2.0.0 */
	fizPoint getBottomRightCorner(fizGWindow& win);

	/** Returns the length of the table (in metres). */
	double getLength();

	/** Returns the width of the table (in metres). */
	double getWidth();
	
	/** Returns the y-coordinate of the headstring of the table (in metres) */
	double getHeadString();
	
	/** Returns the x-y coordinate of the footspot of the table (in metres) */
	fizPoint getFootSpot();
	
	/** Sets the onRobot flag to tell the table if the game is being played on the robot. 
	*	@since v2.5.0 */
	void playingOnRobot(bool onRobot);	
	
	/** Returns true if the game is being played on the robot. 
	*	@since v2.5.0 */
	bool onRobot();
	
	/** Sets the length of the cue stick. 
	*	@param length the length of the cue in metres.
	*	@since v2.5.0 */
	void setCueLength(double length);

	/** Returns the length of the cue stick. 
	*	@since v2.5.0 */
	double getCueLength();
	
	/** Sets the height of the rails around the table.
	*	@param height the height of the rails in metres. 
	*	@since v2.5.0 */
	void setRailHeight(double height);
	
	/** Returns the height of the rails around the table.
	*	@since v2.5.0 */
	double getRailHeight();
	
	/** Sets the coefficient of sliding friction.
	*	@param mu the coefficient of sliding friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuSliding(double mu);
	
	/** Returns the coefficient of sliding friction.
	*	@since v2.5.0 */
	double getMuSliding();
	
	/** Sets the coefficient of rolling friction.
	*	@param mu the coefficient of rolling friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuRolling(double mu);
	
	/** Returns the coefficient of rolling friction.
	*	@since v2.5.0 */
	double getMuRolling();
	
	/** Sets the coefficient of spinning friction.
	*	@param mu the coefficient of spinning friction (between 0 and 1). 
	*	@since v2.5.0 */
	void setMuSpinning(double mu);
	
	/** Returns the coefficient of spinning friction.
	*	@since v2.5.0 */
	double getMuSpinning();
	
	/** Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.5.0 */
	fizPoint getPocketCenter(BoundaryId pocket);
	
	/** Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.5.0 */
	fizPoint getPocketRight(BoundaryId pocket);
	
	/** Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.5.0 */
	fizPoint getPocketLeft(BoundaryId pocket);

	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSWpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getWpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNWpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getNEpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getEpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@since v2.0.0
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocket();
	
	/** DEPRECATED.  USE fizGTable::getPocketLeft(BoundaryId pocket) etc INSTEAD.
	*	Returns the left corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocketLeft();
	
	/** DEPRECATED.  USE fizGTable::getPocketRight(BoundaryId pocket) etc INSTEAD.
	*	Returns the right corner coordinates of the given pocket (in metres), looking 
	*	at the pocket from the center of the table.
	*	@since v2.0.2
	*	@deprecated	since v2.5.0 */
	fizPoint getSEpocketRight();
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getSWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getNWpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getNEpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getEpocket( double &x, double &y );
	
	/** DEPRECATED.  USE fizGTable::getPocketCenter(BoundaryId pocket) etc INSTEAD.
	*	Returns the center coordinates (aiming points) of the given pocket (in metres).
	*	@deprecated	since v2.0.0; added to v2.0.1 for backwards compatibility to v1.x */
	void getSEpocket( double &x, double &y );
	
	GTable *getTable() { return( _pGTable ) ; }
	
private:

	GTable *_pGTable ;
};
#endif // FIZGTABLE_H



/** Wrapper for graphical table state class */
#ifndef FIZGTABLESTATE_H
#define FIZGTABLESTATE_H

class GTableState;

/** The fizGTableState contains a snapshot of all the balls at a given time, and is 
*	meant for use with graphical output and the other "fizG..." classes.
*	@see fizGWindow
*	@see fizGTable
*	@see fizGShot */
class fizGTableState
{
	/** Writes a text representation of the TableState to the given output stream */
	friend ostream & operator<<(ostream &out, fizGTableState &rhs);

public:
	/** Default constructor */
	fizGTableState();
	
	/** Constructor to initialize the fizGTableState correctly for graphical output.
	*	@param win the fizGWindow used for graphical output */
	fizGTableState( fizGWindow& win);
	
	/** Constructs a table state for the given table.
	*	@param win the fizGWindow used for graphical output
	*	@param forTable the table
	*	@since v2.5.0 */
	fizGTableState( fizGWindow& win, fizGTable& forTable );
	
	/** Constructs a table state for the given table.
	*	@param win the fizGWindow used for graphical output
	*	@param forTable the table
	*	@param ballRadius the radius of each ball in metres
	*	@since v2.5.0 */
	fizGTableState( fizGWindow& win, fizGTable& forTable, double ballRadius );
		
	/** Copy constructor */
	fizGTableState(fizGTableState &rhs);

	/** Assignment operator */
	fizGTableState operator=( fizGTableState &rhs );
	
	/** Destructor */
	virtual ~fizGTableState();
	
	/** Populates the given fizTableState with the ball state data from this fizGTableState.
	*	@param state the fizTableState to populate */
	void toFizTableState(fizTableState& state);

	/** Returns the number of balls. */
	int getNumBalls();
	
	/** Returns the specified ball. If num is invalid, simply returns the CUE ball. 
	*	@since v2.0.0*/
	fizBall getBall(BallType num);
	
	/** DEPRECATED.  USE fizGTableState::getBall(BallType num) INSTEAD.
	*	Returns the state and position of the specified ball.
	*	If num is greater than 16, simply returns the CUE ball.
	*   @deprecated since v2.0.0 */
	void getBall(BallType btype, BallState &state, double &x, double &y );
	
	/** Sets the state and position of the specified ball. 
	*	@since v2.0.0 */
	void setBall( BallType num, BallState state, fizPoint r );

	/** Sets the state and position of the specified ball. */
	void setBall(BallType btype, BallState state, double x, double y );
	
	/** Randomizes the locations of all 16 balls on the table. */
	void randomize();

	/** Racks the balls for a game of 8 ball. */
	void rack8ball();
	
	/** Racks the balls for a game of 9 ball. */
	void rack9ball();
	
	/** DEPRECATED. USE operator << INSTEAD.
	*	Writes a text representation of the table state to the given output stream.
	*	@deprecated since v2.0.0; use operator << instead. */
	void dump();

	/** Draws a representation of the current state on the screen. */
	void draw(fizGWindow& win);
	
	GTableState* getTableState();
		
private:

	GTableState* _pGTableState ;
};
#endif



/** Wrapper for graphical shot class.  */
#ifndef FIZGSHOT_H
#define FIZGSHOT_H

#ifndef FIZSHOT_H
#include "fizShot.h"
#endif
#ifndef FIZGTABLE_H
#include "fizGTable.h"
#endif
#ifndef FIZGTABLESTATE_H
#include "fizGTableState.h"
#endif

class GShot;

/** Executes a shot and stores an event list representing the outcome of the shot.
*	You should only ever need one instance of a fizGShot, but we have provided a 
*	copy constructor and assignment operator. */
class fizGShot
{
	friend ostream & operator<<(ostream& out, fizGShot& rhs);
	
public:

	/** Constructor */
	fizGShot();

	/** Destructor */
	virtual ~fizGShot();
	
	/** Copy constructor */
	fizGShot( fizGShot &rhs );

	/** Assignment operator */
	fizGShot operator= ( fizGShot &rhs );
	
	void getFlags(bool& foundBallHit, bool& foundRailHit, int targetball);

	
	/** Executes a shot on the grapical GTable.  Returns a ShotStatus enum indicating the result.
	*	@param tab reference to the fizGTable on which to execute the shot
	*	@param state reference to the fizGTableState with which to execute the shot
	*	@param a right/left english in mm
	*	@param b top/bottom english in mm
	*	@param theta cue stick elevation in degrees
	*	@param phi cue stick azimuth angle (heading) in degrees
	*	@param V pre-impact cue stick velocity in m/s (max. 4.5 m/s)
	*	@param verbose set to true for console output of shot data (optional, default = false) */
	ShotStatus execute(fizGTable& tbl, fizGTableState& state, double a, double b, double theta, double phi, double V, bool verbose = false);

	/** Returns the time-sorted list of events that occured on the last shot, in the form of a 
	*   Standard Template Library vector (array) of fizEvent objects. */
	vector<fizEvent> getEventList();
	
	/** Updates the provided fizGTableState to reflect the state at the given absolute time.
	*	Returns true if the provided time is less than the time of the last event
	*	that occurred during the last shot.  This is used for shot animation.
	*	@param time absolute time in seconds (every shot begins at t=0 by definition)
	*	@param state reference to the fizGTableState */
	bool getStateAt(double time, fizGTableState &state);

	/** Returns the time of the last event in the list. */
	double getLastTime();
	
	/** Saves a text representation of the shot to the working directory.
	*	If autogen is not supplied or is supplied and is true, the filename is automatically generated and contains
	*	the optional string tag. Otherwise if autogen is false the string tag itself is used for the filename. */
	bool save(string tag = "", bool autogen = true);

	/** Loads a shot from the specified file. 
	*	@param fname the filename, including extension, of the shot to load
	*	@param state reference to the initial table state to be populated from the file
	*	@param a reference to the right/left english to be populated from the file
	*	@param b reference to the top/bottom english to be populated from the file
	*	@param theta reference to the cue stick elevation to be populated from the file
	*	@param phi reference to the cue stick azimuth angle (heading) to be populated from the file
	*	@param V reference to the pre-impact cue stick velocity to be populated from the file */
	bool load(string fname, fizGTableState& state, double &a, double &b, double &theta, double &phi, double &V);
	
	/** Saves a verbose dump of the dynamics of all balls with the given timestep resolution.  The 
	*	filename is automatically generated but contains the optional string tag. 
	*	@param timestep the timestep in seconds for the dynamics.  The file size is inversely proportional to the timestep.
	*	@param tag the optional tag string for the filename. */
	bool saveDynamics(double timestep, string tag = "");
	
	/** DEPRECATED. USE operator << INSTEAD.
	*	Writes a text representation of the shot to the standard output stream.
	*	@deprecated since v2.0.0; use operator << instead. */
	void dump();
	
private:

	class GShot *_pGShot ;
};
#endif


/** Wrapper for graphical cue class. */
#ifndef FIZGCUE_H
#define FIZGCUE_H

#ifndef FIZGWINDOW_H
#include "fizGWindow.h"
#endif

class GCue;

/** The fizGCue is a cue stick that is drawn over the table. */
class fizGCue
{
public:
	/** Constructor */
	fizGCue( fizGWindow& win ) ;
	
	/** Constructs a fizGCue sized appropriately for the given tableLength and tableWidth */
	fizGCue( fizGWindow& win, double tableLength, double tableWidth ) ;
	
	/** Destructor */
	virtual ~fizGCue() ;
	
	/* Draws the cue. */
	void draw( double x , double y , double heading );
	
	/* Animates the cue. */
	void draw( double x , double y , double heading , double distance );

private:
	
	GCue *_pGCue ;

};
#endif // FIZGCUE_H

#endif // POOLFIZ_H
/****************************************************************************/
/*									    									*/
/*   Copyright 2005, Will Leckie and Michael Greenspan, Kingston, Canada    */
/*									    									*/
/****************************************************************************/
