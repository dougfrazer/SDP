#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <stdlib.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>

#define EMPTY_BUFFER 0
#define BATTERY 1
#define BUTTON 2

class Serial{
	public:
		//public functions
		Serial(const char*, speed_t); //Constructor

		~Serial(); //Destructor

		bool Setup(); //setup the port and open it, returns true if it worked, false if not.

		void Close(); //close the port
		
		int checkInput(int* value);
		//int getBattLvl();	//if the battery level is ready, return it. otherwise, return -1

		bool Command(char);
		
		void FlushQueue();	//Flush serial input/output queue
		
		int DataWaiting();	//returns the number of bits in the input buffer
		
		char const* path; //Path to device
		
	private:
		int fd; //File descriptor
		speed_t speed;
};

#endif
