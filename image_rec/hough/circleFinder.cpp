//#include "../opencv-1.1.0/cv/include/cv.h"
//#include "../opencv-1.1.0/otherlibs/highgui/highgui.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "circleFinder.h"
#include <set>

using namespace std;

//for mitch
//g++ -I/usr/local/include/opencv -L/usr/local/lib -lcv -lcvaux -lcxcore -lhighgui -lstdc++ -Wall -g -o circleFinder circleFinder.cpp


	/* Assuming table to look like this:
	*  |------------------------------|
	*  |   1          |     2         |
	*  |              |               |
	*  |------------------------------|
	*  |   4          |     3         |
	*  |              |               |
	*  |------------------------------|
	*/
	
uchar* data;
uchar* grayData;
int height;
int width;
int step;
int headerSize = 15;
int channels;
int grayHeight;
int grayWidth;
int grayStep;
int grayChannels;
int param1 = 66;//30;
int param2 = 24;
float Mradius = 7;//11;
float MradVar = 7;//11;
int markerToEdge = 60;//100;
int dp = 1;
float radius = 23.4;//39;
float radVar = 10.2;//17;
float radiusMM = .028;//.057;
int minDistance = 2*radius - radVar;
int minRadius = radius - radVar;
int maxRadius = radius + radVar;
float fullTableY = 75*25.4; //mm, 25.4(mm/inch) * 75 inches
float fullTableX = fullTableY/2;
int yPixels = 720;//1200;
int xPixels = 960;//1600;
int ballCount;
double markerXBorder;
double markerYBorder;
float xPixelsPicture;
float yPixelsPicture;
const int binsPerColor = 16;
const int bins = 256 / binsPerColor;
const int pixelsPerBall = 4765;
int colorHistogram[bins][bins][bins];
//int colorHistogram[3][256];
int setColorHistograms[16][bins][bins][bins];
//int setColorHistograms[3][16][3][256];

//char* pic;
//IplImage* img;
int argument;
int greyScale = 20;

//colors for the table
int bTable = 0;
int gTable = 0;
int rTable = 0;

const float MarkerDistance = 24.5*(25.4); //inches * (millimeters/inch)

int blueInit[16];
int redInit[16];
int greenInit[16];



ball init = { 0, 0, 0, 0, 0, UNKNOWN_ID };
vector<ball> finalBalls;

CircleFinder::CircleFinder()
{
}

void clearColorHistogram()
{
	
	/*for(int i = 0; i < 3 ; i++)
	{
		for(int j = 0; j < 256 ; j++)
		{
			colorHistogram[i][j] = 0;
		}
	}
	*/
	for(int r = 0 ; r < bins ; r++)
	{
		for(int g = 0 ; g < bins ; g++)
		{
			for(int b = 0 ; b < bins ; b++)
			{
				colorHistogram[r][g][b] = 0;
			}
		}
	}
}

int getColorHistogram(int x, int y)
{
	clearColorHistogram();
	int minX = x - radius;
	if(minX < 0) minX = 0;
	int maxX = x + radius;
	if(maxX > xPixels) maxX = xPixels;
	int minY = y - radius;
	if(minY < 0) minY = 0;
	int maxY = y + radius;
	if(maxY > yPixels) maxY = yPixels;
	int thresh = 40;
	
	//white count, used to differ between solids and stripes
	int whiteCount = 0;
	int yellowCount = 0;
	
	for(int j = minY ; j < maxY; j++ )
	{
		for(int i = minX ; i < maxX ; i++ )
		{
			if( sqrt(( i - x)*(i - x) + (j - y )*(j - y)) < radius )
			{
				//need to put it in correct bin which is just the rgb int divided by binsPerColor
				
				uchar red = data[step*j + 3*i];	
				uchar green = data[step*j + 3*i + 1];	
				uchar blue = data[step*j + 3*i + 2];
				/*
				if(!(fabs(r - g) < thresh && fabs(r-b) < thresh && fabs(b-g) < thresh))
				{
					count++;	
					colorHistogram[0][r]++;
					colorHistogram[1][g]++;
					colorHistogram[2][b]++;
				}
				*/
				
				
				int r = red/binsPerColor;	
				int g = green/binsPerColor;		
				int b = blue/binsPerColor;
				colorHistogram[r][g][b]++;
				
				int maxDiff = abs(red - green);
				if(abs(green - blue) > maxDiff) maxDiff = abs(green - blue);
				if(abs(red - blue) > maxDiff) maxDiff = abs(red - blue);
				
				//red and blue are backwards, we're retarded
				if(blue > 230 && green > 230)
				{
				 	if( red > 100) 
				 		whiteCount++;
				 	else	
				 		yellowCount++;
				}
				
			}
		}
	}
	
	printf("yellowCount = %d, whiteCount = %d\n", yellowCount, whiteCount);
	
	//had problems with the yellow ball vs the threshHold for white so we figure it out here
	if(yellowCount > 800)
	{
		if(whiteCount > 400)
			whiteCount = 800;
		else
			whiteCount = 10;
	}
	return whiteCount;
}

int getMarkerScore(int x, int y)
{
	clearColorHistogram();
	int minX = x - Mradius;
	int maxX = x + Mradius;
	int minY = y - Mradius;
	int maxY = y + Mradius;
	
	
	
	int score = 0;
	int scale = 1;
	for(int j = minY ; j < maxY; j++ )
	{
		for(int i = minX ; i < maxX ; i++ )
		{
			int temp = sqrt(( i - x)*(i - x) + (j - y )*(j - y));
			if( temp < Mradius )
			{
				temp = Mradius - temp;
				score += temp*temp*data[step*j + 3*i + 1]/200;						
			}
		}
	}
	//printf("yes = %d\n", score);
	return score;
}

