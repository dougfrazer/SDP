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

/****************************************************************
*
*	These are declared globally; don't modify these declarations.
*	You use myState, myTable and myShot in your code.  You call
*	the animateLastShot() function whenever you want to animate 
*	the most recent shot.  You can use the variables a, b, theta,
*	phi and V throughout your code.
*
****************************************************************/
fizGWindow window(1016, 508, 0.08);
fizGShot myShot;
fizGTable myTable;
fizGTableState myState(window, myTable);
fizGCue myCue(window, myTable.getLength(), myTable.getWidth());

void animateLastShot();
void setTimeScale(double ts);

double phi = 270.0;
double V = 1.0;
double a = 0.0;
double b = 0.0;
double theta = 0.0;

/****************************************************************
*
*	You should only add code to the "userMain" routine below;
*	it will be executed once when you press the "u" key from the
*	Pool Fiz graphics window.  Press the "Esc" key from the 
*	Pool Fiz window to quit the application.
*
*	The example below shows how to use the setTimeScale(), 
*	execute() and animateLastShot() functions, as well as check 
*	the shot parameters for validity.
*
****************************************************************/

void userMain()
{
	// Change the timescale if desired
	setTimeScale(1.0);
	
	// Check the shot parameters
	int cond1 = myTable.isValidBallPlacement( myState );
	int cond2 = myTable.isPhysicallyPossible( myState, a, b, theta, phi, V );
	
	// Execute a shot
	if (cond1 == 0 && cond2 == 0) {
		ShotStatus result = myShot.execute(myTable, myState, a, b, theta, phi, V, true);
	
		// Animate the shot if there were no errors
		if (result == OK) animateLastShot();
	}	
}


/****************************************************************
*
*	None of the remaining code *needs* to be modified.
*	You could modify the keyPress routine below to handle other 
*	key events if you really want to; do so at your own risk...
*
****************************************************************/
double turn_sensitivity = 5.0;
bool gameon = false;
void keyPress( unsigned char c, int x, int y )
{
	switch (c) {
		case 27: exit(0); break;  // exits on "Esc" keypress
		case 'u':
		case 'U':
			{
				userMain();
				glutPostRedisplay();
			}
			break;
		case 'r': 
		case 'R':
			{
				myState.randomize();
				gameon = true;				
				glutPostRedisplay();
			}
			break;
		case '8': 
		case '*':
			{
				myState.rack8ball();
				gameon = true;
				glutPostRedisplay();
			}
			break;		
		case '9': 
		case '(':
			{
				myState.rack9ball();
				gameon = true;
				glutPostRedisplay();
			}
			break;
			
			
		case 'g':
		case 'G':
			{
				phi += turn_sensitivity;
				if (phi > 360.0) phi = 0.0;
				if (phi < 0.0) phi = 360.0;
				glutPostRedisplay();
			}
			break;
		case 'h':
		case 'H':
			{
				phi -= turn_sensitivity;
				if (phi > 360.0) phi = 0.0;
				if (phi < 0.0) phi = 360.0;
				glutPostRedisplay();
			}
			break;
		case 'i':
		case 'I':
			{
				if (turn_sensitivity == 0.5) turn_sensitivity = 5.0;
				else if (turn_sensitivity == 5.0) turn_sensitivity = 0.5;
				glutPostRedisplay();
			}
			break;

		case '+':
		case '=':
			{
				V += 0.1;
				if (V > 4.5) V = 4.5;
				if (V < 0.0) V = 0.0;
				glutPostRedisplay();
			}
			break;
		case '-':
		case '_':
			{
				V -= 0.1;
				if (V > 4.5) V = 4.5;
				if (V < 0.0) V = 0.0;
				glutPostRedisplay();
			}
			break;
		case 'a':
		case 'A':
			{
				a += 0.5;
				if (a > 35.0) a = 35.0;
				if (a < -35.0) a = -35.0;
				glutPostRedisplay();
			}
			break;
		case 'z':
		case 'Z':
			{
				a -= 0.5;
				if (a > 35.0) a = 35.0;
				if (a < -35.0) a = -35.0;
				glutPostRedisplay();
			}
			break;
		case 'd':
		case 'D':
			{
				b += 0.5;
				if (b > 35.0) b = 35.0;
				if (b < -35.0) b = -35.0;
				glutPostRedisplay();
			}
			break;
		case 'c':
		case 'C':
			{
				b -= 0.5;
				if (b > 35.0) b = 35.0;
				if (b < -35.0) b = -35.0;
				glutPostRedisplay();
			}
			break;
			
		case 'w':
		case 'W':
			{
				theta += 1;
				if (theta > 75.0) theta = 75.0;
				if (theta < 0.0) theta = 0.0;
				glutPostRedisplay();
			}
			break;
		case 'x':
		case 'X':
			{
				theta -= 1;
				if (theta > 75.0) theta = 75.0;
				if (theta < 0.0) theta = 0.0;
				glutPostRedisplay();
			}
			break;
			
		case 's': 
		case 'S':
			{
				myShot.save();
				glutPostRedisplay();
			}
			break;
		case 'l': 
		case 'L':
			{
				string fname;
				cout << "enter the filename: ";
				cin >> fname;
				myShot.load(fname, myState, a, b, theta, phi, V);
				gameon = true;
				glutPostRedisplay();
			}
			break;
		default:
			{
				glutPostRedisplay();
			}
			break;
	} // end switch
}

