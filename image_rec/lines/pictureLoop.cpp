#include"getPic.h"
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char** argv){

	int deviceNum = atoi(argv[1]);
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	char* fifoName = argv[4];
	char* fileName = argv[5];
	
	int fdIn;
	
	fdIn = open(fifoName, O_RDONLY | O_NONBLOCK);
	
	initCamera(deviceNum, width, height);
	
	char stop = 0;
	
	//while(!stop){
		getPicture(deviceNum, fileName);
		
		read(fdIn, &stop, 1);
	//}

}
