#include "serial.h"

Serial::Serial(const char* device, speed_t s){
	path = device;
	speed = s;
}

Serial::~Serial(){
	Close();
}

bool Serial::Setup(){
	fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd != -1) //Port opened correctly
	{
		fcntl(fd, F_SETFL, 0);

		struct termios options;
		tcgetattr(fd, &options);  //Get the current options for the port

		cfsetispeed(&options, speed);  //Set the in and out baud rates to 115200
		cfsetospeed(&options, speed);

		options.c_cflag |= (CLOCAL | CREAD);  //Enable the receiver and set local mode

		//setup 8N1 (8 bits, no parity, 1 stopbit)
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8;

		options.c_cflag &= ~CRTSCTS;	// Turn off hardware flow control
		
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //Raw Input - only enable this or canonical
		
		tcsetattr(fd, TCSANOW, &options);  //Set the new options for the port
		fcntl(fd, F_SETFL, FNDELAY);	//Non-blocking
	}
	else
	{ //Could not open the port.
		char error[50];
		sprintf(error, "Serial::Setup(): Unable to open %s", path);
		perror(error);
		return false;
	}
	return true;
}

void Serial::Close(){
	if (fd != -1) {
		close(fd);
	}
}

/*

int Serial::getBattLvl(){	//if the battery level is ready, return it. otherwise, return -1
	char buffer[7];
	char* lvlstr;
	char* bufptr;
	int nbytes = 0;
	int numleft = sizeof(buffer);

	if (fd != -1){
		char cmd = 'b';
		ioctl(fd, TCFLSH, 2);	//make sure an old battery check or button isn't left in there
		if(!write(fd, &cmd, 1))	//send the battery level command
			return 0;

		bufptr = buffer;	//set pointer to front of buffer (index 0)
	
		while (numleft > 0)
		{
			nbytes = read(fd, bufptr, numleft);	//read up to the number of bytes left, the amt read is stored in nbytes
			if(nbytes > 0){	//did we actually recieve anything? (read returns -1 if not... dumb, why not 0? would make too much sense)
			  	bufptr += nbytes;	//move the pointer to right after the last character read
				numleft -= nbytes; //subtract however many were read from the amount left
			}
			if (bufptr[-1] == '>') break;	//just in case we hit the end early
		}
	
		lvlstr = strtok(buffer, "<B>"); //remove delimiters
		return atoi(lvlstr);
	}
	else return 0;
}

*/

int Serial::checkInput(int* value){ //returns value based on what command is in the serial port, modifies value based on type
	char buffer[7];
	char* lvlstr;
	char* bufptr;
	int nbytes = 0;
	int numleft = sizeof(buffer);

	if (fd != -1){
		bufptr = buffer;	//set pointer to front of buffer (index 0)
		
		nbytes = read(fd, bufptr, numleft);	//read up to the number of bytes left, the amt read is stored in nbytes
		
		//do initial check to see if anything is in buffer
		if(nbytes > 0){	//did we actually recieve anything?
		  	bufptr += nbytes;	//move the pointer to right after the last character read
			numleft -= nbytes; //subtract however many were read from the amount left
		}
		else return EMPTY_BUFFER;
		
		//stuff in buffer, figure out what it is
		while (numleft > 0)
		{
			nbytes = read(fd, bufptr, numleft);	//read up to the number of bytes left, the amt read is stored in nbytes
			if(nbytes > 0){	//did we actually recieve anything?
			  	bufptr += nbytes;	//move the pointer to right after the last character read
				numleft -= nbytes; //subtract however many were read from the amount left
			}
			if (bufptr[-1] == '>') break;	//just in case we hit the end early
		}

		if(!strncmp(buffer, "<B", 2)){ //battery level in buffer
			lvlstr = strtok(buffer, "<B>");
			*value = atoi(lvlstr);
			//printf("Got Battery Level: %d\n", atoi(lvlstr));
			return BATTERY;
		}
		if(!strncmp(buffer, "<PBTN", 5)){ //battery level in buffer
			lvlstr = strtok(buffer, "<PBTN>");
			*value = atoi(lvlstr);
			return BUTTON;
		}
		return 0;
	}
	else return 0;
}

bool Serial::Command(char c){
	if (fd != -1){
		//Send the command as a single character
		if(write(fd, &c, 1))
			return true;
		else return false;
	}
	else return false;
}

void Serial::FlushQueue(){
	if (fd != -1)
	{
		ioctl(fd, TCFLSH, 2);
	}
}
