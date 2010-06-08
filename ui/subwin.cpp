/*
 * canvas.c -- An example of subwindows and event handling
 */

#include "AI.h"
#include "shotutil.h"
#include "circleFinder.h"
#include "../serial/serial.h"
#include "../camera/getPic.h"
#include "../image_rec/lines/linefinder.h"

#include <pthread.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>
#include <unistd.h>
#include <queue>

#include "poolfiz.h"


#define FRAME_W 42
#define FRAME_H 40

#define MIN_W 45
#define MIN_H 45

#define RUL_H 20

#define FONT			GLUT_BITMAP_8_BY_13
#define CHAR_W			8
#define CHAR_H			13
#define CHAR_DESCENT		3
#define LINE_SEP		2
#define CARRIAGE_RETURN		13
#define BACK_SPACE		8
#define SIDE_WIDTH		0
#define TEXT_START		2

/* TODO: VERIFY THESE */
#define CAMERA_1_QUADRANT 1
#define CAMERA_2_QUADRANT 2
#define CAMERA_3_QUADRANT 3
#define CAMERA_4_QUADRANT 4

//function declarations
double findCueAngle(int quadrant_num);
void grabStateFromImgRec();

int canW = 300, canH = 200;
int buttonW = 80; int buttonH =  20; int buttonPadding = 6;
int rulX0 = 0;
int pixsPer16th;
int prevX, prevY;

IplImage* motionImage[4][2];

int frameWin, rulerWin, canvasWin, animateButton, suggestButton;
AI theAI;

// TODO: check this
Serial cue("/dev/rfcomm0", B115200);

/* Diplayed Info */
turn_T currentShooter = solids;

int iCalledBall = 0;
int iCalledPocket = 0;
/* end Display Info */

/* Main stuff */
enum shotVizMode { MANUAL, AUTO, UNKNOWN_SHOTVIZMODE };
enum shotVizSpeed { QUARTER, HALF, NORMAL, DOUBLE, FOURTIMES, UNKNOWN_SHOTVIZSPEED };
enum shotVizState { IDLE, PREPARING_SHOT, ANIMATING, LOADING, POST_SHOT, USER_MODE, ERROR, UNKNOWN_SHOTVIZSTATE };
enum initializeState { HIGH_RES, LOW_RES, UNINITIALIZED };
enum cueDetectionMode { ON, OFF };
enum stateMode {CUE_TRACK, GETPICTURES, GUI_IDLE};
enum player { SOLIDS, STRIPES, WINNER_SOLIDS, WINNER_STRIPES };

//to keep track of which cameras are initialized
initializeState initState[4] = {UNINITIALIZED, UNINITIALIZED, UNINITIALIZED, UNINITIALIZED};
shotVizState visState = IDLE;
shotVizMode visMode = MANUAL;
shotVizSpeed visSpeed = NORMAL;
stateMode guiState = GUI_IDLE;
player currPlayer = SOLIDS;
string indexFilename = "shotViz_info.txt";
string shotInfo = "";
double score;

bool exitKeyed = false;
bool firstVisit = true;

bool cueDetectionButtonPressed = false;
bool leftArrowPressed = false;
bool rightArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;
bool suggestButtonPressed = false;
bool animateButtonPressed = false;
bool getPicButtonPressed = false;
bool nextShotPressed = false;
bool suggestShot = true;
bool newTableState = false;
bool loadPrevState = false;
bool tableEvaluateForThisShot = true;
bool firstTime = true;

bool fileMode = false;
string fileName;
string errorString;

double cue_angle;
/* Poolfiz stuff */
fizGWindow window(1016, 508, 0.08);
fizGShot myShot;

/*double length = 2.4384;               //the length of the table in metres
double width = 1.2192;                //the width of the table in metres
double cornerPocketWidth = 0.1061;    //the width of corner pockets (horn to horn) in metres
double sidePocketWidth = 0.15;        //the width of the side pockets (horn to horn) in metres
double muS = 0.2;                     //the coefficient of sliding friction (dimensionless)
double muR = 0.01;                    //the coefficient of rolling friction (dimensionless)
double muSp = 0.044;                  //the coefficient of spinning friction (dimensionless)
double railHeight = 0.05715;          //the height of the top of the rail above the table in metres
double cueLength = 1.0;               //the length of the cue in metres
double railVelDamping = 0.6;          //damping factor of the banks
double railSpinDamping = 0.1;         //spin damping factor of the banks
double railZSpinAngleAdj = 0.0005;    // z-spin angle of deflection factor of the banks
double railVelAngleAdj = 0.001;       //velocity deflection factor of the banks */
//onRobot indicates whether the game is being played on the robotic table (optional, default = false)

fizGTable myTable = *(theAI.theTable);
/*fizGTable myTable(length,
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
			                railVelAngleAdj); */

//std::list<fizGTableState> prevStateQ;
typedef struct 
{
	vector<ball> balls;
	player statePlayer;
}stateAwesome;

fizGTableState myState(window, myTable, ballRadius);
fizGTableState prevState(window, myTable, ballRadius);
list<stateAwesome> stateList;
fizGTableState nextState(window, myTable, ballRadius);
fizPoint coor = theAI.theTable->getPocketCenter(SE_POCKET);
// fizGTableState myState(window, myTable);



fizGCue myCue(window, myTable.getLength(), myTable.getWidth());
shot nextShot;

double phi = 270.0;
double lastPhi = phi;
double nextPhi = phi;
double V = 2.0;
double a = 0.0;
double b = 0.0;
double theta = 0.0;

int global_cue_quad;
double global_current_phi;

/* Animation stuff */
void animateLastShot();
void loadNextShot();
double cueDist = 30;
double cueNormal = 20;
double cueMax = 50;
double cueMin = 17;
double prepareTime = 3.0;
double drawBackTime = 0.8;
double pushForwardTime = 0.4;
double timeScale = 1.0;
double preshotTime = 1.0;
double postshotTime = 1.0;
double errorTime = 2.0;

const double cue_error = 2.0; // number of degrees the stick can be off to send an OK command

/* Array of balls */
ball init = { 0, 0, 0, 0, 0, UNKNOWN_ID };
vector<ball> locations(20, init);


// We know that .1061 meters is 27 pixels.
double metersToPixels = 852/myTable.getLength();
double pixelsToMeters = myTable.getLength()/852;

void convertXYtoPixels(double x, double y, double &pixX, double &pixY)
{
	/********************************************
	* 41px {
	*       ----------------------------
	*       |                      ^   |
	*       |                      |   |
	*       |                   425px  |
	*       |                      |   |
	*       |  <--  852px -->      @   |
	*       |--------------------------|
	* ^82px^                            ^81px^  
	* 41px {
	**********************************************/
	
	double tempx = 82;
	double tempy = 41;
	tempy += x*metersToPixels;
	tempx += y*metersToPixels;
	
	pixX = tempx;
	pixY = tempy;
	return;
}
/*******************************************************************
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 * 				End of predefinitions
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 *******************************************************************/

void loadNextShot();
void drawCue( double distance );
void prepareShot(double secs);
void animateLastShot();
void postShot(double secs);
void setTimeScale(double ts);
void updateParams();
void emitString(const char *s, int tx, int ty);
string convertDouble(double value);
bool motionDetection(void);
void grabStateFromCameras();
string convertBool(bool value){ if(value) return "true"; return "false"; }
bool cueBallMotionDetection(double threshold);
void updateParams();
//void grabStateFromCameras();

/***************************
 * drawCue ()
 ***************************/
void drawCue( double distance, double angle )
{
	double x, y;
	BallState curState;
	if (visState != ANIMATING) {
		myState.getBall( CUE , curState, x, y );
		if(angle < 360)
			myCue.draw( x , y , angle , distance );
		else
			myCue.draw( x , y , phi , distance );
	}
}
int solid_count = 0;
int stripe_count = 0;

