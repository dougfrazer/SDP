//#include "../opencv-1.1.0/cv/include/cv.h"
//#include "../opencv-1.1.0/otherlibs/highgui/highgui.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <jpeglib.h>
#include <stdlib.h>

//for mitch
//g++ -I/usr/local/include/opencv -L/usr/local/lib -lcv -lcvaux -lcxcore -lhighgui -lstdc++ -Wall -g -o circleFinder circleFinder.cpp

uchar* data;
uchar* grayData;
int height;
int width;
int step;
int grayHeight;
int grayWidth;
int grayStep;
float radius;
float radVar;
int ballCount;
float fullTableX = 2133.6; //mm, 25.4(mm/inch) * 12 (inches/foot)*7 (foot table)
float fullTableY = 1066.8;
int xPixels = 1200;
int yPixels = 1600;
int ballsInFinal;

char* pic;
int argument;
int greyScale = 45;

//colors for the table
int bTable = 0;
int gTable = 0;
int rTable = 0;

const float MarkerDistance = 5;

int blueInit[16];
int redInit[16];
int greenInit[16];

struct ball
{
  float x;
  float y;
  float rad;
  int number;
};

//***************************************************
// sets the avg color of each ball 0-8 (complete)
//***************************************************
void getBallColorAverage(float x, float y, int number)
{

    int b = 0;
    int g = 0;
    int r = 0;
    int count = 0;

    
    int i;
    int j;
    int tempb = 0;
    int tempg = 0;
    int tempr = 0;
    
    for( i = (int)(x - (radius/2)) ; i < (int)(x + (radius/2)); i++ )
    {
        for( j = (int)(y - (radius/2)) ; j < (int)(y + (radius/2)); j++ )
        {
            if((((x - i)*(x - i) + (y - j)*(y - j)) < radius))
            {
                tempb = data[j*step + 3*i];
                tempg = data[j*step + 3*i + 1];
                tempr = data[j*step + 3*i + 2];
                if( (abs(tempb-tempg) < greyScale) && (abs(tempb-tempr) < greyScale) && (abs(tempr-tempg) < greyScale))
                {   
                    b += tempb;
                    g += tempg;
                    r += tempr; 
                    count++;
                }
            }
        }
    }
    
    blueInit[number] = (b/count);
    greenInit[number] = (g/count);
    redInit[number] = (r/count);
}

//*********************************************
// Sets the average color of the Table
//*********************************************
void initTableColor(int x, int y, int w, int h)
{
    int i;
    int j;
    int r = 0;
    int g = 0;
    int b = 0;
    for( i = y ; i < (y+h) ; i++ )
    {
        for( j = x ; j < (x+w) ; j++)
        {
            b += data[i*step + 3*j];
            g += data[i*step + 3*j + 1];
            r += data[i*step + 3*j + 2];
        }
    }

    bTable = b/(w*h);
    gTable = g/(w*h);
    rTable = r/(w*h);
}



//**********************************************
// initialize balls (input them in order) and 
// set rectangle to set table color
//**********************************************
void initializer(ball* allBall)
{   
    printf("here");
    int i;
    for( i = 0; i < 9 ; i++ )
    {
        getBallColorAverage( allBall[i].x , allBall[i].y, i);
    }
    
    //set box for Table Color
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    
    initTableColor(x, y, width, height);
}


//*********************************************
// Finds which ball has the closest color ( 0 - 8 )
//*********************************************
int getCorrelation(int b, int r, int g)
{
    double total = b + r + g;
    double b1 = ((double)b)/total;
    double r1 = ((double)r)/total;
    double g1 = ((double)g)/total;
    
    double* correlation = new double[16];
    int i;
    for( i = 0 ; i < 9 ; i++)
    {   
        correlation[i] = abs(b1 - blueInit[i]);
        correlation[i] += abs(r1 - redInit[i]);
        correlation[i] += abs(g1 - greenInit[i]);
    }
    
    total = correlation[0];
    int result =  0;
    
    for( i = 1 ; i < 9; i++)
    {
        if(total > correlation[i])
        {
            total = correlation[i];
            result = i;
        }
    }
    
    return result;
}


