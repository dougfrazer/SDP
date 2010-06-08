#include <cv.h>
#include <highgui.h>
#include <stdio.h>


using namespace std;

int main(){

	CvCapture* theCapture = cvCreateCameraCapture( 0 );
	cvSetCaptureProperty(theCapture, CV_CAP_PROP_BRIGHTNESS, 180);
	cvSetCaptureProperty(theCapture, CV_CAP_PROP_CONTRAST, 25);
	cvSetCaptureProperty(theCapture, CV_CAP_PROP_SATURATION, 35);
	//cvSetCaptureProperty(theCapture, CV_CAP_PROP_GAIN, 100);
	//cvSetCaptureProperty(theCapture, CV_CAP_PROP_FORMAT, 1);
	cvSetCaptureProperty(theCapture, CV_CAP_PROP_FRAME_WIDTH, 1600);
	cvSetCaptureProperty(theCapture, CV_CAP_PROP_FRAME_HEIGHT, 1200);
	
		
	char name[15] = {0};
	
	for(int i = 0; i < 10; i++){
	
		IplImage* theFrame = cvQueryFrame( theCapture );
		sprintf(name, "frame%d.jpg", i);
		
		cvSaveImage( name, theFrame );
	}
	
	cvReleaseCapture( &theCapture );
	return 0;
}