void checkRules()
{
	
	
	bool solid_continue = false;
	bool stripe_continue = false;

	for(int i=1; i<16; i++)
	{
		switch(i)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				if(myState.getBall((BallType)i).getState() != STATIONARY)
				{
					solid_count += 1;
					if(prevState.getBall((BallType)i).getState() == STATIONARY)
					{
						if(currPlayer == SOLIDS)
							solid_continue = true;
						//printf("Someone has sunk the %d ball in the previous turn\n", i);
					}
				}
				break;
				
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				if(myState.getBall((BallType)i).getState() != STATIONARY)
				{
					stripe_count += 1;
					if(prevState.getBall((BallType)i).getState() == STATIONARY)
					{
						if(currPlayer == STRIPES)
							stripe_continue = true;
						//printf("Someone has sunk the %d ball in the previous turn\n", i);
					}
				}
				break;
		
			case 8:
			default:
				break;
		};
	}
	
	// Eight ball
	if(myState.getBall(EIGHT).getState() != STATIONARY && prevState.getBall(EIGHT).getState() == STATIONARY)
	{
		//printf("Someone sunk the 8 ball\n");
		switch(currPlayer)
		{
			case SOLIDS:
				if(solid_count >= 7)
					currPlayer = WINNER_SOLIDS;
				else currPlayer = WINNER_STRIPES;
				return;
			case STRIPES:
				if(stripe_count >= 7)
					currPlayer = WINNER_STRIPES;
				else currPlayer = WINNER_SOLIDS;
				return;
			default:
				break;
		};
	}
	
	
	/* Nothing interesting has happened, just switch the player
	* to the other one */
	if(!tableEvaluateForThisShot)
		switch(currPlayer)
		{
			case SOLIDS:
				if(solid_continue)
					break;
				else
				{
					currPlayer = STRIPES;
					currentShooter = stripes;
					break;
				}
			case STRIPES:
				if(stripe_continue)
					break;
				else
				{
					currPlayer = SOLIDS;
					currentShooter = solids;
					break;
				}
			default:
				break;
		};
	tableEvaluateForThisShot = true;
	updateParams();

}
/***************************
 * loadNextShot ()
 **************************
void loadNextShot()
{
	visState = IDLE;
	string fileName;
	stringstream s;
	s << shotInfo;
	s >> fileName >> currentShooter >> currentBall >> isOpenTable >> isSafetyShot >> player1 >> player1Time >> player1Wins >> player1Points >> player2 >> player2Time >> player2Wins >> player2Points >> sCalledBall >> sCalledPocket;

	char cCalledBall[2];
	char cCalledPocket[2];

	for ( int i = 0 ; i < sCalledBall.length() ; i++ ) {
		cCalledBall[i] = sCalledBall.at(i);
	}

	for ( int i = 0 ; i < sCalledPocket.length() ; i++ ) {
		cCalledPocket[i] = sCalledPocket.at(i);
	}

	iCalledBall = atoi(cCalledBall);
	iCalledPocket = atoi(cCalledPocket);

	bool ok = myShot.load( fileName, myState, a, b, theta, nextPhi, V );
	if (ok) {
		lastPhi = phi;
		prepareShot(preshotTime);
	}
	else {
		cout << "ERROR:  shotViz could not load shot from file " << endl;
	visState = IDLE;
	loadNextShot();< fileName << "." << endl;
		visState = ERROR;
		postShot(errorTime);
	}
}
*/

/***************************
 * loadPrevShot ()
 **************************/
void loadPrevShot()
{

	
}

bool checkTableState()
{
	fizGTable tempTable = myTable;
	fizGShot tempShot = myShot;
	fizGTableState tempState = myState;
	
	ShotStatus result = tempShot.execute(tempTable, tempState, a,b,theta,phi,V, false);
	if(result == OK || theta >= 90)
		return true;
	else
	{
		return false;
	}
}

stateAwesome pushNewState(fizGTableState fizState)
{
	stateAwesome createdState;
	vector<ball> locs(20, init); 
	createdState.statePlayer = currPlayer;
	for(int i=0;i<fizState.getNumBalls();i++)
	{
		if( fizState.getBall( (BallType)i ).getState() != STATIONARY )
		{
			ball blah;
			blah.x = myState.getBall( (BallType)i ).getPos().x;
			blah.y = myState.getBall( (BallType)i ).getPos().y;
			blah.x_wrt_picture = myState.getBall( (BallType)i ).getPos().x;
			blah.y_wrt_picture = myState.getBall( (BallType)i ).getPos().y;
			blah.number = (BallType)i;
			locs[i] = blah;
		}
	}
	createdState.balls = locs;
	return createdState;
}


void updateTableState(stateAwesome updateState)
{
		
	for(int i = 0; i < updateState.balls.size(); i++){
		//printf("ball num %d\n", currLocation[i].number);
		if(updateState.balls[i].number < UNKNOWN_ID){
			myState.setBall(updateState.balls[i].number, STATIONARY, updateState.balls[i].x_wrt_picture,
		            updateState.balls[i].y_wrt_picture);
        }
	}
	currPlayer = updateState.statePlayer;

}

/***************************
 * userMode ()
 **************************/
void userMode()
{
	if(firstTime)
	{
		myState.rack8ball();
		errorString = "";
		firstTime = false;
	}
	
	static int first = 1;
	// draw a line from tip of cue stick to the edge of table.
	double x, y, tableX, tableY, cueX, cueY;
	double tableXPixels, tableYPixels, cueXPixels, cueYPixels;
	BallState curState;
	myState.getBall( CUE , curState, x, y );
	
	tableX = x;
	tableY = y;
	
	cueX = x-cos(phi*M_PI/180)*cueDist*pixelsToMeters;
	cueY = y-sin(phi*M_PI/180)*cueDist*pixelsToMeters;
	
	while( tableX < myTable.getWidth() && tableX > 0 && tableY < myTable.getLength() && tableY > 0)
	{
		tableX += cos(phi*M_PI/180)/1000;
		tableY += sin(phi*M_PI/180)/1000;
	}
	convertXYtoPixels(cueX, cueY, cueXPixels, cueYPixels);
	convertXYtoPixels(tableX, tableY, tableXPixels, tableYPixels);
	//printf("trying to draw cue stick (%f,%f) to (%f,%f)\n", cueXPixels, cueYPixels, tableXPixels, tableYPixels);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex2f(cueXPixels, cueYPixels);
		glVertex2f(tableXPixels, tableYPixels);
	glEnd();
	glColor3f(0,0,0);
	
	//TODO: need to set quadrant to the correct quadrant to find the cue
	int quadrant = 0;
	
	//if(initState[quadrant] != LOW_RES){
		//closeCamera(quadrant);
		
	//	initCamera(quadrant, 640, 480);
		//sleep(5);
	//	initState[quadrant] = LOW_RES;
	//}
	
	int value;
	int type = cue.checkInput(&value);
	if(type == BUTTON){
		printf("button %d pressed\n", value);
		switch(value)
		{
			case 0:
				// this is the _______ button
				break;
			case 2:
				// this is the get pic button
				guiState = GETPICTURES;
    			updateParams();
				break;
			case 1:
				// this is the animate button
				guiState = GUI_IDLE;
    			animateButtonPressed = true;
				break;
			case 5:
				// this is the suggest button
				guiState = GUI_IDLE;
				suggestButtonPressed = true;
				
				break;
			case 4:
				// this is the undo button
				checkRules();
				myState = prevState;
				break;
			case 3:
				// this is the cue_tracking button
				if(guiState == CUE_TRACK)
					guiState = GUI_IDLE;
				else if(guiState == GUI_IDLE)
					guiState = CUE_TRACK;
			
				break;
			
		}
	}
	if(type == BATTERY){
		printf("Battery at %d\n", value);
	}
	
	
	cue_angle = findCueAngle(quadrant);
	global_current_phi = (cue_angle == -1 ? phi:cue_angle);
	
	if(guiState == CUE_TRACK){
		if(cue_angle > -1)
			drawCue(cueDist, cue_angle);
		double opposite_phi = fmod(phi+180.0,360);
		if(fabs(cue_angle - phi) < cue_error)
			cue.Command('s');
		else if(phi >= 0 && phi <= 180)
		{
			if(cue_angle > phi && cue_angle < opposite_phi)
				cue.Command('l');
			else
				cue.Command('r');
		}
		else 
		{
			if(cue_angle < phi && cue_angle > opposite_phi)
				cue.Command('r');
			else
				cue.Command('l');
		}			
	}
	else{
		cue.Command('s');
	}
	
	ShotData theShotData;
	theShotData.a = 0;
	theShotData.b = 0;
	theShotData.v = V;
	theShotData.phi = phi;
	theShotData.theta = theta;
	if(leftArrowPressed)
	{
		//printf("got left arrow\n");
		phi = fmod(phi - .5, 360);//radius pool ball in meters
		theShotData.phi = phi;
		theShotData.theta = theta;
		theta = findMinTheta(myTable, myState, theShotData);
		drawCue(cueDist, phi);
		leftArrowPressed = false;
	}
	if(rightArrowPressed)
	{
		//printf("got right arrow\n");
		phi = fmod(phi + .5, 360);
		theShotData.phi = phi;
		theShotData.theta = theta;
		theta = findMinTheta(myTable, myState, theShotData);
		drawCue(cueDist, phi);
		rightArrowPressed = false;
	}
	if(suggestButtonPressed)
	{
		suggestButtonPressed = false;
		suggestShot = true;
		//prevStateQ.push(myState);
		prevState = myState;
		visState = IDLE;
		loadNextShot();
		myState = prevState;
	}
	if(animateButtonPressed)
	{
		animateButtonPressed = false;
		//prevStateQ.push(myState);
		prevState = myState;
		visState = IDLE;
		loadNextShot();
	}
	if(getPicButtonPressed)
	{
		getPicButtonPressed = false;
		grabStateFromCameras();
	}
	if(nextShotPressed)
	{
		nextShotPressed = false;
		visState = ANIMATING;
		loadNextShot();
	}
	if(upArrowPressed && V < 4.49)
	{
		V += .1;
		upArrowPressed = false;
	}
	if(downArrowPressed && V > .01)
	{
		V -= .1;
		downArrowPressed = false;
	}
	
}