//*********************************************
// Gets the Number of the Ball ( 0 - 15 )
//*********************************************
int getBallNumber(int x, int y)
{
    int b = 0;
    int g = 0;
    int r = 0;
    int count = 0;

    
    int i;
    int j;
    int tempb = 0;
    int tempg = 0;
    int tempr = 0;
    
    for( i = (int)(x - (radius/2)) ; i < (int)(x + (radius/2)); i++ )
    {
        for( j = (int)(y - (radius/2)) ; j < (int)(y + (radius/2)); j++ )
        {
            if((((x - i)*(x - i) + (y - j)*(y - j)) < radius))
            {
                tempb = data[j*step + 3*i];
                tempg = data[j*step + 3*i + 1];
                tempr = data[j*step + 3*i + 2];
                if((abs(tempb-tempg) < greyScale) && (abs(tempb-tempr) < greyScale) && (abs(tempr-tempg) < greyScale))
                {   
                    b += tempb;
                    g += tempg;
                    r += tempr; 
                    count++;
                }
            }
        }
    }
    
    int isStriped = 0;
    if(count < 1200)
    {
        isStriped = 1;
    }
 
    return getCorrelation(b, r, g) + 8*isStriped; 
}



//*********************************************
// Counts the pixels within ball with center ( x , y )
// which are the color of the table
//*********************************************
int countTable(float x, float y)
{
    int b = 0;
    int g = 0;
    int r = 0;
    int tableCount = 0;
    int difference = 0;
    
    int i;
    int j;
    for( i = (int)(x - (radius/2)) ; i < (int)(x + (radius/2)); i++ )
    {
        for( j = (int)(y - (radius/2)) ; j < (int)(y + (radius/2)); j++ )
        {
            if(((x - i)*(x - i) + (y - j)*(y - j)) < radius)
            {
                b = data[j*step + 3*i];
                g = data[j*step + 3*i + 1];
                r = data[j*step + 3*i + 2];
                //printf("at %d, %d r %d g %d b %d\n",i, j, r, g, b);
                
                difference = abs(b-bTable) + abs(g-gTable) + abs(r-rTable);
                printf("difference: %d\n", difference);
                if(difference < 100)
                    tableCount++;
            }
        }
    }
    
    return tableCount;
}


//*********************************************
// Picks the actual balls out of the larger set
//*********************************************
ball* pickBalls(int totalBalls, ball allBall[])
{
    int count = 0;
    int i;
    int j;
    
    //eliminates by radius
    for( i = 0 ; i < totalBalls ; i++ )
    {
        if(allBall[i].rad >= (radius - radVar) && allBall[i].rad <= (radius + radVar))
        {
            allBall[count].x = allBall[i].x;
            allBall[count].y = allBall[i].y;
            allBall[count].rad = allBall[i].rad;
            count++;
        }
    } 
    
    //get avg of really close balls (within 2 times the square of the radius Variance allowed)
    for( i = 0 ; i < count ; i++)
    {
        float matches = 1;
        float finalX = allBall[i].x;
        float finalY = allBall[i].y;
        float finalRAD = allBall[i].rad;
        for( j = i + 1 ; j < count ; j++ )
        {
            if(allBall[i].rad != 0 && allBall[j].rad != 0)
            {
                    float tempX = allBall[i].x - allBall[j].x;
                    float tempY = allBall[i].y - allBall[j].y;
                    if(sqrt(tempX*tempX + tempY*tempY) < 2*radVar*radVar)
                    {
                        finalX += allBall[j].x;
                        finalY += allBall[j].y;
                        finalRAD += allBall[j].rad;
                        matches++;
                        allBall[j].rad = 0;
                    }
            }
        }
        
        allBall[i].x = finalX/matches;
        allBall[i].y = finalY/matches;
        allBall[i].rad = finalRAD/matches;
    }

    //reduces size of list
    ballCount = 0;
    for( i = 0 ; i < count ; i++)
    {
        if(allBall[i].rad != 0)
        {
            allBall[ballCount].x = allBall[i].x;
            allBall[ballCount].y = allBall[i].y;
            allBall[ballCount].rad = allBall[i].rad;
            ballCount++;
        }
    }

    
    //check for semicircles (incomplete)
    for( i = 0 ; i < ballCount ; i++)
    {   
        int var = countTable(allBall[i].x, allBall[i].y);
        if(allBall[i].rad != 0)
        {
            int matches = 0;
            int j;
            for( j = 0 ; j < ballCount ; j++)
            {   
                float tempX = allBall[i].x - allBall[j].x;
                float tempY = allBall[i].y - allBall[j].y;
                if(sqrt(tempX*tempX + tempY*tempY) < (1.5*((float)radius)))
                {
                    matches++;
                }    
            }
            
            //for balls between two balls
            if(matches > 2)
            {
                //allBall[i].rad = 0;
            }
            else if(matches == 2)//for one ball
            {
               for( j = 0 ; j < ballCount ; j++)
                {   
                    float tempX = allBall[i].x - allBall[j].x;
                    float tempY = allBall[i].y - allBall[j].y;
                    if(sqrt(tempX*tempX + tempY*tempY) < (1.5*((float)radius)) && j != i)
                    {
                        //choose between the two (countTable doesn't work as of now)
                        /*if(countTable(allBall[i].x, allBall[i].y) > countTable(allBall[j].x, allBall[j].y))
                            allBall[i].rad = 0;
                        else
                            allBall[j].rad = 0;*/
                    }    
                }
            }
        }
    }
    
    count = 0;
    for( i = 0 ; i < ballCount ; i++)
    {
        if(allBall[i].rad != 0)
            count++;
    }
    
    ball* final[count];

    //eliminates table circles and compacts the list
    count = 0;
    for( i = 0 ; i < ballCount ; i++)
    {
        if(countTable(allBall[i].x, allBall[i].y) > 10)
            allBall[i].rad = 0;
        if(allBall[i].rad != 0)
        {
            allBall[count].x = allBall[i].x;
            allBall[count].y = allBall[i].y;
            allBall[count].rad = allBall[i].rad;
            count++;
        }
    }
    
    ballCount = count;
    
    return allBall;
}


