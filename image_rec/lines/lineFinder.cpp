#include <list>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include "linefinder.h"

using namespace std;


void findLines(const char* fileName, list<line_T> &theLines, int quad_num){
    IplImage* src;
    if( (src=cvLoadImage(fileName, 1))!= 0)
    {
    	IplImage* graySrc = cvCreateImage( cvGetSize(src), 8, 1);
    	cvCvtColor( src, graySrc, CV_BGR2GRAY ); 
        IplImage* dst = cvCreateImage( cvGetSize(graySrc), 8, 1 );
        IplImage* color_dst = cvCreateImage( cvGetSize(graySrc), 8, 3 );
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* lines = 0;
        int i;
        
        cvSmooth( dst, dst, CV_BILATERAL, 9, 9, 50, 50);
        double thresh1 = 100;
        double thresh2 = 150;
        double aperature_size = 3;
        cvCanny( graySrc, dst, thresh1, thresh2, aperature_size );
        cvSaveImage("canny.jpg", dst);
        cvCvtColor( dst, color_dst, CV_GRAY2BGR );
        CvRect roi;
        roi.x = 0;
        roi.y = 169;
        roi.width = 529;
        roi.height = 480 - 169;
        
        if(quad_num == 1 || quad_num == 3){
        roi.x = 117;
        roi.y = 169;
        roi.width = 640 - 117;
        roi.height = 480 - 169;
        
        
        }
        //CvPoint p1;
        //p1.x = 0;
        //p1.y = 0;
        //CvPoint p2;
        //p2.x = 50;
        //p2.y = 50;
        
        //cvRectangle(src, p1, p2, CV_RGB(255,0,0), 3, 8 );
        //cvSaveImage("rect.jpg", src);
        
        //return 0;
        
        //cvSetImageROI( dst, roi );
        //cvSetImageROI( src, roi );
        double rho = 3; //distance resolution in pixel-related units
        double theta = CV_PI / 180.0; //angle resolution in radians
        double threshold = 150; //return line if accumulator value is greater than threshold
        double min_distance = 100; //minimum line length, only used in probabilistic method
        double max_gap = 30; //max gap between segments lying on same line to treat them as one line, only used in probabilistic
        
        
#if 0
        lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, rho, theta, threshold, 0, 0 );

        for( i = 0; i < MIN(lines->total,100); i++ )
        {
            float* line = (float*)cvGetSeqElem(lines,i);
            float rho = line[0];
            float theta = line[1];
            CvPoint pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
            cvLine( dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
        }
#else
		//threshold = 50;
		
        lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, rho, theta, threshold, min_distance, max_gap );
        //printf("lines found: %d\n", lines->total);
        for( i = 0; i < lines->total; i++ )
        {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            //save the line
            line_T newLine;
            newLine.p1 = line[0];
            //newLine.p1.x += roi.x;
            //newLine.p1.y += roi.y;
            newLine.p2 = line[1];
            //newLine.p2.x += roi.x;
            //newLine.p2.y += roi.y;
            theLines.push_back(newLine);
            cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
            cvLine( src, line[0], line[1], CV_RGB(255,0,0), 2, 8 );
        }
#endif
        /*cvNamedWindow( "Source", 1 );
        cvShowImage( "Source", src );

        cvNamedWindow( "Hough", 1 );
        cvShowImage( "Hough", color_dst );*/
        
        //cvSaveImage("lines.jpg", dst);
        
        //cvResetImageROI(src);
        cvSaveImage("lines.jpg", src);
        
        cvReleaseImage(&graySrc);
        cvReleaseImage(&dst);
        cvReleaseImage(&color_dst);
        cvReleaseImage(&src);
        cvReleaseMemStorage(&storage);

        //cvWaitKey(0);
    }
}