/***************************
 * loadNextShotv2 ()
 **************************/
void loadNextShot()
{
	// TODO: do something about AI hiccups...
	// AI returns -2 when no shots were found
	if(!suggestShot)
	{
		//if(guiState == CUE_TRACK)
		//{
			//phi = global_current_phi;
			nextPhi = global_current_phi;
			//lastPhi = phi;
		//}
		//else
		//{
		//	nextPhi = phi;
		//}
		
		ShotData theShotData;
		theShotData.a = 0;
		theShotData.b = 0;
		theShotData.v = V;
		theShotData.phi = nextPhi;
		theShotData.theta = theta;
		theta = findMinTheta(myTable, myState, theShotData);
	}
	else if(fileMode)
	{	
		bool ok = myShot.load( fileName, myState, a, b, theta, nextPhi, V );
		if (ok) {
			lastPhi = phi;
		}
		else {
			cout << "ERROR:  shotViz could not load shot from file " << endl;
			visState = ERROR;
			postShot(errorTime);
		}
	}
	else
	{
		fizGShot theShot;
		score = theAI.chooseShot(myState, 1, nextShot, currentShooter);
		
		if(score == -2){
			V = 2;
			phi = 0;
			visState = USER_MODE;
			errorString = "Did not find any shots";
			return;
		}
		
		errorString = "";
		a = nextShot.a;
		b = nextShot.b;
		theta = nextShot.theta;
		nextPhi = nextShot.phi;
		V = nextShot.v;
		lastPhi = phi;
		nextPhi = nextShot.phi;

	}
 	if(V <= .001) 
 	{
 		printf("Got a bogus velocity\n");
 		V = 2;
 	}
 	
	prepareShot(preshotTime);
}

/***************************
 * prepareShot (double)
 ***************************/
double timeLeft, waitSecs, elapsed;
void prepareShot(double secs)
{
	//printf("Entered prepareShot(), timeleft = %d\n", timeLeft);
	static clock_t start = clock();
	if (visState == PREPARING_SHOT) {
		waitSecs = secs/timeScale;
		elapsed = (double)(clock() - start)/CLOCKS_PER_SEC;
		timeLeft = waitSecs - elapsed;
		if ( elapsed > waitSecs ) {
			animateLastShot();
			cueDist = cueNormal;
		}
// 		else if (timeLeft <= ) {
// 			if (timeLeft > pushForwardTime) cueDist = cueNormal + (timeLeft-pushForwardTime)/drawBackTime * (cueMax - cueNormal);
// 			else cueDist = cueMax - timeLeft/pushForwardTime * (cueMax - cueMin);
// 			cueDist = cueNormal;
// 		}
		else phi = lastPhi + elapsed/(waitSecs) * (nextPhi-lastPhi);
	}
	else {
		start = clock();
		visState = PREPARING_SHOT;
	}
}

/***************************
 * animateLastShot ()
 ***************************/
void animateLastShot()
{
	printf("Entered animateLastShot()\n");
	phi = nextPhi;
	iCalledBall = 0;
	ShotStatus result = myShot.execute(myTable, myState, a, b, theta, phi, V, false);
	if (result == OK) visState = ANIMATING;
	else {
		cout << "ERROR:  shotViz could not play the shot." << endl;
		postShot(postshotTime);
	}
}

/***************************
 * postShot (double)
 ***************************/
void postShot(double secs)
{
	//printf("Entered postShot()\n");
	tableEvaluateForThisShot = false;
	static clock_t start = clock();
	if (visState == POST_SHOT || visState == ERROR) 
	{
/*		if(loadPrevState)
		{
			nextState = myState;
			myState = prevState;
			loadPrevState = false;
		}*/
		visState = USER_MODE;
		userMode();
		//loadNextShot(); // well we finished that shot, how about another?
	}
	else 
	{
		start = clock();
		visState = POST_SHOT;
	}
	
	fizGShot theShot;
	score = theAI.chooseShot(myState, 1, nextShot, currentShooter);
	
	if(suggestShot)
	{
		myState = prevState;
		suggestShot = false;
	}
	else if(score == -2){
		V = 2;
		phi = 0;
		visState = USER_MODE;
		errorString = "Did not find any shots";
		//checkRules();
	}
	else
	{
		errorString = "";
		V = nextShot.v;
		phi = nextShot.phi;
		//checkRules();
	}
}

/***************************
 * setTimeScale (double)
 ***************************/
void setTimeScale(double ts)
{
	if (ts < 0.1) timeScale = 0.1;
	else if (ts > 30.0) timeScale = 30.0;
	else timeScale = ts;
}



/*******************************************************************
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 * 				End of poolfiz functions
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 *******************************************************************/

/* (from OpenGL Programming Guide, 2nd edition, page 601)
 If exact two-dimensional raterization is desired, you must
 carefully specify both the orthographic projection and the vertices
 of primitives that are to be rasterized.  The orthographic projection
 should be specified with integer coordinates, as shown in the
 following example:
      gluOrtho2D(0, width, 0, height);
 where width and height are the dimensions of the viewport.  Given
 this projection matrix, polygon vertices and pixel image positions
 should be placed at integer coordinates to rasterize predictably.
 For example, glRecti(0,0,1,1) reliably fills the lower left pixel of
 the viewport, and glRasterPos2i(0,0) reliably positions an unzoomed
 image at the lower left of the viewport.  Point vertices, line
 vertices, and bitmap positions should be placed at half-integer
 locations, however.  For example, a line drawn from (x1,0.5) to
 (x2,0.5) will be reliably rendered along the bottom row of pixels
 into the viewport, and a point drawn at (0.5,0.5) will reliably fill
 the same pixel as glRecti(0,0,1,1).

 An optimum compromise that allows all primitives to be specified at
 integer positions, while still ensuring predictable rasterization, is
 to translate x and y by 0.375, as shown in the following code
 fragment [reshapeCB()].  Such a translation keeps polygon and pixel
 image edges safely away from the centers of pixels, while moving line
 vertices close enough to the pixel centers.
*/


/*******************************************************************
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 * 				Begin Camera functions
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 *******************************************************************/

void grabStateFromCameras()
{
	// Get all the pictures, save them into 1.jpg, 2.jpg, etc. 
	//actually do this in grabStateFromImgRec so we can thread it
	//system("../camera/getPic 0 960 720 1.jpg");
	//system("../camera/getPic 1 960 720 2.jpg");
	//system("../camera/getPic 2 960 720 3.jpg");
	//system("../camera/getPic 3 960 720 4.jpg");
	
	grabStateFromImgRec();
}

typedef struct{
	CircleFinder* cf;
	circleInputInfo* theInfo;
} threadInfo_T;