ball getMarker(int minX, int minY, int maxX, int maxY)
{
	int finalX = minX;
	int finalY = minY;
	int score = 0;
	for(int i = minX ; i < maxX ; i++)
	{
		for(int j = minY ; j < maxY ; j++)
		{
			int temp = getMarkerScore(i, j);
			if(temp > score)
			{
				finalX = i;
				finalY = j;
				score = temp;
			}
		}
	}
	//printf("marker at ( %d , %d )\n", finalX, finalY);
	ball temp;
	temp.x = finalX;
	temp.y = finalY;
	temp.rad = Mradius;
	return temp;
}

void getColorHistogram(int timeThrough)
{
	printf("final ball size = %d\n", finalBalls.size());
	//reorder the balls for reference picture
	multiset<ball, ballYLT> wholeSet;
	for(int j = 0; j < finalBalls.size(); j++){
		wholeSet.insert(finalBalls[j]);
	}
	
	set<ball, ballYLT>::iterator wholeSetIter = wholeSet.begin();
	printf("wholeSet is size %d\n", wholeSet.size());
	vector<ball> outputVector;
	
	//for all the balls
	while(wholeSetIter != wholeSet.end()){
		multiset<ball, ballXLT> ySet;
		
		double lastY = wholeSetIter->y;
		
		//push one row into the new set
		while(wholeSetIter != wholeSet.end() && fabs(wholeSetIter->y - lastY) < 50){
			ySet.insert(*wholeSetIter);
			wholeSetIter++;
		}
		
		//insert balls into output vector in order
		set<ball, ballYLT>::iterator newSetIter = ySet.begin();
		while(newSetIter != ySet.end()){
			outputVector.push_back(*newSetIter);
			newSetIter++;
		}
		
		printf("outputVector size %d\n", outputVector.size());
	}
	
	printf("output vector size %d\n", outputVector.size());
	int subtract = 0;
	for( int i = 0; i < outputVector.size() /*+ 1*/ ; i++)
	{
		//skip 4, 6, 12, and 14 balls
		if(false/*i == 6 || i == 14*/)
		{
			clearColorHistogram();
			subtract++;
		}
		else
		{
			getColorHistogram(outputVector[i-subtract].x, outputVector[i-subtract].y);
			//getNumberOfColoredPixels(outputVector[i-subtract].x, outputVector[i-subtract].y);
		}
		/*
		for(int j = 0; j < 3 ; j++)
		{
			for(int k = 0; k < 256 ; k++)
			{	
				//if(avg)
				//{
				//if(setColorHistograms[timeThrough][i][j][k] < colorHistogram[j][k])
					setColorHistograms[timeThrough][i][j][k] = colorHistogram[j][k];
					//setColorHistograms[timeThrough][i][j][k] /= 2;
				//}	
				//else
				//{
				//	setColorHistograms[0][i][j][k] = colorHistogram[j][k];
				//}
			}
		}
	
	
		*/
		for(int r = 0 ; r < bins ; r++)
		{
			for(int g = 0 ; g < bins ; g++)
			{
				for(int b = 0 ; b < bins ; b++)
				{
					if(setColorHistograms[i][r][g][b] < colorHistogram[r][g][b])
						setColorHistograms[i][r][g][b] = colorHistogram[r][g][b];
				}
			}
		}
	
	}
	//printf("done with getting colorhistograms\n");
	
}

void initializeColorHistograms(int quad)
{
	string line;
	char buffer[30];
	sprintf(buffer, "colorHistograms-%d.txt", quad);
  	ifstream myfile (buffer);
  	if (myfile.is_open())
  	{
    for(int timeThrough = 0; timeThrough < 1 ; timeThrough++)
    {
		for(int ballNum = 0 ; ballNum < 16 ; ballNum++)
		{
			/*
			for(int i = 0; i < 3 ; i++)
			{
				for(int j = 0; j < 256 ; j++)
				{
					getline(myfile,line);
					setColorHistograms[timeThrough][ballNum][i][j] = atoi(line.c_str());
				}
			}
			*/
			
			for(int r = 0 ; r < bins ; r++)
			{
				for(int g = 0 ; g < bins ; g++)
				{
					for(int b = 0 ; b < bins ; b++)
					{
						getline(myfile,line);
						setColorHistograms[ballNum][r][g][b] = atoi(line.c_str());
					}
				}
			}
		}
	}
	myfile.close();
	}
	else
	{
		printf("Could not open file to setColorHistograms for quad %d\n", quad);
	}
	
}

