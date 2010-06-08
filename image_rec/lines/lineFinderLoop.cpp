#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "linefinder.h"
#include "getPic.h"
#include "cv.h"
#include "highgui.h"
#include "shotutil.h"
#include "math.h"
#include "circleFinder.h"

//in pixels of the picture for right now, later will need to be wrt picture (hopefully still in pixels)
CvPoint cue_location;
int cue_quadrant;
double phi;

char* imgNames[4] = {"first.jpg", "second.jpg", "third.jpg", "fourth.jpg"};
                        
IplImage* motionImage[4][2] = {0};

/**
* @function motionDetection(void)

* @brief Checks to see if anything on the table is moving anymore.

* @return Boolean value.
*/
bool motionDetection(void)
{
	int COLOR_THRESHOLD = 20;
	int PIXEL_THRESHOLD = 20;
	int pixel_count = 0;
	// column 0: new images
 	// column 1: old images

	for(int i=0;i<4;i++)
	{
		if(motionImage[i][1] != NULL)
			cvReleaseImage(&motionImage[i][1]);
		motionImage[i][1] = motionImage[i][0];
		motionImage[i][0] = NULL;
		//char motion_filename[50];
		//sprintf(motion_filename, "motion_detection_%d.pnm", i);
		//getPicture(i, imgNames[i]);
		motionImage[i][0] = cvLoadImage(imgNames[i], 1);
	}

	// Compare Images	
	for(int i=0;i<4;i++)
	{

		RgbImage rgb_new(motionImage[i][0]);
		if(motionImage[i][1] == NULL)
			continue;
		RgbImage rgb_old(motionImage[i][1]);
		int width = motionImage[i][0]->width;
		int height = motionImage[i][0]->height;
		int row = 0;
		int col = 0;
		
		if(i % 2 == 0){
			row = 150;
			width = 540;
		}
		else{
			row = 130;
			col = 100;
		}

		for(; row<height; row++)
		{
			for(; col<width; col++)
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
	
	printf("pixel count: %d\n", pixel_count);
	if(pixel_count > PIXEL_THRESHOLD) 
		return false;
	
	return true;
}

/**
* @function cueBallMotionDetection(double threshold)

* @params threshold Defines the percentage of the surrounding square to be green to trigger a true response.  Note that this adds in the corner values, so a threshold that is too small will always return true.

* @brief A basic check for motion detection.  Will look at the image with the cue-ball in it and check if there is a threshold amount of green pixels where the cueball should be.  If there is, we assume the cue-ball has moved and return true.  Otherwise, we return false.

* @return Boolean value.
*/
bool cueBallMotionDetection(double threshold)
{
	//int cue_quadrant = whichQuadrantIsTheCueIn();
	// Now that we know which quadrant the cueball is in, poll that camera.
	// This should be a low-resolution image.
	
	/*if(initState[cue_quadrant] != LOW_RES){
		initCamera(cue_quadrant, 640, 480);
		initState[cue_quadrant] = LOW_RES;
	}*/
	
	//getPicture(0, "cue_ball_detection.pnm");
	
	// Load the image (it was saved to the hard drive).
	IplImage* img = cvLoadImage(imgNames[cue_quadrant], 1);
	if(img == 0)
	{
		printf("Failed to load image for cue ball motion detection\n");
		return false;
	}
	
	// Check the area of the cue ball.
	int ballSize = 84*640.0/960;
	//printf("ballSize: %d\n", ballSize);
	int row = cue_location.x;
	int col = cue_location.y;
	//row = row*640.0/1600;
	//col = col*480.0/1200;
	
	//set area to search
	int rowMin = row-ballSize/2;
	if(rowMin < 0) rowMin = 0;
	int rowMax = row+ballSize/2;
	if(rowMax > img->height) rowMax = img->height;
	int colMin = col - ballSize/2;
	if(colMin < 0) colMin = 0;
	int colMax = col + ballSize/2;
	if(colMax > img->width) colMax = img->width;
	
	//printf("ball: %d, %d\nrow: %d, col: %d\nlength: %d\n", row, col, rowMin, colMin, ballSize);
	
	
	int green_count = 0;
	int white_count = 0;
	CvRect roi;
	roi.x = rowMin;
	roi.y = colMin;
	roi.width = ballSize;
	roi.height = ballSize;
	cvSetImageROI(img, roi);
	cvSaveImage("cue_ball.jpg", img);
	
	IplImage* cue_image = cvLoadImage("cue_ball.jpg");
	cvResetImageROI(img);
	// Create an RGB map of the image for ease of use.
	RgbImage rgb(cue_image);
	
	
	//printf("width: %d, height: %d\n", cue_image->width, cue_image->height);
	
	for(int j=0; j< cue_image->height; j++)
	{
		for(int l=0; l< cue_image->width; l++)
		{
			// Check all RGB values.  See if they are green enough.
			// half or less blue or red and more than one third green);
			//printf("RGB: (%d,%d,%d)\n", rgb[row][col].r, rgb[row][col].g, rgb[row][col].b);
			//instead of that, just check if they are not white.
			int maxDiff = fabs(rgb[j][l].r - rgb[j][l].b);
			if(fabs(rgb[j][l].r - rgb[j][l].g) > maxDiff) maxDiff = fabs(rgb[j][l].r - rgb[j][l].g);
			if(fabs(rgb[j][l].b - rgb[j][l].g) > maxDiff) maxDiff = fabs(rgb[j][l].b - rgb[j][l].g);
			
			
			if(rgb[j][l].r > 230 && rgb[j][l].b > 180 && rgb[j][l].g > 230)
				white_count++;
			
			//if(rgb[j][l].r < 255/2 && rgb[j][l].b < 255/2 && rgb[j][l].g > 255/3)
				//green_count++;
		}
	}
	
	cvReleaseImage(&img);
	cvReleaseImage(&cue_image);
	
	//printf("X: %d, Y: %d\tWhite: %d\n", row, col, white_count);
	
	threshold = .1;
	// Threshold will be 400 out of 1000 pixels.
	
	//printf("green count: %d\n", green_count);
	
	if(white_count > ballSize*ballSize*threshold)
		return false;
	
	return true;

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
double findCueAngle()
{
	list<line_T> theLines;

#ifndef NO_CAMERAS
	//getPicture(cue_quadrant,imgNames[cue_quadrant]);
	//printf("got cue det pic\n");
	findLines(imgNames[cue_quadrant], theLines, cue_quadrant);
	double slope, angle, best_angle = 0, best_dist = 5000;
	
	double angleSum = 0;
	int count = 0;
	
	for(list<line_T>::iterator it = theLines.begin(); it != theLines.end(); it++)
	{
		// Swap points 1 and 2 so that p2 is always the one closest to the cue ball
		if( distance(&(it->p1), &cue_location) < distance(&(it->p2), &cue_location))
		{
			CvPoint temp = it->p1;
			it->p1 = it->p2;
			it->p2 = temp;
		}
		
		//printf("cue: %d, %d\np1: %d, %d\np2: %d, %d\n\n", cue_location.x, cue_location.y,
		//       it->p1.x, it->p1.y, it->p2.x, it->p2.y);
		
		double xDiff = it->p1.x - it->p2.x;
		double yDiff = it->p1.y - it->p2.y;
		if(yDiff == 0) yDiff += .0000000001;
		
		slope = yDiff/xDiff;
		angle = atan( slope );
		angle = angle*180 / M_PI;
		
		// TODO: account for angle difference between images
		//this should do it
		if(cue_quadrant == 0 || cue_quadrant == 3) angle -= 90;
		else angle += 90;
		
		//because we are using y/x??
		angle = 360 - angle;
		
		
		//find true angle, i.e. with respect to table
		if(it->p1.x < it->p2.x){
			angle += 180;
		}
		
		angle = fmod(angle, 360);
		
		//fizPoint ballPos = myState.getBall(CUE).getPos();
		//vec2 ballPoint(ballPos.x, ballPos.y);
		
		
		vec2 testPoint(cue_location.x, cue_location.y);
		vec2 p1(it->p1.x, it->p1.y);
		vec2 p2(it->p2.x, it->p2.y);
		double distance = pointToLineDistance(p1, p2, testPoint);// ballPoint);

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
		
		//printf("%f\n", distance);
		if(distance < 10 && distance >= 0 )//fabs(angle - phi) < 90)
		{
			angleSum += angle;
			count++;
		}
	}

	//return best_angle;
	if(count > 0)
		return angleSum / count;//theLines.size();
	else
		return -1;
#else
	return 45;
#endif
}

int main(int argc, char** argv)
{
	printf("in lineFinderLoop\n");
	
	int fdIn;
	int fdOut;
	int i;
	
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
	
	fdOut = open("fifoToUi", O_WRONLY);
	fdIn = open("fifoToLineFinder", O_RDONLY | O_NONBLOCK);
	
	fifoInfo_T setUpInfo;
	i = 0;
	
	printf("lineFinderLoop running\n");
	
	while(i < sizeof(fifoInfo_T)){
		int j = read(fdIn, &setUpInfo + i, sizeof(fifoInfo_T) - i);
		//printf("problem reading from fifo\n");
		
		while(j < 0){
			j = read(fdIn, &setUpInfo + i, sizeof(fifoInfo_T) - i);
		}
		
		printf("fifo read ok\n");
		
		i += j;
	}
	
	cue_location.x = setUpInfo.cue_x;// * (640.0/1600);
	cue_location.y = setUpInfo.cue_y;// * (480.0/1200);
	cue_quadrant = setUpInfo.cue_quad;
	phi = setUpInfo.phi;
	
	printf("read cue.x %d, cue.y %d, quad: %d\n", setUpInfo.cue_x, setUpInfo.cue_y, cue_quadrant);
	
	//set up 4 processes for streaming from the cameras
	/*i = mkfifo("cam1Fifo", 0666);
	i = mkfifo("cam2Fifo", 0666);
	i = mkfifo("cam3Fifo", 0666);
	i = mkfifo("cam4Fifo", 0666);
	
	
	system("./pictureLoop 0 640 480 cam1Fifo first.pnm &");
	system("./pictureLoop 1 640 480 cam2Fifo second.pnm &");
	system("./pictureLoop 2 640 480 cam3Fifo third.pnm &");
	system("./pictureLoop 3 640 480 cam4Fifo fourth.pnm &");
	
	int cam1Fifo, cam2Fifo, cam3Fifo, cam4Fifo;
	cam1Fifo = open("cam1Fifo", O_WRONLY);
	cam2Fifo = open("cam2Fifo", O_WRONLY);
	cam3Fifo = open("cam3Fifo", O_WRONLY);
	cam4Fifo = open("cam4Fifo", O_WRONLY);
	*/
	
	
	
	initCamera(0, 640, 480);//atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	initCamera(1, 640, 480);
	initCamera(2, 640, 480);
	initCamera(3, 640, 480);
	
	
	unsigned char stop = 0;
	int cue_moved_count = 0;
	int noMotionCount = 0;
	
	fifoReturn_T returnValues;
	
	state_E currentState = cue_angle_tracking;//cue_angle;
	
	while(!stop) {
		
		getPicture(0, imgNames[0]);
		getPicture(1, imgNames[1]);
		getPicture(2, imgNames[2]);
		getPicture(3, imgNames[3]);
			
		returnValues.cue_angle = -1;
		returnValues.cueMoved = 0;
		
		if(currentState == cue_angle_tracking){
			returnValues.cue_angle = findCueAngle();
			returnValues.cueMoved = cueBallMotionDetection(.2);
			if(returnValues.cueMoved)
				cue_moved_count++;
			else
				cue_moved_count = 0;
		}
		
		if(cue_moved_count > 8){
			currentState = motion_detection;
			//closeCamera(cue_quadrant);
		}
		
		
		if(currentState == motion_detection)
			returnValues.noMotion = motionDetection();
		else
			returnValues.noMotion = false;
		
		if(returnValues.noMotion){
			noMotionCount++;
		}
		else
			noMotionCount = 0;
		
		if(noMotionCount > 10){
			stop = 1;
			currentState = done_cue_tracking;
		}
		
		returnValues.currentState = currentState;
		printf("current state: %d\n", (int)currentState);
		
		if(currentState == done_cue_tracking) break;
		
		i = 0;
			
		int written = 0;
		while(written < sizeof(returnValues)){
		
			int j = write(fdOut, &returnValues + written, sizeof(returnValues) - written);
			if(j < 0){
				//printf("error %d\n", errno);
				//stop = 1;
				//break;
			}
			else{
				written += j;
			}
		}
		
		//fsync(fdOut);
    	
    	read(fdIn, &stop, 1);
    	//stop = 1;
	}
	
	
	closeCamera(0);
	closeCamera(1);
	closeCamera(2);
	closeCamera(3);
	
	if(currentState = done_cue_tracking)
		write(fdOut, &returnValues, sizeof(returnValues));
	/*char stopVal = 1;
	write(cam1Fifo, &stopVal, 1);
	write(cam2Fifo, &stopVal, 1);	
	write(cam3Fifo, &stopVal, 1);
	write(cam4Fifo, &stopVal, 1);*/


	close(fdIn);
	close(fdOut);
}