void* threadMaker(void* info)
{
	threadInfo_T* input = (threadInfo_T*) info;
	input->cf->getLocations((void*) input->theInfo);
}

void grabStateFromImgRec()
//void grabStateFromCameras()
{
	int currNum;
	
	CircleFinder cf;
	
	for(int i = 0; i < 16; i++){
		myState.setBall((BallType)i, NOTINPLAY, 0, 0);
	}
	
	threadInfo_T toThread;
	toThread.cf = &cf;
	circleInputInfo theInfo1, theInfo2, theInfo3, theInfo4;
	// Iterate through each picture and figure out where the balls are
	//  store this information into the locations vector 
	pthread_t theThread;
	void* status;
	for(int camera = 0; camera < 4; camera++)
	{
		printf("\nTrying to get picture for camera %d\n", camera);
		// Initialize a blank vector and fill it in
		vector<ball> currLocation;// (20, init);
		int rc;
		
		
		switch(camera)
		{
			case 0:
				system("../camera/getPic 0 960 720 1.jpg");
				//system("../camera/getPic 0 1600 1200 1.pnm");
				theInfo1.picName = "1.jpg";
				theInfo1.quad = 2;
				toThread.theInfo = &theInfo1;
				rc = pthread_create(&theThread, NULL, threadMaker, (void*) &toThread);
			
				break;
			case 1: 
				system("../camera/getPic 1 960 720 2.jpg");
				//system("../camera/getPic 1 1600 1200 2.pnm");
				pthread_join(theThread, &status);
				
				theInfo2.picName = "2.jpg";
				theInfo2.quad = 3;
				toThread.theInfo = & theInfo2;
				rc = pthread_create(&theThread, NULL, threadMaker, (void*) &toThread);

				
				break;
			case 2: 
				system("../camera/getPic 2 960 720 3.jpg");
				//system("../camera/getPic 2 1600 1200 3.pnm");
				pthread_join(theThread, &status);
				theInfo3.picName = "3.jpg";
				theInfo3.quad = 4;
				toThread.theInfo = &theInfo3;
				rc = pthread_create(&theThread, NULL, threadMaker, (void*) &toThread);
				
				break;
			case 3: 
				system("../camera/getPic 3 960 720 4.jpg");
				//system("../camera/getPic 3 1600 1200 4.pnm");
				pthread_join(theThread, &status);
				theInfo4.picName = "4.jpg";
				theInfo4.quad = 1;
				toThread.theInfo = &theInfo4;
				rc = pthread_create(&theThread, NULL, threadMaker, (void*) &toThread);
				
				break;
				
			default:
				printf("\n\n----------------------------\
						\nerror in grabStateFromCameras function, somehow camera got above 4 \
						\n---------------------------\n");
		}
	}
	
	pthread_join(theThread, &status);
	
	vector<ball> currLocation;
	vector<ball> empty;
	cf.putTableTogether(theInfo1.result, theInfo2.result, theInfo3.result, theInfo4.result, currLocation, empty);
	
	printf("%d balls returned\n", currLocation.size());
	for(int i = 0; i < currLocation.size(); i++){
		//printf("ball num %d\n", currLocation[i].number);
		if(currLocation[i].number < UNKNOWN_ID){
			locations[(int)currLocation[i].number] = currLocation[i];
			myState.setBall(currLocation[i].number, STATIONARY, currLocation[i].x_wrt_picture,
		            currLocation[i].y_wrt_picture);
        }
	}
	
	if(myState.getBall(CUE).getState() == STATIONARY)
		global_cue_quad = locations[CUE].quad;
		
	checkRules();
}

int whichQuadrantIsTheCueIn()
{
	/* Assuming table to look like this:
	*  |------------------------------|
	*  |   2          |     3         |
	*  |              |               |
	*  |------------------------------|
	*  |   1          |     4         |
	*  |              |               |
	*  |------------------------------|
	*/
	
	//locations[CUE].x_wrt_picture = 300;
	//locations[CUE].y_wrt_picture = 300;
	//myState.setBall(CUE, STATIONARY, pixelsToMeters*locations[CUE].x_wrt_picture, pixelsToMeters*locations[CUE].y_wrt_picture);
	// Figure out which quadrant the cue ball is in.
	int cue_quadrant = 0;
	
	// Make sure we dont hit null-pointers if we try and run this before grabStateFromCameras();
	//if( locations[CUE].x == 0)
	//	grabStateFromCameras();
	
	// Calculate the quadrant based on absolute distances.
	if( locations[CUE].x_wrt_picture < myTable.getWidth()/2)
	{
		if(locations[CUE].y_wrt_picture < myTable.getLength()/2)
			cue_quadrant = 1;
		else
			cue_quadrant = 2;
	}
	else
	{
		if(locations[CUE].y_wrt_picture < myTable.getLength()/2)
			cue_quadrant = 0;
		else
			cue_quadrant = 3;
	}
	
	return cue_quadrant;
}

/**
* @function cueBallMotionDetection(double threshold)

* @params threshold Defines the percentage of the surrounding square to be green to trigger a true response.  Note that this adds in the corner values, so a threshold that is too small will always return true.

* @brief A basic check for motion detection.  Will look at the image with the cue-ball in it and check if there is a threshold amount of green pixels where the cueball should be.  If there is, we assume the cue-ball has moved and return true.  Otherwise, we return false.

* @return Boolean value.
*/
bool cueBallMotionDetection(double threshold)
{
	int cue_quadrant = whichQuadrantIsTheCueIn();
	// Now that we know which quadrant the cueball is in, poll that camera.
	// This should be a low-resolution image.
	
	/*if(initState[cue_quadrant] != LOW_RES){
		initCamera(cue_quadrant, 640, 480);
		initState[cue_quadrant] = LOW_RES;
	}*/
	
	getPicture(0, "cue_ball_detection.pnm");
	
	// Load the image (it was saved to the hard drive).
	IplImage* img = cvLoadImage("cue_ball_detection.pnm", 1);
	if(img == 0)
	{
		printf("Failed to load image for cue ball motion detection\n");
		return false;
	}

	// Create an RGB map of the image for ease of use.
	RgbImage rgb(img);
	
	// Check the area of the cue ball.
	int ballSize = 84*640.0/1600;
	int row = locations[CUE].x;
	int col = locations[CUE].y;
	row = row*640.0/1600;
	col = col*480.0/1200;
	
	//set area to search
	int rowMin = row-ballSize/2;
	if(rowMin < 0) rowMin = 0;
	int rowMax = row+ballSize/2;
	if(rowMax > img->height) rowMax = img->height;
	int colMin = col - ballSize/2;
	if(colMin < 0) colMin = 0;
	int colMax = col + ballSize/2;
	if(colMax > img->width) colMax = img->width;
	
	
	int green_count = 0;
	
	IplImage *blank = cvCreateImage(cvSize(ballSize,ballSize), IPL_DEPTH_8U, 1);
	RgbImage blankRGB(blank);
	
	for(int j=rowMin; j< rowMax; j++)
	{
		for(int l=colMin; l< colMax; l++)
		{
			// Check all RGB values.  See if they are green enough.
			// half or less blue or red and more than one third green);
			//printf("RGB: (%d,%d,%d)\n", rgb[row][col].r, rgb[row][col].g, rgb[row][col].b);
			if(rgb[j][l].r < 255/2 && rgb[j][l].b < 255/2 && rgb[j][l].g > 255/3)
				green_count++;
			blankRGB[j-(row-ballSize/2)][l-(col-ballSize/2)].r = rgb[j][l].r;
			blankRGB[j-(row-ballSize/2)][l-(col-ballSize/2)].g = rgb[j][l].g;
			blankRGB[j-(row-ballSize/2)][l-(col-ballSize/2)].b = rgb[j][l].b;
		}
	}
	
	//cvSaveImage("cue_ball.jpg", blank);
	//cvReleaseImage(&img);
	printf("X: %d, Y: %d\tGreen: %d\n", row, col, green_count);
	
	// Threshold will be 400 out of 1000 pixels.
	if(green_count > ballSize*ballSize*threshold)
		return true;
	
	return false;

}

double distance(CvPoint* A, CvPoint* B)
{
	return sqrt( pow((B->x-A->x),2) + pow((B->y-A->y),2) );
}