void writeColorHistogramsToFile(bool avg = false, int quad = 1)
{
	int i;
	int j;
	
	ofstream setfile ("colorHistograms-old-lowerRes.txt");
  	if (setfile.is_open())
  	{

	int ballNum;
	int r;
	int g;
	int b;
	for(int timeThrough = 0; timeThrough < 1 ; timeThrough++)
	{
		/*for(ballNum = 0 ; ballNum < 16 ; ballNum++)
		{
		
			for(int i = 0; i < 3 ; i++)
			{
				for(int j = 0; j < 256 ; j++)
				{
					setfile << setColorHistograms[timeThrough][ballNum][i][j] << "\n";
				}
			}
		}
		*/
		for(ballNum = 0; ballNum < 16 ; ballNum++)
		{
			//printf("writing for ballNum %d\n", ballNum);
			for(int r = 0 ; r < bins ; r++)
			{
				for(int g = 0 ; g < bins ; g++)
				{
					for(int b = 0 ; b < bins ; b++)
					{
						setfile << setColorHistograms[ballNum][r][g][b] << "\n";
					}
				}
			}
		}
	}
	setfile.close();
	}
	else
	{
		printf("Couldn't save histograms to file\n");
	}
	
	
	//initializeColorHistograms();
	getColorHistogram(0);// 0 then 1 then 2twist
	
	char buffer[30];
	sprintf(buffer, "colorHistograms-%d.txt", quad);
	setfile.open(buffer);
  	if (setfile.is_open())
  	{

		int ballNum;
		int r;
		int g;
		int b;
		for(int timeThrough = 0; timeThrough < 1 ; timeThrough++)
		{
			/*for(ballNum = 0 ; ballNum < 16 ; ballNum++)
			{
		
				for(int i = 0; i < 3 ; i++)
				{
					for(int j = 0; j < 256 ; j++)
					{
						setfile << setColorHistograms[timeThrough][ballNum][i][j] << "\n";
					}
				}
			}
			*/
			for(ballNum = 0; ballNum < 16 ; ballNum++)
			{
				//printf("writing for ballNum %d\n", ballNum);
				for(int r = 0 ; r < bins ; r++)
				{
					for(int g = 0 ; g < bins ; g++)
					{
						for(int b = 0 ; b < bins ; b++)
						{
							setfile << setColorHistograms[ballNum][r][g][b] << "\n";
						}
					}
				}
			}
		}
		setfile.close();
	}
	else
	{
		printf("Couldn't save histograms to file\n");
	}
	//printf("done writing to file\n");
}

typedef struct{
	vector<int> histComparisons;
	int whiteCount;
} ballNumInfo_T;

ballNumInfo_T getTheBallNumber(int x, int y)
{
	ballNumInfo_T returnInfo;
	//vector<int> histComparisons;
	returnInfo.whiteCount = getColorHistogram(x,y);
	int closest = 0;
	int count;
	int closestCount;
	int i;
	for( i = 0 ; i < 16 ; i++ )
	{
		count = 0;
		if(false/*i == 6 || i == 14*/) 
		{
			returnInfo.histComparisons.push_back(count);
			continue;
		}
		
		int maxColor = 0;
		int minColor = 0;
		
		for(int r = 0 ; r < bins ; r++)
		{
			for(int g = 0 ; g < bins ; g++)
			{
				for(int b = 0 ; b < bins ; b++)
				{
					//maxColor = max( r, max(b,g));
					//minColor = min( r, min(b,g));
					
					//if(maxColor - minColor > 2){
						//intersection
						if(colorHistogram[r][g][b] > setColorHistograms[i][r][g][b])
							count += setColorHistograms[i][r][g][b];
						else 
							count += colorHistogram[r][g][b];
					//}
				}
			}
		}
			
		returnInfo.histComparisons.push_back(count);
		//printf("the count was %d\n", count);
		if(i == 0 )
			closestCount = count;
		else// < for euclid, > for intersection, < for chi squared, > for BHATTACHARYYA
		{	
			if(count > closestCount)
			{
				closestCount = count;
				closest = i;
			}
		}	
		//}
	}

	//return closest;
	
	return returnInfo;
}

