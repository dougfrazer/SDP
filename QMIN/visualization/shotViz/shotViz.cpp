#include "poolfiz.h"

#ifdef __APPLE_CC__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
  #include <GL/glu.h>
  #include <GL/gl.h>
#endif // APPLE_CC

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
using namespace std;

/* Diplayed Info */
string currentShooter = "";
string currentBall = "";
string player1 = "";
string player2 = "";
string player1Points = "";
string player2Points = "";
string player1Wins = "";
string player2Wins = "";
string player1Time = "";
string player2Time = "";
string isOpenTable = "";
string isSafetyShot = "";
string sCalledBall = "";
string sCalledPocket = "";
int iCalledBall = 0;
int iCalledPocket = 0;
/* end Display Info */

/* Main stuff */
enum shotVizMode { MANUAL, AUTO, UNKNOWN_SHOTVIZMODE };
enum shotVizSpeed { QUARTER, HALF, NORMAL, DOUBLE, FOURTIMES, UNKNOWN_SHOTVIZSPEED };
enum shotVizState { IDLE, PREPARING_SHOT, ANIMATING, LOADING, POST_SHOT, ERROR, UNKNOWN_SHOTVIZSTATE };
shotVizState visState = IDLE;
shotVizMode visMode = MANUAL;
shotVizSpeed visSpeed = NORMAL;
string indexFilename = "shotViz_info.txt";
string shotInfo = "";
bool exitKeyed = false;
bool firstVisit = true;

/* Poolfiz stuff */
fizGWindow window(1016, 508, 0.08);
fizGShot myShot;

double length = 2.4384;               //the length of the table in metres
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
double railVelAngleAdj = 0.001;       //velocity deflection factor of the banks
//onRobot indicates whether the game is being played on the robotic table (optional, default = false)