/**
* @function findCueAngle(char* fileName, int quadrant_num)

* @params fileName The picture to search
* @params quadrant_num The quadrant number we are searching

* @brief Returns the angle the cue stick is currently facing.

* @return double The cue angle.
*/
double findCueAngle(int quadrant_num)
{
	static state_E cue_angle_state = cue_angle_tracking;

	//list<line_T> theLines;
	//int cue_quadrant = whichQuadrantIsTheCueIn();
	
	static int fdIn;
	static int fdOut;
	int i;
	
	static int first = 1;
	enum state_enum {TRACKING, NOT_TRACKING};
	static state_enum theCurrentState = NOT_TRACKING;
	
	if(guiState == CUE_TRACK)
		theCurrentState = TRACKING;
		
	if(theCurrentState == NOT_TRACKING) return -1;
	
	if(first){
		system("rm fifoToUi fifoToLineFinder");
		first = 0;
		fifoInfo_T setUpInfo;
		setUpInfo.cue_x = locations[CUE].x * 640.0 / 960;//1600; //change back to 1600 and 1200 if using highres
		setUpInfo.cue_y = locations[CUE].y * 480.0 / 720;//1200;
		setUpInfo.cue_quad = (global_cue_quad + 2) % 4;//whichQuadrantIsTheCueIn();
		setUpInfo.phi = phi;
		
		i = mkfifo("fifoToUi", 0666);
		if(i < 0) {
			//printf("Problems creating the fifo\n");
			if(errno == EEXIST) {
				//printf("fifo already exists\n");
			}
			//printf("errno is set as %d\n", errno);
		}
	
		i = mkfifo("fifoToLineFinder", 0666);
		if(i < 0) {
			//printf("Problems creating the fifo\n");
			if(errno == EEXIST) {
				//printf("fifo already exists\n");
			}
			//printf("errno is set as %d\n", errno);
		}
	
		
		
		//sleep(2);
		system("../image_rec/lines/lineFinderLoop 0 640 480 &");
		fdIn = open("fifoToUi", O_RDONLY);
		fdOut = open("fifoToLineFinder", O_WRONLY);
		//system("../camera/getPic 0 640 480 lowRes.pnm");
		
		write(fdOut, &setUpInfo, sizeof(setUpInfo));
	}
	
	fifoReturn_T result;
	i = 0;
	//while(result == 0){
		while(i < sizeof(result)){
			int j = read(fdIn, &result + i, sizeof(result) - i);
			if(j < 0){
				switch(errno){
				case EINTR:
					printf("The call was interrupted by a \
					        signal before any data was read.\n");
					break; 
				case EAGAIN:
					printf("Non-blocking I/O has been selected \
					       using O_NONBLOCK and no data was immediately available for reading. \n");
					break;
				case EIO:
					printf("I/O error. This will happen for example"
					       "when the process is in a background process group, "
					       "tries to read from its controlling tty, and either it "
					       "is ignoring or blocking SIGTTIN or its process group is "
					       "orphaned. It may also occur when there is a low-level "
					       "I/O error while reading from a disk or tape.\n");
					 break;
				case EISDIR:
					printf("fd refers to a directory.\n");
					break;
				case EBADF:
					printf("fd is not a valid file descriptor or is not open for reading.\n");
					break;
				case EINVAL:
					printf("fd is attached to an object which is unsuitable for reading.\n");
					break;
				case EFAULT:
					printf("buf is outside your accessible address space.\n");
					break;
				default:
					printf("unknown error %d\n", errno);
				}
				exit(1);
			}
			i += j;
		}
	//}
	
	cue_angle_state = result.currentState;
	//printf("received %f\ncue: %s\nmotion:%s\n", result.cue_angle, (result.cueMoved ? "true":"false"), (result.noMotion ? "true":"false"));
	printf("cue: %s, motion: %s, state: %s %d\n", (result.cueMoved ? "true":"false"),
	                                           (result.noMotion ? "true":"false"),
	                                           (result.currentState == cue_angle_tracking ?
	                                             "cue angle":(result.currentState == motion_detection ?
	                                             "motion detection":"done")), (int)result.currentState);
	
	//close serial port, will be opened again after getting pictures
	if(cue_angle_state == motion_detection){
		//cue.Close();
	}
	
	//TODO: will have to change this to false when doing actual motion detection
	if( cue_angle_state == done_cue_tracking ){
		guiState = GETPICTURES;
		theCurrentState = NOT_TRACKING;
		printf("stopping tracking\n");
		//cue.Close();
		close(fdIn);
		close(fdOut);
		first = 1;
	}
	
	if(guiState != CUE_TRACK && theCurrentState == TRACKING){
		printf("stopping tracking\n");
		//cue.Close();
		theCurrentState = NOT_TRACKING;
		char stopVal = 1;
		write(fdOut, &stopVal, 1);
		close(fdIn);
		close(fdOut);
		first = 1;
		cue_angle_state = cue_angle_tracking;
	}
	
	return result.cue_angle;
	

#ifndef NO_CAMERAS
	/*getPicture(quadrant_num,"cue_detection.pnm");
	//printf("got cue det pic\n");
	findLines("cue_detection.pnm", theLines);
	double slope, angle, best_angle = 0, best_dist = 5000;
	
	CvPoint cue_point;
	cue_point.x = locations[CUE].x;// * 640.0/1600;//_wrt_picture;
	cue_point.y = locations[CUE].y;// * 480.0/1200;//_wrt_picture;
	double angleSum = 0;
	
	for(list<line_T>::iterator it = theLines.begin(); it != theLines.end(); it++)
	{
		// Swap points 1 and 2 so that p1 is always the one closest to the cue ball
		if( distance(&(it->p1), &cue_point) > distance(&(it->p2), &cue_point))
		{
			CvPoint temp = it->p1;
			it->p1 = it->p2;
			it->p2 = temp;
		}
		
		//printf("cue: %f, %f\np1: %f, %f\np2: %f, %f\n\n", cue_point.x, cue_point.y,
		//       it->p1.x, it->p1.y, it->p2.x, it->p2.y);
		
		double xDiff = it->p1.x - it->p2.x;
		double yDiff = it->p1.y - it->p2.y;
		if(yDiff == 0) yDiff += .0000000001;
		
		slope = yDiff/xDiff;
		angle = atan( slope );
		angle = angle*180 / M_PI;
		
		// TODO: account for angle difference between images
		//this should be covered in the big if statement where we find the true angle
		if(quadrant_num == 0 || quadrant_num == 3) angle -= 90;
		
		//because we are using y/x??
		angle = 360 - angle;
		
		//used to extend the line so we can find the distance from the line to the cue ball
		vec2 np1;
		vec2 np2;
		//printf("found a line p1=(%d,%d)\tp2=(%d,%d)\n", it->p1.x, it->p1.y, it->p2.x, it->p2.y);
		
		
		//find true angle, i.e. with respect to table
		if(it->p1.x < it->p2.x){
			angle += 180;
			if(it->p1.y > it->p2.y){
				//if(quadrant_num == 0 || quadrant_num == 3) angle = 180 - angle; 
				//else angle = -angle;
				// 1
				
				np1.x = it->p1.x - 500*xDiff;
				np1.y = it->p1.y + 500*yDiff;
				np2.x = it->p2.x + 500*xDiff;
				np2.y = it->p2.y - 500*xDiff;
			}
			else{
				//if(quadrant_num == 0 || quadrant_num == 3) angle += 180;
				//else have correct angle
				// 4
				
				np1.x = it->p1.x - 500*xDiff;
				np1.y = it->p1.y - 500*yDiff;
				np2.x = it->p2.x + 500*xDiff;
				np2.y = it->p2.y + 500*xDiff;
				
			}
		}
		else{
			if(it->p1.y < it->p2.y){
				//if(quadrant_num == 0 || quadrant_num == 3) angle = -angle;
				//else angle = 180 - angle;
				// 2
				
				np1.x = it->p1.x + 500*xDiff;
				np1.y = it->p1.y - 500*yDiff;
				np2.x = it->p2.x - 500*xDiff;
				np2.y = it->p2.y + 500*xDiff;
			}
			else{
				//if(quadrant_num == 0 || quadrant_num == 3) angle = angle; //have correct angle
				//else angle = 180 + angle;
				// 3
	
				np1.x = it->p1.x + 500*xDiff;
				np1.y = it->p1.y + 500*yDiff;
				np2.x = it->p2.x - 500*xDiff;
				np2.y = it->p2.y - 500*xDiff;
			}
		}
		
		angle = fmod(angle, 360);
		
		fizPoint ballPos = myState.getBall(CUE).getPos();
		vec2 ballPoint(ballPos.x, ballPos.y);
		
		switch(quadrant_num){
		case 0:
			//ballPoint.x = myTable.getLength()/2.0 - ballPoint.y;
			//ballPoint.x = ballPoint.x * metersToPixels * 640/1600.0;
			
			//ballPoint.y = 480 - myTable.getWidth()/2.0 + ballPoint.x;
			//ballPoint.y = ballPoint.x * metersToPixels * 480/1200.0;
			//ballPoint.y = 
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}
		
		vec2 testPoint(cue_point.x, cue_point.y);
		vec2 p1(it->p1.x, it->p1.y);
		vec2 p2(it->p2.x, it->p2.y);
		//double distance = pointToLineDistance(p1, p2, testPoint);// ballPoint);

		double realSlope = slope; //to get y/x
		
		//filter out horizontal lines near the side of the table
		//they are most likely just the table
		if(fabs(realSlope) < .05){
			if(fabs(it->p1.y - 87) < 43.5 || it->p1.y > 460) 
				continue;
		}
		
		//filter out vertical lines near the side of the table
		//probably just the sides of the table
		if(fabs(realSlope) > 15){
			if(fabs(it->p1.x - 568) < 40 || it->p1.y < 20) 
				continue;
		}
		//if(fabs(distance - best_dist) < 5)
		{
			//distance = (distance + best_dist)/2;
			//angle = (angle + best_angle)/2;
		}
		//if( distance < best_dist ) 
		{
			//best_angle = angle;
			//best_dist = distance;
		}
		
		angleSum += angle;
	}

	//return best_angle;
	return angleSum / theLines.size();
	if(best_angle == 0) return cue_angle;*/
#else
	return 45;
#endif
}


 
/**
* @function motionDetection(void)

* @brief Checks to see if anything on the table is moving anymore.

* @return Boolean value.
*/
bool motionDetection(void)
{

	for(int i=0;i<1;i++){
		if(initState[i] != LOW_RES){
			initCamera(i, 640, 480);
			initState[i] = LOW_RES;
		}
	}

	int COLOR_THRESHOLD = 50;
	int PIXEL_THRESHOLD = 100;
	int pixel_count = 0;
	// column 0: new images
 	// column 1: old images

	for(int i=0;i<1;i++)
	{
		if(motionImage[i][1] != NULL)
			cvReleaseImage(&motionImage[i][1]);
		motionImage[i][1] = motionImage[i][0];
		motionImage[i][0] = NULL;
		char motion_filename[50];
		sprintf(motion_filename, "motion_detection_%d.pnm", i);
		getPicture(i, motion_filename);
		motionImage[i][0] = cvLoadImage(motion_filename, 1);
	}

	// Compare Images	
	for(int i=0;i<1;i++)
	{

		RgbImage rgb_new(motionImage[i][0]);
		if(motionImage[i][1] == NULL)
			continue;
		RgbImage rgb_old(motionImage[i][1]);
		int width = motionImage[i][0]->width;
		int height = motionImage[i][0]->height;

		for(int row=0; row<height; row++)
		{
			for(int col=0; col<width; col++)
			{
				if( abs(rgb_new[row][col].r - rgb_old[row][col].r) > COLOR_THRESHOLD ||
					abs(rgb_new[row][col].g - rgb_old[row][col].g) > COLOR_THRESHOLD || 
					abs(rgb_new[row][col].b - rgb_old[row][col].b) > COLOR_THRESHOLD )
					{
						pixel_count++;
					}
			}
		}
	}
	
	if(pixel_count > PIXEL_THRESHOLD) 
		return true;
	
	return false;
}