///////////////////////////////////////////////////
// Finds the markers in the image
// Returns...
///////////////////////////////////////////////////
void CircleFinder::findMarkers(IplImage* img, int quad, markerStruct &markerInfo){
	
	int m0X;
	int m0Y;
	int m1X;
	int m1Y;
	int m2X;
	int m2Y;
	int m3X;
	int m3Y;
	int m4X;
	int m4Y;
	int markerWidth = 4*Mradius;
	int markerHeight = 4*Mradius;
	
	/*switch(quad)
	{
		case 1: m0X = 69;
				m0Y = 1063;
				m1X = 68;
				m1Y = 726;
				m2X = 493;
				m2Y = 297;
				m3X = 824;
				m3Y = 302;
				m4X = 1152;
				m4Y = 306;
				break;
				
		case 2: m0X = 1440;
				m0Y = 1021;
				m1X = 1437;
				m1Y = 698;
				m2X = 1026;
				m2Y = 285;
				m3X = 703;
				m3Y = 285;
				m4X = 374;
				m4Y = 285;
				break;
				
		case 3: m0X = 138;
				m0Y = 984;
				m1X = 136;
				m1Y = 654;
				m2X = 542;
				m2Y = 232;
				m3X = 872;
				m3Y = 232;
				m4X = 1208;
				m4Y = 230;
				break;
		//quad 4
		default:m0X = 1454;
				m0Y = 868;
				m1X = 1464;
				m1Y = 536;
				m2X = 1043;
				m2Y = 121;
				m3X = 706;
				m3Y = 100;
				m4X = 370;
				m4Y = 100;
				break;
	}*/
	
	//for 960x720
	switch(quad)
	{
		case 1: m0X = 38;
				m0Y = 635;
				m1X = 38;
				m1Y = 433;
				m2X = 291;
				m2Y = 174;
				m3X = 492;
				m3Y = 176;
				m4X = 690;
				m4Y = 177;
				break;
				
		case 2: m0X = 870;
				m0Y = 600;
				m1X = 870;
				m1Y = 405;
				m2X = 616;
				m2Y = 156;
				m3X = 421;
				m3Y = 159;
				m4X = 222;
				m4Y = 160;
				break;
				
		case 3: m0X = 74;
				m0Y = 601;
				m1X = 72;
				m1Y = 402;
				m2X = 318;
				m2Y = 146;
				m3X = 516;
				m3Y = 147;
				m4X = 715;
				m4Y = 146;
				break;
		//quad 4
		default:m0X = 876;
				m0Y = 527;
				m1X = 879;
				m1Y = 330;
				m2X = 627;
				m2Y = 70;
				m3X = 424;
				m3Y = 72;
				m4X = 221;
				m4Y = 73;
				break;
	}
	
	//for 960x720
	/*m0X = (int)(m0X*.6);
	m0Y = (int)(m0Y*.6);
	m1X = (int)(m1X*.6);
	m1Y = (int)(m1Y*.6);
	m2X = (int)(m2X*.6);
	m2Y = (int)(m2Y*.6);
	m3X = (int)(m3X*.6);
	m3Y = (int)(m3Y*.6);
	m4X = (int)(m4X*.6);
	m4Y = (int)(m4Y*.6);*/
	
	m0X -= markerWidth/2;
	m0Y -= markerHeight/2;
	m1X -= markerWidth/2;
	m1Y -= markerHeight/2;
	m2X -= markerWidth/2;
	m2Y -= markerHeight/2;
	m3X -= markerWidth/2;
	m3Y -= markerHeight/2;
	m4X -= markerWidth/2;
	m4Y -= markerHeight/2;

	
	markerInfo.markers.push_back(getMarker(m0X, m0Y, m0X + markerWidth, m0Y + markerHeight));
	markerInfo.markers.push_back(getMarker(m1X, m1Y, m1X + markerWidth, m1Y + markerHeight));
	markerInfo.markers.push_back(getMarker(m2X, m2Y, m2X + markerWidth, m2Y + markerHeight));
	markerInfo.markers.push_back(getMarker(m3X, m3Y, m3X + markerWidth, m3Y + markerHeight));
	markerInfo.markers.push_back(getMarker(m4X, m4Y, m4X + markerWidth, m4Y + markerHeight));
	
	char buffer[50];
   
    CvRect roi;
    roi.x = m0X;
    roi.y = m0Y;
    roi.width = markerWidth;
    roi.height = markerHeight;
    cvSetImageROI( img, roi );
    sprintf(buffer, "marker-%d-quad-%d.jpg", 0, quad);
    cvSaveImage(buffer, img);
    cvResetImageROI(img);
   
    roi.x = m1X;
    roi.y = m1Y;
    roi.width = markerWidth;
    roi.height = markerHeight;
    cvSetImageROI( img, roi );
    sprintf(buffer, "marker-%d-quad-%d.jpg", 1, quad);
    cvSaveImage(buffer, img);
    cvResetImageROI(img);
   
    roi.x = m2X;
    roi.y = m2Y;
    roi.width = markerWidth;
    roi.height = markerHeight;
    cvSetImageROI( img, roi );
    sprintf(buffer, "marker-%d-quad-%d.jpg", 2, quad);
    cvSaveImage(buffer, img);
    cvResetImageROI(img);
   
    roi.x = m3X;
    roi.y = m3Y;
    roi.width = markerWidth;
    roi.height = markerHeight;
    cvSetImageROI( img, roi );
    sprintf(buffer, "marker-%d-quad-%d.jpg", 3, quad);
    cvSaveImage(buffer, img);
    cvResetImageROI(img);
   
    roi.x = m4X;
    roi.y = m4Y;
    roi.width = markerWidth;
    roi.height = markerHeight;
    cvSetImageROI( img, roi );
    sprintf(buffer, "marker-%d-quad-%d.jpg", 4, quad);
    cvSaveImage(buffer, img);
    cvResetImageROI(img);

	/*if(topMarkers.size() != 3){
		printf("Found Incorrect Number of Top Markers!!! (%d)\n", topMarkers.size());
	}

	if( sideMarkers.size() != 2 ) {
		printf("found Incorrect number of side markers!!! (%d)\n", sideMarkers.size());
	}*/
	
    
    //FIND INTERSECTING POINT
    float bottom = markerInfo.markers[2].x - markerInfo.markers[4].x;
    //if(bottom == 0) bottom += .00001;
    markerInfo.slope1 = (markerInfo.markers[2].y - markerInfo.markers[4].y)
                  /bottom;
    float intercept1 = markerInfo.markers[2].y - (markerInfo.slope1*markerInfo.markers[2].x); 
    
    float slope2 = 0;
    float intercept2 = 0;
    markerInfo.refX = 0;
    markerInfo.refY = 0;
    
    if(markerInfo.markers[0].x == markerInfo.markers[1].x)
    {
    	markerInfo.refX = markerInfo.markers[0].x;//markerInfo.slope1*markerInfo.markers[0].x + intercept1;
    	markerInfo.refY = markerInfo.slope1*markerInfo.refX + intercept1;
    }
    else
    {
    	float bottom1 = markerInfo.markers[1].x - markerInfo.markers[0].x;
    	if(bottom1 == 0) bottom1 += .00001;
    	slope2 = (markerInfo.markers[1].y - markerInfo.markers[0].y)
    	         /bottom1;
    	intercept2 = markerInfo.markers[1].y - (slope2*markerInfo.markers[1].x); 
    	markerInfo.refX = (intercept2 - intercept1)/(markerInfo.slope1 - slope2);
    	markerInfo.refY = markerInfo.slope1*markerInfo.refX + intercept1;
    	printf("angle of marker lines is %f\n", (180*atan((slope2 - markerInfo.slope1)/(1 + markerInfo.slope1*slope2)))/M_PI);
    }
    
    //get the scale of mm/pixel
    float distance = 24.5*25.4; //inches* (mm/inch)
    markerInfo.scale = distance/sqrt((markerInfo.markers[1].x - markerInfo.markers[3].x) *
                                     (markerInfo.markers[1].x - markerInfo.markers[3].x) + 
                                     (markerInfo.markers[1].y - markerInfo.markers[3].y) *
                                     (markerInfo.markers[1].y - markerInfo.markers[3].y)); 
}