//*********************************************
// Finds a Side Marker Position
//*********************************************
ball findMarker(int x, int y, IplImage* gray)
{
    int searchWidth = x + 50;
    int searchHeight = y + 50;

    //find the dot
    int finalx = x;
    int finaly = y;
    
    int i;
    for( i = x ; i < searchHeight ; i++ )
    {
        int j;
        for ( j = y ; j < (searchWidth - 1); j++ )
        {
            if((grayData[i*grayStep + j] + grayData[i*grayStep + j + 1]) > (grayData[finaly*grayStep + finalx] + grayData[finaly*grayStep + finalx + 1]))
            {
                finalx = j;
                finaly = i;
            }
        }
    }

    ball result;
    result.x = finalx;
    result.y = finaly;
    result.rad = 0;
    return  result;
}


//*********************************************
// Finds all the balls
//*********************************************
ball* findBalls(int quad)
{
    //printf("got in ok\n");
    IplImage* img;
    if( argument == 2 && (img=cvLoadImage(pic, 1))!= 0)
    {   
        //printf("got into find balls\n");
        height     = img->height;
        width      = img->width;
        step       = img->widthStep/sizeof(uchar); //some extra information at end of width for each row so use this to scan rows
        data       = (uchar *)img->imageData;

        //example: this would print out tprintf("%f, %f, %f\n", blue, green, red);he last color BGR
        //printf("%d %d %d \n", data[(height-1)*step +3*width -3], data[(height-1)*step +3*width -2], data[(height-1)*step +3*width -1]);

        IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );
        CvMemStorage* storage = cvCreateMemStorage(0);
        cvCvtColor( img, gray, CV_BGR2GRAY ); 
        grayData   = (uchar *)gray->imageData;
        grayHeight     = gray->height;
        grayWidth      = gray->width;
        grayStep       = gray->widthStep/sizeof(uchar);

        int minRadius = 23;
        int maxRadius = 35;
        radius = 26.5;
        radVar = 2;
        CvSeq* circles = cvHoughCircles( 
			gray, 			//image
			storage, 		//circle storage
			CV_HOUGH_GRADIENT, 	//method
			1, 			//dp
			1,  			//min distance 
			40, //50			//param1 lower means search harder
			17, //17			//param2 higher means requires more of an arc to find circle 
			minRadius,
			maxRadius );
			
        //init();
	    ball* allBalls = new ball[circles->total];
        int i;
        for( i = 0; i < circles->total ; i++ )
        {
             float* p = (float*)cvGetSeqElem( circles, i );
             allBalls[i].x = p[0];
             allBalls[i].y = p[1];
             allBalls[i].rad = p[2];
             
        }
        ball* finalBalls = pickBalls(i, allBalls);
        
        printf("finalBallCount is : %d\n", ballCount );
        initializer(finalBalls);
        
        int j;        
        for( j = 0 ; j < ballCount ; j++ )
        {   
            printf("( %f , %f )\n", finalBalls[j].x, finalBalls[j].y);
            cvCircle( img, cvPoint(cvRound(finalBalls[j].x),cvRound(finalBalls[j].y)), 3, CV_RGB(0,255,0), -1, 8, 0 );
            cvCircle( img, cvPoint(cvRound(finalBalls[j].x),cvRound(finalBalls[j].y)), cvRound(finalBalls[j].rad), CV_RGB(255,0,0), 3, 8, 0 );
            printf("Ball is number: %d\n", getBallNumber(finalBalls[j].x, finalBalls[j].y));
        }
        
        
    /****
    find markers and find actual positions
    ***/

        //front xy refers to position, second x#,y# refers to top or side respectively
        int xMarkerx1 = 990;
        int yMarkerx1 = 0;
        int xMarkerx2 = 390;
        int yMarkerx2 = 0;
        int xMarkery1 = 30;
        int yMarkery1 = 390;
        int xMarkery2 = 30;
        int yMarkery2 = 390;
        
        switch ( quad ) 
        {

            case 2 :    xMarkerx1 = xPixels - xMarkerx1;
                        xMarkerx2 = xPixels - xMarkerx2;
                        xMarkery1 = xPixels - xMarkery1;
                        xMarkery2 = xPixels - xMarkery2;
                        break;
                        
            case 3 :    xMarkerx1 = xPixels - xMarkerx1;
                        xMarkerx2 = xPixels - xMarkerx2;
                        xMarkery1 = xPixels - xMarkery1;
                        xMarkery2 = xPixels - xMarkery2;
                        yMarkerx1 = xPixels - yMarkerx1;
                        yMarkerx2 = xPixels - yMarkerx2;
                        yMarkery1 = xPixels - yMarkery1;
                        yMarkery2 = xPixels - yMarkery2;
                        break; 
                        
            case 4 :    yMarkerx1 = xPixels - yMarkerx1;
                        yMarkerx2 = xPixels - yMarkerx2;
                        yMarkery1 = xPixels - yMarkery1;
                        yMarkery2 = xPixels - yMarkery2;
                        break;
                  
            default:    break;
        }

        
        ball markerx1 = findMarker(xMarkerx1, xMarkery1, gray);
        ball markerx2 = findMarker(xMarkerx2, xMarkery2, gray);
        ball markery1 = findMarker(yMarkerx1, yMarkery1, gray);
        ball markery2 = findMarker(yMarkerx2, yMarkery2, gray);
        
        //****************************
        //for testing purposes********
        //****************************
        
        markerx1.x = 428;
        markerx2.x = 1006;
        markery1.y = 403;
        markery2.y = 695;
        
        
        //scale is mm/pixel
        //takes avg, but maybe should just take X because it's twice as far or maybe have one for x and one for y?
        float scale = ((fullTableX/(4*abs(markerx1.x-markerx2.x))) + (fullTableY/(4*abs(markery1.y-markery2.y))))/2;
                       
        float x0 = ((markerx1.x + markerx2.x)/2); //middle of qudrant
        float y0 = markery1.y; //both in pixels
        
        cvCircle( img, cvPoint(cvRound(x0),cvRound(y0)), 3, CV_RGB(0,0,255), -1, 8, 0 );
        
        //finds position in quadrant with center at 1/4 x of the table and 1/4 y of the table
        //printf("middle of quadrant: (%f , %f) scale: %f\n", x0, y0, scale);
        for( j = 0 ; j < ballCount ; j++ )
        {   
            allBalls[j].x -= x0;
            allBalls[j].y -= y0;
            
            //printf("pixels relative to middle( %f , %f )\n", allBalls[j].x, allBalls[j].y);
            
            allBalls[j].x = allBalls[j].x*scale;
            allBalls[j].y = allBalls[j].y*scale;    
        }
        
        
        float xShift = 0;
        float yShift = 0;
        
        switch( quad )
        {  
            case 2: xShift = fullTableX/4;
                    yShift = fullTableY/4;
                    break;
                    
            case 3: xShift = fullTableX/4;
                    yShift = 3*fullTableY/4;
                    break;
                    
            case 4: xShift = 3*fullTableX/4;
                    yShift = 3*fullTableY/4;
                    break;
                    
            default:
                    xShift = 3*fullTableX/4;
                    yShift = fullTableY/4;
        }
        
        //printf("shift: ( %f , %f )\n", xShift, yShift);
        for( j = 0 ; j < ballCount ; j++ )
        {
            //printf("before shift( %f , %f )\n", allBalls[j].x, allBalls[j].y);
            allBalls[j].x += xShift;
            allBalls[j].y += yShift;
            //printf("after shift( %f , %f )\n", allBalls[j].x, allBalls[j].y);
        }
        
        initTableColor(250, 750, 926, 312);
        printf("r %d, g %d, b %d\n", rTable, gTable, bTable);
       
        cvNamedWindow( "circles", 1 );
        cvShowImage( "circles", img );
	    cvSaveImage("circles2.jpg", img);
	    cvSaveImage("findCenter.jpg", img);
	    
	    return allBalls;
    }
    ball* failBall;
    
    return failBall;
}