/*******************************************************************
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 * 				End of camera functions
 *~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
 *******************************************************************/ 

void reshapeCB(int w, int h)	/* generic 2D reshape callback */
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,w,0,h);
  glScalef(1.0,-1.0,1.0);	/* this flips the coordinate system so
				 that y increases going down the
				 screen (see below) */
  glTranslatef(0.0,-h,0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.375,0.375,0.0);/* so we can draw using integer
				   coordinates (see above) */
}
/*
 Note the flip of the y coordinate.  GLUT thinks the upper left hand
 corner of the screen or window is (0,0).  OpenGL, by default, thinks
 the lower left corner is (0,0).  This presents a problem when, for
 example, you compare the coordinates of a mouse event (from GLUT) with the
 coordinates of an OpenGL object.  The y coordinate flip makes OpenGL
 coordinates correspond to GLUT coordinates.
*/

/*-------------------- Frame ----------------------------*/
void processSpecialKeys(int, int, int);
void keyCanvasCB(unsigned char key, int x, int y);
/* emitString(s, x, y) -- displays string s at location x,y  */
void emitString(const char *s, int tx, int ty)
{
  int x,y;

  x = tx;
  y = ty;
  while( *s ) {
    if( *s == CARRIAGE_RETURN ) {
      x = tx;
      y += CHAR_H + LINE_SEP;
    } else {
      glRasterPos2i(x,y);
      glutBitmapCharacter(FONT,*s);
      x += CHAR_W;
   }
    ++s;
  }
}

string convertDouble(double value) {
  std::ostringstream o;
  if (!(o << value))
    return "";
  return o.str();
}

void displayFrameCB(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glColor3f(0.0,0.0,0.0); // set all the text to black
  emitString("Pool Table Analyzer",10,CHAR_H);
  emitString("Tim Langlois, Mitch Kendall, Doug Frazer, Dave Fraska",glutGet(GLUT_WINDOW_WIDTH)/2.0,CHAR_H);
  glutSwapBuffers();
}

void reshapeFrameCB(int tw, int th)
{
  int w,h;

  if( tw < MIN_W ) w = MIN_W; else w = tw;
  if( th < MIN_H ) h = MIN_H; else h = th;
  if( w != tw || h != th ) glutReshapeWindow(w,h);

  reshapeCB(w,h);

  glutSetWindow(canvasWin);
  glutPositionWindow(SIDE_WIDTH,FRAME_H);
  glutReshapeWindow(w,h-FRAME_H);
}


void openFrameWindow(void)
{
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  
  //glutInitWindowPosition( 1280-window.getWidth(), 1024-window.getHeight()-32 + RUL_H + FRAME_H );
  glutInitWindowSize( window.getWidth() + SIDE_WIDTH, window.getHeight() + FRAME_H + 200);

  frameWin = glutCreateWindow("Pool Table Analyzer");
  glClearColor(0,1,.5,0);		/* set clear color to middle-blue  (Red Green Blue Alpha) */

  glutDisplayFunc(displayFrameCB);
  glutReshapeFunc(reshapeFrameCB);
  glutKeyboardFunc(keyCanvasCB);
  glutSpecialFunc(processSpecialKeys);
}


/*---------------------- Canvas ----------------------------*/
void drawButtons();
void displayCanvasCB(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	static clock_t lasttime = clock();
	clock_t thistime = clock();
	double elapsed = (double)(thistime - lasttime) / CLOCKS_PER_SEC;
	lasttime = thistime;
	static double shotTime = 0.0;
	static bool startOfShot = true;
	bool moreanim = false;
	if (visState == ANIMATING) {
		if (startOfShot) shotTime = 0.0;
		else shotTime += elapsed * timeScale;
		moreanim = myShot.getStateAt(shotTime, myState);
	}

	if (moreanim) {
		visState = ANIMATING;
		startOfShot = false;
	}
	else if (visState == USER_MODE)
	{
		
	}
	else if (visState != PREPARING_SHOT) {
		postShot(postshotTime);
		startOfShot = true;
	}

	glClear( GL_COLOR_BUFFER_BIT );
	myTable.draw(window);
	myState.draw(window);
	if(visState == USER_MODE)
	{
		userMode();
	}
	updateParams();
	drawButtons();
	drawCue(cueDist, 500);
	glFlush();
	glutSwapBuffers();
}