///////////////////////////////////////////////////
// Finds the balls in the image
///////////////////////////////////////////////////
void CircleFinder::findBallsOneImage(IplImage* img, int quad, markerStruct &markerInfo, vector<ball> &theBalls){

    CvMemStorage* storage = cvCreateMemStorage(0);
	initializeColorHistograms(quad);

	//change here to use the red channel
	//cvSmooth( gray, gray, CV_BILATERAL, 5, 5, 9, 9);
	//cvSmooth( img, img, CV_BILATERAL, 7, 7, 20, 20);

	//just for testing (seeing what the canny looks like)
	double thresh1 = param1;
    double thresh2 = param1/2;
    double aperature_size = 3;
    IplImage* dst = cvCreateImage( cvGetSize(img), 8, 1 );
    cvCanny( img, dst, thresh1, thresh2, aperature_size );
    cvSaveImage("canny.jpg", dst);
    cvReleaseImage(&dst);
   
	int maxX = xPixels;
	int minX = markerInfo.markers[0].x + markerToEdge;
	int minY = markerInfo.markers[2].y + markerToEdge;
	int maxY = yPixels;
	if( quad == 2 || quad == 4 )
	{
		maxX = minX - 2*markerToEdge;
		minX = 0;
	}	
   
    CvRect roi;
    roi.x = minX;
    roi.y = minY;
    roi.width = maxX - minX;
    roi.height = maxY - minY;
    cvSetImageROI( img, roi );
    
	CvSeq* circles = cvHoughCircles(img, storage, CV_HOUGH_GRADIENT, dp, 
	                                minDistance, param1, param2, minRadius,maxRadius );

    ballCount = circles->total;
    for( int i = 0; i < ballCount ; i++ )
    {
		float* p = (float*)cvGetSeqElem( circles, i );
		ball tempBall;
		tempBall.x = p[0] + minX;
		tempBall.y = p[1] + minY;
		tempBall.rad = p[2];
		tempBall.quad = quad;
		
		theBalls.push_back(tempBall);
	}
	
	cvResetImageROI(img);
	
	printf("balls found: %d\n", theBalls.size());

	float markerToBumper = (2 + (15/16.0))*25.4;
	//get actual positions
	if(quad%2 == 1)
	{
		for( int i = 0 ; i < theBalls.size() ; i++ )
		{
			float slope = (theBalls[i].y - markerInfo.refY)/(theBalls[i].x - markerInfo.refX);
			float distance = markerInfo.scale*sqrt((markerInfo.refX - theBalls[i].x)
			                                       *(markerInfo.refX - theBalls[i].x) +
			                                        (markerInfo.refY - theBalls[i].y)*
			                                        (markerInfo.refY - theBalls[i].y));
			                                        
			float angle = atan((markerInfo.slope1 - slope)/(1 + markerInfo.slope1*slope));
			theBalls[i].y_wrt_picture = (distance*cos(angle) - markerToBumper)/1000;
			theBalls[i].x_wrt_picture = -(distance*sin(angle) + markerToBumper)/1000;
		}
	}
	else
	{
		
		for( int i = 0 ; i < theBalls.size() ; i++ )
		{
			float slope = (theBalls[i].y - markerInfo.refY)/(theBalls[i].x - markerInfo.refX);
			float distance = markerInfo.scale*sqrt((markerInfo.refX - theBalls[i].x) *
			                                       (markerInfo.refX - theBalls[i].x) + 
			                                       (markerInfo.refY - theBalls[i].y) *
			                                       (markerInfo.refY - theBalls[i].y));
			float angle = atan((markerInfo.slope1 - slope)/(1 + markerInfo.slope1*slope));
			theBalls[i].y_wrt_picture = (distance*cos(angle) - markerToBumper)/1000;
			theBalls[i].x_wrt_picture = (distance*sin(angle) - markerToBumper)/1000;
		}
		for( int i = 0 ; i < theBalls.size() ; i++ )
		{
			theBalls[i].y_wrt_picture = (fullTableY/1000) - theBalls[i].y_wrt_picture;
		}	
	}
	
	if(quad == 2 || quad == 1)
	{	
		for( int j = 0 ; j < theBalls.size() ; j++ )
		{
			theBalls[j].x_wrt_picture = (fullTableX/1000) - theBalls[j].x_wrt_picture;
			theBalls[j].y_wrt_picture = (fullTableY/1000) - theBalls[j].y_wrt_picture;
		}	
	}
	
	vector<ball> tempBalls;
	
	for(int j = 0; j < theBalls.size(); j++)
	{
		if(quad == 1 || quad == 2)
		{
			if(theBalls[j].x_wrt_picture > (fullTableX/1000.0)/2 - radiusMM)
				tempBalls.push_back(theBalls[j]);
		}
		else
		{
			if(theBalls[j].x_wrt_picture < (fullTableX/1000.0)/2 + radiusMM)
				tempBalls.push_back(theBalls[j]);
		}
	}
	
	theBalls.clear();
	
	for(int j = 0; j < tempBalls.size(); j++)
	{
		theBalls.push_back(tempBalls[j]);
	}

    cvReleaseMemStorage(&storage);
}

