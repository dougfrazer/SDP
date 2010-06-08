#ifndef CIRCLEFINDER_H
#define CIRCLEFINDER_H

#include <vector>
#include <string>
#include "../../QMIN/poolfiz/bin/poolfiz.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


/* The BallType (number) of the ball. Can be: CUE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, ELEVEN, TWELVE, THIRTEEN, FOURTEEN, FIFTEEN, UNKNOWN_ID   */
// Added a temporary enum for the type of ball

//enum BallTurn { SOLIDS, STRIPES, CUE_BALL, EIGHT_BALL };

typedef struct
{
  float x;
  float y;
  float rad;
  float x_wrt_picture;
  float y_wrt_picture;
  BallType number;
  vector<int> histogramComparison;
  int whiteCount;
  int quad;
  //BallTurn number;
}ball;

typedef struct
{
	string picName;
	int quad;
	vector<ball> result;
} circleInputInfo;

struct ballXLT{
	bool operator() (const ball& b1, const ball& b2) {return b1.x < b2.x;}
};

struct ballYLT{
	bool operator() (const ball& b1, const ball& b2) {return b1.y < b2.y;}
};

/**********************************************
* Added Stuff
***********************************************/

template<class T> class Image
{
  private:
  IplImage* imgp;
  public:
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
  unsigned char b,g,r;
} RgbPixel;

typedef struct{
  float b,g,r;
} RgbPixelFloat;

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;

typedef struct{
	//memory needs to be cleaned up after
	CvMemStorage* storage;
	vector<ball> markers;
	int markerCount;
	float scale;
	float refX;
	float refY;
	float slope1;
}markerStruct;

/*******************************************************/



class CircleFinder
{
    public:
        CircleFinder();
        void* getLocations(void* input);
        void putTableTogether(vector<ball> &v1, vector<ball> &v2, vector<ball> &v3, vector<ball> &v4, vector<ball> &output, vector<ball> &prevState);

    protected:
        
    private:
    	void swapMarkers(int i, int j, markerStruct &markerInfo);
        void getBallColorAverage(float x, float y, int number);
        void initTableColor(int x, int y, int w, int h);
        bool checkPixelDifference(RgbPixel pix, BallType ball);
        
        int findGlare(int x, int y);
        int getCorrelation(int b, int r, int g);
        int getBallNumber(float x, float y);
        int countTable(float x, float y);
        
        void pickBalls();
        void initializer();
        void bootInfo();
        ball findMarker(int x, int y);
        void findBalls(char* fileName, int quad);
        void findMarkers(IplImage* img, int quad, markerStruct &markerInfo);
        void findBallsOneImage(IplImage* img, int quad, markerStruct &markerInfo, vector<ball> &theBalls);
        void filterBalls(vector<ball> &output, vector<ball> &v1, vector<ball> &v2, vector<ball> &v3, vector<ball> &v4);
        void addBallsToTable(vector<ball> &output, vector<ball> &v1);
        
    
    
};

#endif