void idleCanvasCB()
{
	if (firstVisit) {
		visState = IDLE;
		/*ball tempBall;
		tempBall.number = CUE;
		tempBall.x = 456;
		tempBall.y = 465;
		//tempBall.
		locations.push_back(tempBall);
		myState.setBall(CUE, STATIONARY, .5, .5);*/
		
		
		firstVisit = false;
	}
	
	if(guiState == GETPICTURES){
		grabStateFromCameras();
		//stateAwesome newState;
		//newState.balls = locations;
		//newState.statePlayer = currPlayer;
		prevState = myState;
		//stateList.push_front(newState);
		if(myState.getBall(CUE).getState() == STATIONARY){
			guiState = CUE_TRACK;
		}
		else{
			guiState = GUI_IDLE;
		}
		
		//cue.Close();
		//cue.Setup();
	}

	//if (visState == IDLE) loadNextShot();
	if (visState == IDLE ) ;// userMode();
	else if (visState == PREPARING_SHOT) prepareShot(preshotTime);
	else if (visState == POST_SHOT) postShot(postshotTime);
	else if (visState == ERROR) postShot(errorTime);
	glutPostRedisplay();
}

void keyCanvasCB(unsigned char key, int x, int y)
{
  printf("Got a key request");
  if (key == 27) 
  	exit(0);
  	
  if(key == 'q' || key == 'Q')
  {
  	myState.randomize();
  	visState = IDLE; 
  }
  
  if(key == 'n' || key == 'N')
  {
  	//grabStateFromCameras();
  	visState = IDLE;
  }
  
  if(key == ' ')
  {
  	tableEvaluateForThisShot = false;
  	animateButtonPressed = true;
  //if(key == 'l' || key == 'L')
  //{
  	//serialPort.Command('l');
  //}
  
  //if(key == 'r' || key == 'R')
  	//serialPort.Command('r');
  }

}

void processSpecialKeys(int key, int x, int y)
{
  //printf("Entered processSpecialKeys()\n");
  if(key == GLUT_KEY_LEFT)
  	leftArrowPressed = true;
  
  if(key == GLUT_KEY_RIGHT)
  	rightArrowPressed = true;
  	
  if(key == GLUT_KEY_UP)
  	upArrowPressed = true;
  
  if(key == GLUT_KEY_DOWN)
  	downArrowPressed = true;
  	
}

void mouseCanvasCB(int mousebutton, int state, int x, int y)
{
  if( state == GLUT_UP ) {	/* mouse button released */  
    prevX = x; prevY = y;		/* set the start position for moves */
    if( x > (0)*buttonW + buttonPadding && 
    	x < (1)*buttonW + buttonPadding  && 
    	y > window.getHeight() - (1)*buttonH - buttonPadding + 100 && 
    	y < window.getHeight() - (0)*buttonH - buttonPadding + 100) {
    	// within the bottom left hand button
    	printf("got a mouse click in the animate button\n");
    	guiState = GUI_IDLE;
    	animateButtonPressed = true;
    }
	if( x > (1)*buttonW + (2)*buttonPadding && 
    	x < (2)*buttonW + (2)*buttonPadding  && 
    	y > window.getHeight() - (1)*buttonH - buttonPadding + 100 && 
    	y < window.getHeight() - (0)*buttonH - buttonPadding + 100) {
    	// within the second bottom left hand button
    	printf("got a mouse click in the undo button\n");
    	checkRules();
		myState = prevState;
		//stateAwesome newState = stateList.front();
		//updateTableState(newState);
		//stateList.pop_front();
		//guiState = GUI_IDLE;
   	}
   	if( x > (2)*buttonW + (3)*buttonPadding && 
		x < (3)*buttonW + (3)*buttonPadding  && 
		y > window.getHeight() - (1)*buttonH - buttonPadding + 100 && 
		y < window.getHeight() - (0)*buttonH - buttonPadding + 100) {
    	// within the third bottom left hand button
    	printf("got a mouse click in the get pic button\n");
    	guiState = GETPICTURES;
    	updateParams();
   	}
   	if( x > (1)*buttonW + (2)*buttonPadding && 
		x < (2)*buttonW + (2)*buttonPadding  && 
		y > window.getHeight() - (2)*buttonH - buttonPadding + 100 && 
		y < window.getHeight() - (1)*buttonH - buttonPadding + 100) {
    	// within the fourth bottom left hand button
    	printf("got a mouse click in the cue detection button\n");
		if(guiState == CUE_TRACK)
			guiState = GUI_IDLE;
		else if(guiState == GUI_IDLE)
			guiState = CUE_TRACK;
			
		updateParams();
   	}
   	if( x > (1)*buttonW + (2)*buttonPadding && 
		x < (2)*buttonW + (2)*buttonPadding  && 
		y > window.getHeight() + (0)*buttonH + buttonPadding + 100 && 
		y < window.getHeight() + (1)*buttonH + buttonPadding + 100) {
		// within the fifth bottom left hand button
		printf("got a mouse click in the suggest button\n");
		guiState = GUI_IDLE;
		suggestButtonPressed = true;
   	}
   	if( x > (0)*buttonW + 1*buttonPadding && 
		x < (1.5)*buttonW + 1*buttonPadding  && 
		y > window.getHeight() - (1)*buttonH - buttonPadding + 30 && 
		y < window.getHeight() - (0)*buttonH - buttonPadding + 30) {
		// within the sixth bottom left hand button
		printf("got a mouse click in the switch player button\n");
		guiState = GUI_IDLE;
		switch(currPlayer)
		{
			case SOLIDS:
				currPlayer = STRIPES;
				currentShooter = stripes;
				break;
			case STRIPES:
				currPlayer = SOLIDS;
				currentShooter = solids;
				break;
			case WINNER_STRIPES:
				currPlayer = STRIPES;
				currentShooter = stripes;
				break;
			case WINNER_SOLIDS:
				currPlayer = SOLIDS;
				currentShooter = stripes;
				break;
		}
   	}
   	
   	if( x > (2)*buttonW + (3)*buttonPadding && 
		x < (3)*buttonW + (3)*buttonPadding  && 
		y > window.getHeight() - (1)*buttonH - buttonPadding + 30 && 
		y < window.getHeight() - (0)*buttonH - buttonPadding + 30) {
		// within the sixth bottom left hand button
		printf("got a mouse click in the randomize button\n");
		guiState = GUI_IDLE;

    	//stateAwesome newState;
    	//newState = pushNewState(myState);
    	//stateList.push_front(newState);
    	prevState = myState;
    	myState.randomize();
   	}
  }
}

/* passiveMotionCanvasCB is called whenever the mouse moves in the
   canvas window */
void passiveMotionCanvasCB(int x, int y)
{
  int deltax, deltay;
  prevX = x;  prevY = y;
  glutPostRedisplay();	/* <-------- force a display event */
}

void openCanvasWindow(void)
{
  canvasWin = glutCreateSubWindow(frameWin,FRAME_W,FRAME_H,canW + buttonW,canH + buttonH + 200);
  glClearColor(1,1,1,0);
  glutDisplayFunc(displayCanvasCB);	/* required */
  glutReshapeFunc(reshapeCB);
  glutMouseFunc(mouseCanvasCB);
  glutPassiveMotionFunc(passiveMotionCanvasCB);
  glutIdleFunc(idleCanvasCB);
  glutKeyboardFunc(keyCanvasCB);
  glutSpecialFunc(processSpecialKeys);
  
}

void updateParams()
{
	string str = "Current Player: ";
	if(currPlayer == SOLIDS) str += "Solids";
	else if(currPlayer == STRIPES) str += "Stripes";
	else if(currPlayer == WINNER_SOLIDS) str = "Solids has won!!";
	else if(currPlayer == WINNER_STRIPES) str = "Stripes has won!!";
	else str += "unknown";
	emitString(str.c_str(), 20, 12);

	str = "Velocity: ";
	str += convertDouble(V);
	emitString(str.c_str(), window.getWidth()/2.0 - 150, 12);

	str = "Phi: ";
	str += convertDouble(phi);
	emitString(str.c_str(), window.getWidth()/2.0, 12);


	str = "Cue Angle: ";
	str += convertDouble(cue_angle);
	emitString(str.c_str(), window.getWidth()/2.0 + 150, 12);
	
	
	
/*
	str = "State: ";
	if (guiState == GETPICTURES) str += "Getting Pictures...";
	else if (guiState == CUE_TRACK) str += "Tracking the cue stick...";
	else if (guiState == GUI_IDLE)
	{
		if (visState == USER_MODE) str += "User Mode";
		else if (visState == IDLE) str += "Idle";
		else if (visState == PREPARING_SHOT) str += "PREPARING SHOT...";
		else if (visState == ANIMATING) str += "ANIMATING...";
		else if (visState == POST_SHOT) str += "DONE.";
		else if (visState == ERROR) str += "ERROR!";
	}
	emitString(str.c_str(), window.getWidth() - 230, window.getHeight() - 10 );*/

	/*str = "Timescale: ";
	if (visSpeed == QUARTER) str += "1/4 speed";
	else if (visSpeed == HALF) str += "1/2 speed";
	else if (visSpeed == NORMAL) str += "Normal speed";
	else if (visSpeed == DOUBLE) str += "2x speed";
	else if (visSpeed == FOURTIMES) str += "4x speed";
	else str += "UNKNOWN!";
	emitString(str.c_str(), window.getWidth() - 530, window.getHeight() - 10 );*/
}