///////////////////////////////////////////////////////
// filters balls from two vectors from the same picture
///////////////////////////////////////////////////////
void CircleFinder::filterBalls(vector<ball> &output, vector<ball> &v1, vector<ball> &v2, vector<ball> &v3, vector<ball> &v4)
{
	for(int i = 0; i < v1.size(); i++)
	{
		output.push_back(v1[i]);
	}
	
	for(int i = 0; i < v2.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x - v2[i].x) < radius &&
			   fabs(output[j].y - v2[i].y) < radius)
			{
				output[j].x += v2[i].x;
				output[j].x /= 2;
				
				output[j].y += v2[i].y;
				output[j].y /= 2;
				
				output[j].x_wrt_picture += v2[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v2[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v2[i]);
		}
	}
	
	for(int i = 0; i < v3.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x - v3[i].x) < radius &&
			   fabs(output[j].y - v3[i].y) < radius)
			{
				output[j].x += v3[i].x;
				output[j].x /= 2;
				
				output[j].y += v3[i].y;
				output[j].y /= 2;
				
				output[j].x_wrt_picture += v3[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v3[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v3[i]);
		}
	}
	
	for(int i = 0; i < v4.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x - v4[i].x) < radius &&
			   fabs(output[j].y - v4[i].y) < radius)
			{
				output[j].x += v4[i].x;
				output[j].x /= 2;
				
				output[j].y += v4[i].y;
				output[j].y /= 2;
				
				output[j].x_wrt_picture += v4[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v4[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v4[i]);
		}
	}
}




