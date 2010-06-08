#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h>
#include "v4l2uvc.h"
#include "getPic.h"
#include "utils.h"


struct vdIn* videoInArray[4];

void closeCamera(int deviceNum){
	close_v4l2(videoInArray[deviceNum]);
	free(videoInArray[deviceNum]);
	videoInArray[deviceNum] = 0;
	//freeLut();
	//fclose("/dev/video0");
}

void initCamera(int deviceNum, int width, int height) {

	char deviceName[12] = {0};
	sprintf(deviceName, "/dev/video%d", deviceNum);
    //printf("%s\n", deviceName);
    char* videodevice = deviceName;
    //int width = 640;//1600;
    //int height = 480;//1200;
    int fps = 30;
    int format = (width == 1600 ? V4L2_PIX_FMT_YUYV:V4L2_PIX_FMT_MJPEG);
    int grabmethod = 1;
    char* avifilename = NULL;
    
    videoInArray[deviceNum] = (struct vdIn *) malloc( sizeof(struct vdIn) );
    struct vdIn* videoIn = videoInArray[deviceNum];
    
    //unsigned char* ptr = (unsigned char*) malloc(1600*1200*3*sizeof(char)); //try to malloc 5mb
    
    //if(!ptr) printf("malloc failed\n");
    
    if(init_videoIn(videoIn, (char *) videodevice, width, height, fps, format,
			 grabmethod, avifilename) < 0) {
			 
        printf("couldn't init\n");
        return;
	}
	
	
	
	initLut();
	
	int success = v4l2SetControl(videoIn, V4L2_CID_BRIGHTNESS, 202);
	if(success == -1) printf("could not set brightness\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_GAIN, 75);
	if(success == -1) printf("could not set gain\n");
	
	
	success = v4l2SetControl(videoIn, V4L2_CID_CONTRAST, 178);
	if(success == -1) printf("could not set contrast\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_SATURATION, 67);
	if(success == -1) printf("could not set saturation\n");
	
	
	//enum_controls(videoIn->fd);
	//save_controls(videoIn->fd);
	
	
	struct v4l2_control control;
	control.id    =V4L2_CID_EXPOSURE_AUTO;
	control.value =1;
	int value;
	if ((value = ioctl(videoIn->fd, VIDIOC_S_CTRL, &control)) < 0)
		printf("Set Auto Exposure on error\n");
	else
		printf("Auto Exposure set to %d\n", control.value);
	
	if ((value = v4l2SetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE, 333)) < 0)
		printf("Set Absolute Exposure up error\n");
		
	int exposure = v4l2GetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE);
	printf("exposure: %d\n", exposure);
	
	/*
	success = v4l2SetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE, 321); //set to manual
	if(success == -1) printf("could not set exposure\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_GAIN, 120); //set to manual
	if(success == -1) printf("could not set gain\n");
	*/
	
	
	
	//v4l2SetControl(videoIn, V4L2_CID_BRIGHTNESS, 70);
	//int exposure = v4l2GetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE);
	//printf("exposure %d\n\n", exposure);
	//enum_controls(videoIn);
	
	//printf("done init\n");
	return;
}


void getPicture(int deviceNum, char* fileName) {
	
	struct vdIn* videoIn = videoInArray[deviceNum];
	if(videoIn == 0){
		//printf("bad pointer\n");
		return;
	}
	//videoIn->rawFrameCapture = 1;
	//printf("starting grab\n");
	if(uvcGrab(videoIn) < 0) return;//printf("error grabbing\n");
	//printf("done grab\n");
	switch(videoIn->formatIn){
		case V4L2_PIX_FMT_MJPEG:
			get_picture(videoIn->tmpbuffer,videoIn->buf.bytesused, fileName);
			break;
		case V4L2_PIX_FMT_YUYV:
			get_pictureYV2(videoIn->framebuffer,videoIn->width,videoIn->height, fileName);
			break;
		default:
			break;
	}
	//get_pictureYV2(videoIn->framebuffer,videoIn->width,videoIn->height, fileName);
	//printf("done saving picture\n");
	
	return;
}

/*void * getPictureThread(void* deviceNum) {
	char deviceName[12] = {0};
	sprintf(deviceName, "/dev/video%d", (int)deviceNum);
    printf("%s\n", deviceName);
    char* videodevice = deviceName;
    int width = 640;//1600;
    int height = 480;//1200;
    int fps = 30;
    int format = V4L2_PIX_FMT_YUYV;
    int grabmethod = 1;
    char* avifilename = NULL;
    struct vdIn* videoIn;
    videoIn = (struct vdIn *) malloc( sizeof(struct vdIn) );
    
    //unsigned char* ptr = (unsigned char*) malloc(1600*1200*3*sizeof(char)); //try to malloc 5mb
    
    //if(!ptr) printf("malloc failed\n");
    
    if(init_videoIn(videoIn, (char *) videodevice, width, height, fps, format,
			 grabmethod, avifilename) < 0) {
			 
        printf("couldn't init\n");
        return 0;
	}
	
	initLut();
	int success = v4l2SetControl(videoIn, V4L2_CID_BRIGHTNESS, 180);
	if(success == -1) printf("could not set brightness\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_CONTRAST, 25);
	if(success == -1) printf("could not set contrast\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_SATURATION, 35);
	if(success == -1) printf("could not set contrast\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_EXPOSURE_AUTO, 0); //set to manual
	if(success == -1) printf("could not set manual exposure\n");
	
	success = v4l2SetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE, 300); //set to manual
	if(success == -1) printf("could not set exposure\n");
	
	//v4l2SetControl(videoIn, V4L2_CID_BRIGHTNESS, 70);
	//int exposure = v4l2GetControl(videoIn, V4L2_CID_EXPOSURE_ABSOLUTE);
	//printf("exposure %d\n\n", exposure);
	//enum_controls(videoIn);
	videoIn->rawFrameCapture = 1;
	uvcGrab(videoIn);
	if(uvcGrab(videoIn) < 0) printf("error grabbing\n");
	get_pictureYV2(videoIn->framebuffer,videoIn->width,videoIn->height, (int)deviceNum);
}

void getFourPictures(){
	pthread_t threads[4] = {0};
	
	int val = 0;
	val = pthread_create(&threads[0], NULL, getPictureThread, (void*) 0);
	val = pthread_create(&threads[1], NULL, getPictureThread, (void*) 1);
	val = pthread_create(&threads[2], NULL, getPictureThread, (void*) 2);
	val = pthread_create(&threads[3], NULL, getPictureThread, (void*) 3);
	
	void* status;
	val = pthread_join(threads[0], &status);
	val = pthread_join(threads[1], &status);	
	val = pthread_join(threads[2], &status);	
	val = pthread_join(threads[3], &status);
}*/