//*********************************************
// Joins the lists of balls for each Quadrant
//*********************************************
ball* joinQuadrants(ball* q1, ball* q2, ball* q3, ball* q4)
{
    ball final[16];
    
    int i;
    int size;
    int j;
    int count;
    ballsInFinal = 0;
    size = 16;
    for(i = 0 ; i < size ; i++)
    {
        if(q1[i].rad == 0)
            break;
        count = 0;
        for(j = 0 ; j < ballsInFinal ; j++)
        {
            if(final[j].number == q1[i].number)
                count++;
        }
        if(count == 0)
        {
            final[ballsInFinal] = q1[i];
            ballsInFinal++;
        }   
    }
    
    for(i = 0 ; i < size ; i++)
    {
        if(q2[i].rad == 0)
            break;
        count = 0;
        for(j = 0 ; j < ballsInFinal ; j++)
        {
            if(final[j].number == q2[i].number)
                count++;
        }
        if(count == 0)
        {
            final[ballsInFinal] = q2[i];
            ballsInFinal++;
        }   
    }
    
    for(i = 0 ; i < size ; i++)
    {
        if(q3[i].rad == 0)
            break;
        count = 0;
        for(j = 0 ; j < ballsInFinal ; j++)
        {
            if(final[j].number == q3[i].number)
                count++;
        }
        if(count == 0)
        {
            final[ballsInFinal] = q3[i];
            ballsInFinal++;
        }   
    }
    
    for(i = 0 ; i < size ; i++)
    {
        if(q4[i].rad == 0)
            break;
        count = 0;
        for(j = 0 ; j < ballsInFinal ; j++)
        {
            if(final[j].number == q4[i].number)
                count++;
        }
        if(count == 0)
        {
            final[ballsInFinal] = q4[i];
            ballsInFinal++;
        }   
    }
    
    return final;
}

