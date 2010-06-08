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
string currentShoter = "";
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
string isSaftyShot = "";

string sCalledBall = "";
string sCalledPocket = "";

int iCalledBall = 0;
int iCalledPocket = 0;
/* end Display Info */

/* Main stuff */
enum dbVizMode { MANUAL, AUTO, UNKNOWN_DBVIZMODE };
enum dbVizSpeed { QUARTER, HALF, NORMAL, DOUBLE, FOURTIMES, UNKNOWN_DBVIZSPEED };
enum dbVizState { IDLE, PREPARING_SHOT, ANIMATING, LOADING, UNKNOWN_DBVIZSTATE };
dbVizState visState = IDLE;
dbVizMode visMode = MANUAL;
dbVizSpeed visSpeed = NORMAL;
string dirName = "";
vector<string> shots;
int shotIdx = 0;

/* Poolfiz stuff */
fizGWindow window(1016, 508, 0.08);
fizGShot myShot;
fizGTable myTable;
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
double prepareTime = 0.25;
double drawBackTime = 0.8;
double pushForwardTime = 0.4;
double timeScale = 1.0;
bool matchOver = false;

void setTimeScale(double ts)
{
	if (ts < 0.1) timeScale = 0.1;
	else if (ts > 10.0) timeScale = 10.0;
	else timeScale = ts;
}

/* Functions */
void keyPress( unsigned char c, int x, int y )
{
	switch (c) {
		case 27: exit(0); break;  // exits on "Esc" keypress
		case 'm':
		case 'M':
			{
				if (visMode == MANUAL) visMode = AUTO;
				else if (visMode == AUTO) visMode = MANUAL;
				else {
					cout << "ERROR in keyPress()  :  visMode is set to UNKNOWN_DBVIZMODE.  Setting to AUTO." << endl;
					visMode = MANUAL;
				}
			}
			break;

		case 'x':
		case 'X':
			{
				visState = IDLE;
			}
			break;

		case 'n':
		case 'N':
			{
				loadNextShot();
			}
			break;

		case 's':
		case 'S':
			{
				if (visSpeed == QUARTER) {
					visSpeed = HALF;
					setTimeScale(0.5);
				}
				else if (visSpeed == HALF) {
					visSpeed = NORMAL;
					setTimeScale(1.0);
				}
				else if (visSpeed == NORMAL) {
					visSpeed = DOUBLE;
					setTimeScale(2.0);
				}
				else if (visSpeed == DOUBLE) {
					visSpeed = FOURTIMES;
					setTimeScale(4.0);
				}
				else if (visSpeed == FOURTIMES) {
					visSpeed = QUARTER;
					setTimeScale(0.25);
				}
				else {
					cout << "ERROR in keyPress()  :  visSpeed is set to UNKNOWN_DBVIZSPEED.  Setting to NORMAL." << endl;
					visSpeed = NORMAL;
					setTimeScale(1.0);
				}
			}
			break;
		default: break;
	} // end switch
}