//*********************************************
// Finds all the balls
//*********************************************
void CircleFinder::findBalls(char* fileName, int quad)
{
	IplImage* img;
    if( (img=cvLoadImage(fileName, 1)) != 0)
    {   
    
    	//separate image into separate channels
    	//green looks good for finding the markers
    	//red looks good for finding the 7 ball
    	IplImage *red, *green, *blue, *gray;
    	red = cvCreateImage( cvGetSize(img), 8, 1 );
    	green = cvCreateImage( cvGetSize(img), 8, 1 );
    	blue = cvCreateImage( cvGetSize(img), 8, 1 );
    	gray = cvCreateImage( cvGetSize(img), 8, 1 );
        cvCvtColor( img, gray, CV_BGR2GRAY ); 
    	cvCvtPixToPlane(img, blue, green, red, 0);
    	/*cvSaveImage("red.jpg", red);
    	cvSaveImage("green.jpg", green);
    	cvSaveImage("blue.jpg", blue);*/
    	cvSmooth( red, red, CV_BILATERAL, 7, 7, 20, 20);
    	cvSmooth( green, green, CV_BILATERAL, 7, 7, 20, 20);
    	cvSmooth( blue, blue, CV_BILATERAL, 7, 7, 20, 20);
    	cvSmooth( gray, gray, CV_BILATERAL, 7, 7, 20, 20);
    
    
        height     = img->height;
        width      = img->width;
        step       = img->widthStep/sizeof(uchar); 
        channels   = img->nChannels;
        data       = (uchar *)img->imageData;
        

		//find markers
		markerStruct markerInfo;
		markerInfo.storage = cvCreateMemStorage(0);
        findMarkers(green, quad, markerInfo);
        
        //draw reference point
    	cvCircle( img, cvPoint(cvRound(markerInfo.refX),cvRound(markerInfo.refY)), 
    	          3, CV_RGB(0,0,255), -1, 8, 0 );

        //draw markers on image
        for( int j = 0 ; j < markerInfo.markers.size() ; j++ )
    	{  
    	   cvCircle( img, cvPoint(cvRound(markerInfo.markers[j].x),
    	                          cvRound(markerInfo.markers[j].y)), 
    	                          3, CV_RGB(0,255,0), -1, 8, 0 );
    	   cvCircle( img, cvPoint(cvRound(markerInfo.markers[j].x),
    	                          cvRound(markerInfo.markers[j].y)),
    	                          cvRound(Mradius), CV_RGB(255,0,0), 3, 8, 0 );
    	}
		
		//FIND BALLS
		vector<ball> theRedBalls;
		findBallsOneImage(red, quad, markerInfo, theRedBalls);
		
		for(int i = 0; i < theRedBalls.size(); i++){
			cvCircle( red, cvPoint(cvRound(theRedBalls[i].x),cvRound(theRedBalls[i].y)), 3, 
			          CV_RGB(0,255,0), -1, 8, 0 );
		}
		
		vector<ball> theGreenBalls;
		findBallsOneImage(green, quad, markerInfo, theGreenBalls);
		
		for(int i = 0; i < theGreenBalls.size(); i++){
			cvCircle( green, cvPoint(cvRound(theGreenBalls[i].x),cvRound(theGreenBalls[i].y)), 3, 
			          CV_RGB(0,255,0), -1, 8, 0 );
		}
		
		vector<ball> theBlueBalls;
		findBallsOneImage(blue, quad, markerInfo, theBlueBalls);
		
		for(int i = 0; i < theBlueBalls.size(); i++){
			cvCircle( blue, cvPoint(cvRound(theBlueBalls[i].x),cvRound(theBlueBalls[i].y)), 3, 
			          CV_RGB(0,255,0), -1, 8, 0 );
		}
		
		vector<ball> theGrayBalls;
		findBallsOneImage(gray, quad, markerInfo, theGrayBalls);
		
		for(int i = 0; i < theGrayBalls.size(); i++){
			cvCircle( gray, cvPoint(cvRound(theGrayBalls[i].x),cvRound(theGrayBalls[i].y)), 3, 
			          CV_RGB(0,255,0), -1, 8, 0 );
		}
		
		filterBalls(finalBalls, theRedBalls, theGreenBalls, theBlueBalls, theGrayBalls);
		
		for( int i = 0 ; i < finalBalls.size() ; i++ )
		{
			ballNumInfo_T returnInfo = getTheBallNumber((int)finalBalls[i].x, (int)finalBalls[i].y);
			finalBalls[i].histogramComparison = returnInfo.histComparisons;
			finalBalls[i].whiteCount = returnInfo.whiteCount;
			/*int pixelCount = getNumberOfColoredPixels((int)finalBalls[i].x, (int)finalBalls[i].y);
			if(pixelCount < 3000 && pixelCount > 1200 && finalBalls[i].number != CUE)
				finalBalls[i].number = (BallType)(8 + (int)finalBalls[i].number);
			*/
			printf("ball %d  position( %f , %f ) pixels( %f , %f)\n",finalBalls[i].number, finalBalls[i].x_wrt_picture, finalBalls[i].y_wrt_picture, finalBalls[i].x, finalBalls[i].y);
			//printf("ball %d  position( %f , %f )\n", balls[i].number , balls[i].x, balls[i].y);
		}
		
		//if(quad == 4)
		//	writeColorHistogramsToFile(true, quad);
		
		for(int i = 0; i < finalBalls.size(); i++){
			cvCircle( img, cvPoint(cvRound(finalBalls[i].x),cvRound(finalBalls[i].y)), 3, 
			          CV_RGB(0,255,0), -1, 8, 0 );
			cvCircle( img, cvPoint(cvRound(finalBalls[i].x),cvRound(finalBalls[i].y)), 
			          cvRound(radius), CV_RGB(0,255,0), 3, 8, 0 );
		}
		
		char buffer[50];
		sprintf(buffer, "findCenter-%d.jpg", quad);
		cvSaveImage(buffer, img);
		
	    sprintf(buffer, "blue-%d.jpg", quad);
	    cvSaveImage(buffer, blue);
	    
	    sprintf(buffer, "green-%d.jpg", quad);
	    cvSaveImage(buffer, green);
	    
	    sprintf(buffer, "red-%d.jpg", quad);
	    cvSaveImage(buffer, red);
	    
	    sprintf(buffer, "gray-%d.jpg", quad);
	    cvSaveImage(buffer, gray);
	    
	    
	    //release the images so they do not take up memory
	    cvReleaseImage(&gray);
	    cvReleaseImage(&img);
	    //cvReleaseImage(&dst);
	    cvReleaseImage(&red);
	    cvReleaseImage(&green);
	    cvReleaseImage(&blue);
    }
    else
        printf("failed to open picture: %s\n", fileName);
}