//*********************************************
// wait for motion
//*********************************************
int waitForMotion()
{
    return 0;
}

//*********************************************
// waits for motion to stop
//*********************************************
int waitForIdle()
{
    return 0;
}


//*********************************************
// Gets the position of all balls
// [ (0,0) being top left corner (in mm) ]
//*********************************************
int main(int argc, char** argv)
{

    argument = 2;
    //while(true)
    //{
        //waitForIdle();
        pic = argv[1];
        ball* quad1 = findBalls(2);
        pic = argv[1];
        ball* quad2 = findBalls(2);
        pic = argv[1];
        ball* quad3 = findBalls(2);
        pic = argv[1];
        ball* quad4 = findBalls(2);
        
        int j;
        for( j = 1 ; j < 16 ; j++)
        {
            printf("( %f , %f ) \n", quad1[j].x, quad1[j].y);
            printf("( %f , %f ) \n", quad2[j].x, quad2[j].y);
            printf("( %f , %f ) \n", quad3[j].x, quad3[j].y);
            printf("( %f , %f ) \n", quad4[j].x, quad4[j].y);
        }
        //delete[] quad1;
        
        /*ball* finalBalls = joinQuadrants(quad1, quad2, quad3, quad4);
        //waitForMotion();
    //}
        int i;
        for( i = 0 ; i < 16 ; i++ )
        {
            printf("Ball %d at position ( %f , %f )\n", finalBalls[i].number, finalBalls[i].x , finalBalls[i].y);
        }*/
    return 0;
}



/*
    //swap for initialization reasons
    ball temp = allBalls[1];
    allBalls[1] = allBalls[7];
    allBalls[7] = temp;
    
    temp = allBalls[2];
    allBalls[2] = allBalls[13];
    allBalls[13] = temp;
    
    temp = allBalls[3];
    allBalls[3] = allBalls[8];
    allBalls[8] = temp;
    
    temp = allBalls[5];
    allBalls[5] = allBalls[14];
    allBalls[14] = temp;
    
    temp = allBalls[14];
    allBalls[14] = allBalls[6];
    allBalls[6] = temp;
    
    temp = allBalls[7];
    allBalls[7] = allBalls[14];
    allBalls[14] = temp;
    
    temp = allBalls[8];
    allBalls[8] = allBalls[13];
    allBalls[13] = temp;
    //end swap
    
*/