void animateLastShot()
{
	phi = nextPhi;
	iCalledBall = 0;
	ShotStatus result = myShot.execute(myTable, myState, a, b, theta, phi, V, true);
	if (result == OK) visState = ANIMATING;
	else {
		cout << "ERROR:  dbViz could not play back shot from file; Poolfiz returned error.  Skipping this shot." << endl;
		visState = IDLE;
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
	if (shotIdx < shots.size()) {
		// get next shot from file
		//string currFilename = dirName + "/" + shots.at(shotIdx);

		string fileName, currLine = shots.at(shotIdx);

		stringstream s;
		s << currLine;
		s >> fileName >> currentShoter >> currentBall >> isOpenTable >> isSaftyShot >> player1 >> player1Time >> player1Wins >> player1Points >> player2 >> player2Time >> player2Wins >> player2Points >> sCalledBall >> sCalledPocket;

		string currFilename = dirName + "/" + fileName;

		cout << "break 0: " << currFilename << endl ;

		char cCalledBall[2];
		char cCalledPocket[2];

		for ( int i = 0 ; i < sCalledBall.length() ; i++ )
		{
			cCalledBall[i] = sCalledBall.at(i);
		}

		for ( int i = 0 ; i < sCalledPocket.length() ; i++ )
		{
			cCalledPocket[i] = sCalledPocket.at(i);
		}

		iCalledBall = atoi(cCalledBall);
		iCalledPocket = atoi(cCalledPocket);

		//	string::size_type length = currFilename.length();
		//	string::size_type end = currFilename.find(".log",0);
		//	currFilename = currFilename.substr(0, end+4);

		bool ok = myShot.load( currFilename, myState, a, b, theta, nextPhi, V );

		cout << "break 1 " << a << ", " << b << ", " << theta << ", " << nextPhi << ", " << V << endl ;

		shotIdx++;
		if (ok) {
			lastPhi = phi;
			prepareShot(1.0);
		}
		else {
			visState = IDLE;
			cout << "ERROR:  dbViz could not load shot from file " << currFilename << ".  Skipping this shot." << endl;
		}
	}
	else {
		cout << "Game/match over." << endl;
		matchOver = true;
	}
}

void idle()
{
	if (visMode == AUTO) {
		if (visState == IDLE && !matchOver ) loadNextShot();
		else if (visState == PREPARING_SHOT) prepareShot(3.0);
	}
	else {
		if (visState == PREPARING_SHOT) prepareShot(3.0);
	}

	glutPostRedisplay();
}

void updateParams()
{
	string str = "Currently Shooting: ";
	str += currentShoter;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Shooting For Type: ";
	str += currentBall;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Safety: ";
	str += isSaftyShot;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Open Table: ";
	str += isOpenTable;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Player A: ";
	str += player1;
	glColor3f( 1,1,1 );
	glRasterPos2f( 225 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Player B: ";
	str += player2;
	glColor3f( 1,1,1 );
	glRasterPos2f( 375 , 12 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Wins: ";
	str += player1Wins;
	glColor3f( 1,1,1 );
	glRasterPos2f( 225 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Wins: ";
	str += player2Wins;
	glColor3f( 1,1,1 );
	glRasterPos2f( 375 , 24 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Points: ";
	str += player1Points;
	glColor3f( 1,1,1 );
	glRasterPos2f( 225 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Points: ";
	str += player2Points;
	glColor3f( 1,1,1 );
	glRasterPos2f( 375 , 36 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Time Left: ";
	str += player1Time;
	glColor3f( 1,1,1 );
	glRasterPos2f( 225 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Time Left: ";
	str += player2Time;
	glColor3f( 1,1,1 );
	glRasterPos2f( 375 , 48 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Called Ball: ";
	str += sCalledBall;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 70 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Called Pocket: ";
	if (iCalledPocket == 0) str += "SW";
	else if (iCalledPocket == 1) str += "W";
	else if (iCalledPocket == 2) str += "NW";	
	else if (iCalledPocket == 3) str += "NE";
	else if (iCalledPocket == 4) str += "E";
	else if (iCalledPocket == 5) str += "SE";
	else str += "?";	
	//str += sCalledPocket;
	glColor3f( 1,1,1 );
	glRasterPos2f( 30 , 82 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	// put called ball and pockt info and display here
	/*if ( iCalledBall != 0 )
	{

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

	}*/
	// put called ball and pockt info and display here

	str = "State: ";
	if (visState == IDLE) str += "IDLE.";
	else if (visState == ANIMATING) str += "ANIMATING...";
	else if (visState == PREPARING_SHOT) str += "PREPARING SHOT...";
	glColor3f( 1,1,1 );
	glRasterPos2f( window.getWidth() - 230, window.getHeight() - 10 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}

	str = "Mode: ";
	if (visMode == MANUAL) str += "MANUAL";
	else str += "AUTO";
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
		visState = IDLE;
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
	int maxArgs = 2;
	if (argc > 1) {
		if (argc-1 <= maxArgs) {
			int argIdx = 1;
			string thisArg = "";
			while (argIdx < argc) {
				thisArg = argv[argIdx];
				if (thisArg == "-a") visMode = AUTO;
				else if (thisArg == "-m") visMode = MANUAL;
				else dirName = thisArg;
				argIdx++;
			}

			string line;
			string indexFilename = dirName + "/" + "index_" + dirName + ".log";

			cout << "Reading index file " << indexFilename << "..." << endl;
			ifstream file(indexFilename.c_str());

			while (getline(file,line))
			{
				// check if line has a comment flag (#); if so, ignore it
				string::size_type pos = line.find("#",0);
				string::size_type length = line.length();
				if ( ( pos == string::npos ) && ( length > 0 ) )
				{
					shots.push_back(line);
				}
			}

			cout << "Found the following shot files: " << endl;
			for (int i=0; i < shots.size(); i++) {
				string str = shots.at(i);
				cout << str << endl;
			}

			if ( shots.size() < 1 ) {
				cout << "ERROR:  dbViz could not read shot list from " << indexFilename << "." << endl;
				return -1;
			}
			else {
				glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
				glutInit( &argc, argv );

				glutInitWindowPosition( 0, 768-window.getHeight()-32 );
				glutInitWindowSize( window.getWidth() , window.getHeight() );
				glutCreateWindow( "Match Visualization" );

				glutDisplayFunc( display );
				glutMouseFunc( mouse );
				glutMotionFunc( mouseMotion );
				glutKeyboardFunc( keyPress );
				glutIdleFunc( idle );

				gluOrtho2D(0, window.getWidth(), window.getHeight(), 0 );

				string vers = getPoolfizVersion();
				cout << "Initializing " << vers << endl;

				glutMainLoop();
				return 0;
			}
		}
		else {
			cout << "ERROR:  dbViz got too many command arguments." << endl;
			cout << "Try \"dbViz [MODE] directory_name\" where MODE = -a for autoplay or -m for manual play." << endl;
			return -1;
		}
	}
	else {
		cout << "ERROR:  dbViz needs a directory name to look in." << endl;
		cout << "Try \"dbViz [MODE] directory_name\" where MODE = -a for autoplay or -m for manual play." << endl;
		return -1;
	}
}