void CircleFinder::putTableTogether(vector<ball> &v1, vector<ball> &v2, vector<ball> &v3, vector<ball> &v4, vector<ball> &output, vector<ball> &prevState)
{

	double samePosThresh = .03;
	for(int i = 0; i < v1.size(); i++)
	{
		output.push_back(v1[i]);
	}
	
	for(int i = 0; i < v2.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x_wrt_picture - v2[i].x_wrt_picture) < samePosThresh &&
			   fabs(output[j].y_wrt_picture - v2[i].y_wrt_picture) < samePosThresh)
			{	
				output[j].x_wrt_picture += v2[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v2[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v2[i]);
		}
	}
	
	for(int i = 0; i < v3.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x_wrt_picture - v3[i].x_wrt_picture) < samePosThresh &&
			   fabs(output[j].y_wrt_picture - v3[i].y_wrt_picture) < samePosThresh)
			{	
				output[j].x_wrt_picture += v3[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v3[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v3[i]);
		}
	}
	
	for(int i = 0; i < v4.size(); i++)
	{

		bool foundSame = false;
		for(int j = 0; j < output.size(); j++)
		{
			if(fabs(output[j].x_wrt_picture - v4[i].x_wrt_picture) < samePosThresh &&
			   fabs(output[j].y_wrt_picture - v4[i].y_wrt_picture) < samePosThresh)
			{	
				output[j].x_wrt_picture += v4[i].x_wrt_picture;
				output[j].x_wrt_picture /= 2;
				
				output[j].y_wrt_picture += v4[i].y_wrt_picture;
				output[j].y_wrt_picture /= 2;
				
				foundSame = true;
			}
		}
		
		if(!foundSame)
		{
			output.push_back(v4[i]);
		}
	}
	
	printf("found %d balls\n", output.size());
	for( int i = 0 ; i < output.size() ; i++ )
	{
		output[i].number = UNKNOWN_ID;
	}
	bool numberCanBeUsed[16]; //assumes no ball can be added since last state
	
	numberCanBeUsed[0] = true; //cue should always be possible
	
	//if its the first state then all balls are possible
	if(prevState.size() == 0)
	{
		for(int i = 0 ; i < 16 ; i++)
		{
			numberCanBeUsed[i] = true;
		}
	}
	else
	{
		for(int i = 1 ; i < 16 ; i++)
		{
			numberCanBeUsed[i] = false;
		}
	}

	//looks for balls that haven't moved
	float thresh = .01;
	int ballsLeft = output.size();
	for( int i = 0 ; i < prevState.size() ; i++ )	
	{
		numberCanBeUsed[prevState[i].number] = true; //this number is now possible
		for(int j = 0 ; j < output.size() ; j++ )
		{
			//should probably look for "similar" color too
			if(fabs(prevState[i].x_wrt_picture - output[j].x_wrt_picture) < thresh &&
			   fabs(prevState[i].y_wrt_picture - output[j].y_wrt_picture) < thresh)
			{
			   output[j].number = prevState[i].number;
			   numberCanBeUsed[prevState[i].number] = false;
			   ballsLeft--;
			   break;
		    }
		}
	}
	
	//looks at different histogram comparison "scores" and eliminates possibilities by which numbers are left
	
	int maxWhiteCount = 0;
	int maxWhiteCountBall;
	for(int i = 0; i < output.size(); i++)
	{
		if(output[i].whiteCount > maxWhiteCount && output[i].number == UNKNOWN_ID){
			maxWhiteCount = output[i].whiteCount;
			maxWhiteCountBall = i;
		}
	}
	
	
	if(maxWhiteCount > 1500){
		numberCanBeUsed[0] = false;
		output[maxWhiteCountBall].number = CUE;
		ballsLeft--;
	}
	else
	{
		numberCanBeUsed[0] = false;
	}
	
	for(int k = 0 ; k < ballsLeft ; k++)
	{	
		int maxBall = 0;
		int num = 0;
		for( int i = 0 ; i < output.size() ; i++ )
		{
			if(output[i].number == UNKNOWN_ID)
			{
				int maxCount = 0;
				int whiteCount = output[i].whiteCount;
				int solidThresh = 250;
				for( int j = 0 ; j < output[i].histogramComparison.size() ; j++ )
				{	
					if(maxCount <= output[i].histogramComparison[j])
					{ 
						if(numberCanBeUsed[j] && ((whiteCount > solidThresh && (j == 0 || j > 8)) ||
						                          (whiteCount < solidThresh && j > 0 && j < 9 )))
						{
							maxCount = output[i].histogramComparison[j];
							num = j;
							maxBall = i;
						}
					
					}
				}
			}
		}
			
		BallType type = CUE;
		switch(num)
	  	{
		 	case 1 : 	type = ONE;
			      		break;
			case 2 : 	type = TWO;
						break;
			case 3 : 	type = THREE;
						break;
			case 4 : 	type = FOUR;
						break;
			case 5 : 	type = FIVE;
						break;
			case 6 : 	type = SIX;
						break;
			case 7 : 	type = SEVEN;
						break;
			case 8 : 	type = EIGHT;
						break;
			case 9 : 	type = NINE;
						break;
			case 10 : 	type = TEN;
						break;
			case 11: 	type = ELEVEN;
						break;
			case 12 : 	type = TWELVE;
						break;
			case 13 : 	type = THIRTEEN;
						break;
			case 14 : 	type = FOURTEEN;
						break;
			case 15 : 	type = FIFTEEN;
			      		break;
		 
			default :	type = CUE;
	    } 
	   
		output[maxBall].number = type;
		numberCanBeUsed[num] = false;
		
	}
	
	//print for debugging purposes only
	for( int i = 0 ; i < output.size() ; i++ )
	{
		printf("Looking for match from ball at ( %f , %f )\n", output[i].x_wrt_picture, output[i].y_wrt_picture);
		printf("chose ball %d\nwhiteCount: %d\n", (int)output[i].number, output[i].whiteCount);
		//for(int j = 0 ; j < output[i].histogramComparison.size() ; j++)
		//{
		//	printf("match for ball %d is %d\n", j, output[i].histogramComparison[j]);
		//}
	}
}

//*********************************************
// Gets the position of all balls
// [ (0,0) being top left corner (in mm) ]
//*********************************************
//void CircleFinder::getLocations(string pic1, string pic2, string pic3, string pic4, int quadrant, vector<ball> &result, vector<ball> &prevState)
void* CircleFinder::getLocations(void* input)
{
	circleInputInfo* theInfo = (circleInputInfo*) input;
	printf("circlefinder received: %s %d\n", theInfo->picName.c_str(), theInfo->quad);
    argument = 2;
    finalBalls.clear();
    
    findBalls((char*)theInfo->picName.c_str(), theInfo->quad);
    
    for(int i = 0 ; i < finalBalls.size() ; i++)
    {
    	theInfo->result.push_back(finalBalls[i]);
    }
    
    /*finalBalls.clear();
    findBalls((char*)pic2.c_str(), 2);
    addBallsToTable(result, finalBalls);
    
    finalBalls.clear();
    findBalls((char*)pic3.c_str(), 3);
    addBallsToTable(result, finalBalls);
    
    finalBalls.clear();
    findBalls((char*)pic4.c_str(), 4);
    addBallsToTable(result, finalBalls);
    
   	findColors(result, prevState);
    
    printf("complete finding balls in quadrant %d\n", quadrant);
	*/
}


