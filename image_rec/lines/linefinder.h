#ifndef LINEFINDER_H
#define LINEFINDER_H
#include <cv.h>
#include <highgui.h>
#include <list>

using namespace std;

typedef struct{
  CvPoint p1;
  CvPoint p2;
} line_T;

enum state_E {cue_angle_tracking, motion_detection, done_cue_tracking};

typedef struct{
	int cue_x;
	int cue_y;
	int cue_quad;
	double phi;
} fifoInfo_T;

typedef struct{
	double cue_angle;
	int cueMoved;
	int noMotion;
	state_E currentState;
} fifoReturn_T;

void findLines(const char* fileName, list<line_T> &theLines, int quad_num);


#endif