void drawButtons()
{
	int NUM_BUTTONS = 6;
	string str = "Animate";
	emitString(str.c_str(), buttonPadding + 5, window.getHeight() - buttonH + buttonPadding + 100);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (1)*buttonPadding + (0)*buttonW, window.getHeight() - buttonH - buttonPadding + 100); 
		glVertex2f( (1)*buttonPadding + (1)*buttonW, window.getHeight() - buttonH - buttonPadding + 100);
		glVertex2f( (1)*buttonPadding + (1)*buttonW, window.getHeight() - buttonPadding + 100); 
		glVertex2f( (1)*buttonPadding + (0)*buttonW, window.getHeight() - buttonPadding + 100);
	glEnd();
	
	str = "Suggest";
	emitString(str.c_str(), 2*buttonPadding + buttonW + 5, window.getHeight() + 1*buttonH - 1*buttonPadding + 100);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() - buttonH - buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() - buttonH - buttonPadding + 100);
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() - buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() - buttonPadding + 100);
	glEnd();
	
	str = "Get Pic";
	emitString(str.c_str(), 3*buttonPadding + 2*buttonW + 5, window.getHeight() - 1*buttonH + buttonPadding + 100);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (3)*buttonPadding + (2)*buttonW, window.getHeight() - buttonH - buttonPadding + 100); 
		glVertex2f( (3)*buttonPadding + (3)*buttonW, window.getHeight() - buttonH - buttonPadding + 100);
		glVertex2f( (3)*buttonPadding + (3)*buttonW, window.getHeight() - buttonPadding + 100); 
		glVertex2f( (3)*buttonPadding + (2)*buttonW, window.getHeight() - buttonPadding + 100);
	glEnd();
	
	str = "Undo";
	emitString(str.c_str(), 2*buttonPadding + 1*buttonW + 5, window.getHeight() - buttonH + buttonPadding + 100);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() + (1)*buttonH - (0)*buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() + (1)*buttonH - (0)*buttonPadding + 100);
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() + (0)*buttonH - (0)*buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() + (0)*buttonH - (0)*buttonPadding + 100);
	glEnd();
	
	str = "Get Cue";
	emitString(str.c_str(), 2*buttonPadding + buttonW + 5, window.getHeight() - 2*buttonH - 0*buttonPadding + 100);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() - 2*buttonH - 2*buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() - 2*buttonH - 2*buttonPadding + 100);
		glVertex2f( (2)*buttonPadding + (2)*buttonW, window.getHeight() - 1*buttonH - 2*buttonPadding + 100); 
		glVertex2f( (2)*buttonPadding + (1)*buttonW, window.getHeight() - 1*buttonH - 2*buttonPadding + 100);
	glEnd();
	
	str = "Change Players";
	emitString(str.c_str(), buttonPadding + 5, window.getHeight() - buttonH + buttonPadding + 30);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (1)*buttonPadding + (0)*buttonW, window.getHeight() - buttonH - buttonPadding + 30); 
		glVertex2f( (1)*buttonPadding + (1.5)*buttonW, window.getHeight() - buttonH - buttonPadding + 30);
		glVertex2f( (1)*buttonPadding + (1.5)*buttonW, window.getHeight() - buttonPadding + 30); 
		glVertex2f( (1)*buttonPadding + (0)*buttonW, window.getHeight() - buttonPadding + 30);
	glEnd();
	
	str = "Randomize";
	emitString(str.c_str(),  3*buttonPadding +2*buttonW + 5, window.getHeight() - buttonH + buttonPadding + 30);
	glBegin(GL_LINE_LOOP);
		glVertex2f( (3)*buttonPadding + (2)*buttonW, window.getHeight() - buttonH - buttonPadding + 30); 
		glVertex2f( (3)*buttonPadding + (3)*buttonW, window.getHeight() - buttonH - buttonPadding + 30);
		glVertex2f( (3)*buttonPadding + (3)*buttonW, window.getHeight() - buttonPadding + 30); 
		glVertex2f( (3)*buttonPadding + (2)*buttonW, window.getHeight() - buttonPadding + 30);
	glEnd();
	
	
	if(guiState != GETPICTURES)
		glColor3f(0.2f, 0.6f, 0.0f);
	else
		glColor3f(1.0f, 0.1f, 0.1f);
		
		

	glBegin(GL_QUADS);
		glVertex2f( 300, window.getHeight() + 160); 
		glVertex2f( 1000, window.getHeight() + 160);
		glVertex2f( 1000, window.getHeight() + 10); 
		glVertex2f( 300, window.getHeight() + 10);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 0.0f);
	str = "State: ";
	if (guiState == GETPICTURES) str += "Getting Pictures...";
	else if (guiState == CUE_TRACK) str += "Tracking the cue stick...";
	else if (guiState == GUI_IDLE)
	{
		if (visState == USER_MODE) str += "User Mode";
		else if (visState == IDLE) str += "Idle";
		else if (visState == PREPARING_SHOT) str += "PREPARING SHOT...";
		else if (visState == ANIMATING) str += "ANIMATING...";
		else if (visState == POST_SHOT) str += "DONE.";
		else if (visState == ERROR) str += "ERROR!";
	}
	
	emitString(str.c_str(), (1000+300)/2 - 60, window.getHeight() + (10+160)/2 - 10);
	if(guiState != GETPICTURES)
	{
		str = "It is OK to shoot";
		emitString(str.c_str(), (1000+300)/2 - 60, window.getHeight() + (10+160)/2 + 10);
	}
	else
	{
		str = "Wait while the computer is working";
		emitString(str.c_str(), (1000+300)/2 - 80, window.getHeight() + (10+160)/2 + 10);
	}
	
	str = errorString;
	emitString(str.c_str(), (1000+300)/2 - 60, window.getHeight() + (10+160)/2 + 50);
	

}

int main(int argc, char *argv[])
{
  int scrW,scrWmm;
  
  if(cue.Setup())
  	printf("Successfully opened the serial port\n");
  else
	printf("Failed to open the serial port\n");
  //if(serialPort.Setup())
  //	printf("successfully opened the serial port\n");
  //else
  //	printf("failed to open the serial port\n");
  
  	if(argc == 2)
  	{
  		fileMode = true;
  		fileName = argv[1];
  	}
  	
	else
		//myState.randomize();
		//grabStateFromCameras();
		
		
	visMode = AUTO;
	
	
	glutInit(&argc, argv);		/* initialize GLUT system */
	
	/* calculate pixels per 16th of an inch */
	scrW = glutGet(GLUT_SCREEN_WIDTH);
	scrWmm = glutGet(GLUT_SCREEN_WIDTH_MM);
	if( scrW == 0 || scrWmm == 0 ) pixsPer16th=89.0/16.0;
	else pixsPer16th = (scrW*25.4)/(scrWmm*16.0);

	openFrameWindow();		/* frameWin is the parent window */
	openCanvasWindow();		/* canvasWin is a subwindow of frameWin */

	visState = IDLE;
	glutMainLoop();
	return 0;

	if(argc > 4)
	{
		cout << "ERROR:  shotViz got too many command arguments." << endl;
		cout << "Try \"shotViz [MODE] [-rb if playing on robot]\" where MODE = -a for auto exit or -m for manual exit." << endl;
		return -1;
	}

}