/****************************************************************
*
*	You shouldn't modify any of the remaining code unless you 
*	know what you're doing!
*
****************************************************************/
bool animating = false;
double shotTime = 0.0;
double timeScale = 1.0;

void setTimeScale(double ts)
{
	if (ts < 0.01) timeScale = 0.01;
	else if (ts > 100.0) timeScale = 100.0;
	else timeScale = ts;
}

void animateLastShot()
{
	animating = true;
}

void idle()
{
	glutPostRedisplay();
}

void updateParams()
{
	string str = "Sensitive Rotate: ";
	if (turn_sensitivity == 0.5) str += "On";
	if (turn_sensitivity == 5.0) str += "Off";
	
	glColor3f( 1,1,1 );
	glRasterPos2f( 5, 15 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	}
	
	stringstream s4;
	s4 << theta;
	str = "theta = " + s4.str() + " degrees";

	glRasterPos2f( 5, 30 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 

	stringstream s1;
	s1 << V;
	str = "V = " + s1.str() + " m/s";

	glRasterPos2f( 5, 45 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 

	stringstream s2;
	s2 << a;
	str = "a = " + s2.str() + " mm";

	glRasterPos2f( 5, 60 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 

	stringstream s3;
	s3 << b;
	str = "b = " + s3.str() + " mm";

	glRasterPos2f( 5, 75 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 

	stringstream s5;
	s5 << shotTime;
	str = "t = " + s5.str() + " s";

	glRasterPos2f( 5, window.getHeight() - 25 );
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 

	stringstream s6;
	s6 << timeScale;
	str = "scale = " + s6.str();

	glRasterPos2f( 5, window.getHeight() - 10);
	for (int i=0; i<str.length(); i++) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, (GLubyte) str.at(i) );
	} 
}

void drawCue( double distance )
{
	double x, y;
	BallState curState;
	myState.getBall( CUE , curState, x, y );
	
	if (!animating && gameon) {
		myCue.draw( x , y , phi , distance );
	}
}

clock_t lasttime = clock();
double elapsed = 0.0;

void display()
{
	static bool lastanim = false;
	clock_t thistime = clock();
	elapsed = (double)(thistime - lasttime) / CLOCKS_PER_SEC;
	lasttime = thistime;
	glClear( GL_COLOR_BUFFER_BIT );
	myTable.draw(window);
	
	bool moreanim = false;
	if (!lastanim && animating) {
		shotTime = 0.0f;
		moreanim = myShot.getStateAt(shotTime, myState);
		lastanim = true;
	}
	else if (lastanim && animating) {
		
		shotTime += elapsed * timeScale;
		moreanim = myShot.getStateAt(shotTime, myState);
		lastanim = true;
	}
	else if (lastanim && !animating) {
		lastanim = false;
	}
	
	myState.draw(window);
	updateParams();
	drawCue(20);
	
	animating = moreanim;
	glFlush();
	glutSwapBuffers();
}

void mouse( int button, int state, int x, int y )
{
	glutPostRedisplay();
}

void mouseMotion( int x, int y )
{
	glutPostRedisplay();
}

int main( int argc, char **argv )
{
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInit( &argc, argv );

	glutInitWindowPosition( 0, 768-window.getHeight()-32 );
	glutInitWindowSize( window.getWidth() , window.getHeight() );
	glutCreateWindow( "Poolfiz" );

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