fizGTable myTable(length,
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

fizGTableState myState(window, myTable);
fizGCue myCue(window, myTable.getLength(), myTable.getWidth());

double phi = 270.0;
double lastPhi = phi;
double nextPhi = phi;
double V = 1.0;
double a = 0.0;
double b = 0.0;
double theta = 0.0;

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

void setTimeScale(double ts)
{
	if (ts < 0.1) timeScale = 0.1;
	else if (ts > 30.0) timeScale = 30.0;
	else timeScale = ts;
}

/* Functions */
void keyPress( unsigned char c, int x, int y )
{
	switch (c) {
		case 27: exit(0); break;  // exits on "Esc" keypress
			
		case 'r':
		case 'R':
		{	
			if (visState == POST_SHOT || visState == ERROR) {
				firstVisit = true;
				visState = IDLE;
			}			
		}
		break;
		
		
		default: exitKeyed = ( (visMode == MANUAL) && (visState == ERROR || visState == POST_SHOT) ); break;
	} // end switch
}

void postShot(double secs)
{
	static clock_t start = clock();
	if (visState == POST_SHOT || visState == ERROR) {
		double waitSecs = secs/timeScale;
		double elapsed = (double)(clock() - start)/CLOCKS_PER_SEC;
		double timeLeft = waitSecs - elapsed;
		if ( elapsed > waitSecs ) {
			if (visMode == AUTO) exit(0);
			else if (visMode == MANUAL && exitKeyed) exit(0);
		}
	}
	else {
		start = clock();
		visState = POST_SHOT;
	}
}

void animateLastShot()
{
	phi = nextPhi;
	iCalledBall = 0;
	ShotStatus result = myShot.execute(myTable, myState, a, b, theta, phi, V, false);
	if (result == OK) visState = ANIMATING;
	else {
		cout << "ERROR:  shotViz could not play back shot from file; poolfiz returned an error." << endl;
		visState = ERROR;
		postShot(postshotTime);
	}
}

void prepareShot(double secs)
{
	static clock_t start = clock();
	if (visState == PREPARING_SHOT) {
		double waitSecs = secs/timeScale;
		double elapsed = (double)(clock() - start)/CLOCKS_PER_SEC;
		double timeLeft = waitSecs - elapsed;
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
		cout << "ERROR:  shotViz could not load shot from file " << fileName << "." << endl;
		visState = ERROR;
		postShot(errorTime);
	}
}

void idle()
{
	if (firstVisit) {
		visState = IDLE;
		firstVisit = false;
	}		
	if (visState == IDLE ) loadNextShot();
	else if (visState == PREPARING_SHOT) prepareShot(preshotTime);
	else if (visState == POST_SHOT) postShot(postshotTime);
	else if (visState == ERROR) postShot(errorTime);
	glutPostRedisplay();
}

void updateParams()
{
	string str = "Current Player: ";
	str += currentShooter;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Shooting For: ";
	str += currentBall;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Called Safety: ";
	str += isSafetyShot;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Table Open: ";
	str += isOpenTable;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Player A: ";
	str += player1;
	glColor3f( 1,1,1 );
	glRasterPos2f( 205 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Player B: ";
	str += player2;
	glColor3f( 1,1,1 );
	glRasterPos2f( 355 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Wins: ";
	str += player1Wins;
	glColor3f( 1,1,1 );
	glRasterPos2f( 205 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Wins: ";
	str += player2Wins;
	glColor3f( 1,1,1 );
	glRasterPos2f( 355 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Points: ";
	str += player1Points;
	glColor3f( 1,1,1 );
	glRasterPos2f( 205 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Points: ";
	str += player2Points;
	glColor3f( 1,1,1 );
	glRasterPos2f( 355 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Time Left: ";
	str += player1Time;
	glColor3f( 1,1,1 );
	glRasterPos2f( 205 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Time Left: ";
	str += player2Time;
	glColor3f( 1,1,1 );
	glRasterPos2f( 355 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Called Ball: ";
	str += sCalledBall;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 70 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Called Pocket: ";
	str += sCalledPocket;
	glColor3f( 1,1,1 );
	glRasterPos2f( 10 , 82 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	/* put called ball and pocket info and display here
	if ( iCalledBall != 0 )  {

		double winWidth = 1016;
		double winHeight = 508;
		double winBuff = 0.12;

		double yscale = ( (int) ( winWidth*(1-2*winBuff) ) ) / 2.4384 ;
		double xscale = ( (int) ( winWidth*(1-2*winBuff) ) ) / 2 / 1.2192 ;

		double xoff = winBuff*winHeight ;
		double yoff = winBuff*winWidth ;

		fizBall curBall = myState.getBall ( (BallType) iCalledBall );

		str = "X <- Called Ball";
		glColor3f( 0,0,0 );
		glRasterPos2f( yoff + yscale * curBall.getPos().y, xoff + xscale * curBall.getPos().x );
		for (int i=0; i<str.length(); i++) {
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
		}

		fizPoint calledPocket;

		if (iCalledPocket == 1)
			calledPocket = myTable.getSWpocket();
		else if (iCalledPocket == 2)
			calledPocket = myTable.getWpocket ();
		else if (iCalledPocket == 3)
			calledPocket = myTable.getNWpocket();
		else if (iCalledPocket == 4)
			calledPocket = myTable.getNEpocket();
		else if (iCalledPocket == 5)
			calledPocket = myTable.getEpocket ();
		else if (iCalledPocket == 6)
			calledPocket = myTable.getSEpocket();

		str = "X <- Called Pocket";
		glColor3f( 1,1,1 );
		glRasterPos2f( yoff + yscale * calledPocket.y, xoff + xscale * calledPocket.x );
		for (int i=0; i<str.length(); i++) {
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
		}

	}
	*/
	// put called ball and pocket info and display here

	str = "State: ";
	if (visState == IDLE) str += "IDLE.";
	else if (visState == PREPARING_SHOT) str += "PREPARING SHOT...";
	else if (visState == ANIMATING) str += "ANIMATING...";
	else if (visState == POST_SHOT) str += "DONE.";
	else if (visState == ERROR) str += "ERROR!";
	glColor3f( 1,1,1 );
	glRasterPos2f( window.getWidth() - 230, window.getHeight() - 10 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Mode: ";
	if (visMode == MANUAL) str += "MANUAL EXIT";
	else str += "AUTO EXIT";
	glColor3f( 1,1,1 );
	glRasterPos2f( window.getWidth() - 355, window.getHeight() - 10 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Timescale: ";
	if (visSpeed == QUARTER) str += "1/4 speed";
	else if (visSpeed == HALF) str += "1/2 speed";
	else if (visSpeed == NORMAL) str += "Normal speed";
	else if (visSpeed == DOUBLE) str += "2x speed";
	else if (visSpeed == FOURTIMES) str += "4x speed";
	else str += "UNKNOWN!";
	glColor3f( 1,1,1 );
	glRasterPos2f( window.getWidth() - 530, window.getHeight() - 10 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}
}

void drawCue( double distance )
{
	double x, y;
	BallState curState;
	if (visState != ANIMATING) {
		myState.getBall( CUE , curState, x, y );
		myCue.draw( x , y , phi , distance );
	}
}

void display()
{
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
	else if (visState != PREPARING_SHOT) {
		postShot(postshotTime);
		startOfShot = true;
	}

	glClear( GL_COLOR_BUFFER_BIT );
	myTable.draw(window);
	myState.draw(window);
	updateParams();
	drawCue(cueDist);
	glFlush();
	glutSwapBuffers();
}

void mouse( int button, int state, int x, int y ) {}

void mouseMotion( int x, int y ) {}

int main( int argc, char **argv )
{

	//printf("%f, %f\n", myTable.getLength(), myTable.getWidth());
	//return 0;
	
	if (argc <= 3) {
		visMode = AUTO;
		string thisArg;
		int argIdx = 1;
		while (argIdx < argc) {
			string thisArg = argv[argIdx];
			if (thisArg == "-m") visMode = MANUAL;	
			else if (thisArg == "-rb") myTable.playingOnRobot(true);
			else cout << "Unknown argument \"" << thisArg << "\"" << endl;
			argIdx++;
		}
		
		ifstream file(indexFilename.c_str());

		string line;
		while (getline(file,line)) {
			if ( line.length() > 4 ) shotInfo = line;
		}
		
		file.close();

		glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
		glutInit( &argc, argv );

		glutInitWindowPosition( 1280-window.getWidth(), 1024-window.getHeight()-32 );
		glutInitWindowSize( window.getWidth() , window.getHeight() );
		glutCreateWindow( "Shot Visualizer" );

		glutDisplayFunc( display );
		glutMouseFunc( mouse );
		glutMotionFunc( mouseMotion );
		glutKeyboardFunc( keyPress );
		glutIdleFunc( idle );

		gluOrtho2D(0, window.getWidth(), window.getHeight(), 0 );

		visState = IDLE;
		glutMainLoop();
		return 0;

	}
	else {
		cout << "ERROR:  shotViz got too many command arguments." << endl;
		cout << "Try \"shotViz [MODE] [-rb if playing on robot]\" where MODE = -a for auto exit or -m for manual exit." << endl;
		return -1;
	}
}
