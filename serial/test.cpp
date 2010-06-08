#include "serial.h"
#include<stdio.h>  
#include <fcntl.h>

//#include <time.h>

int main()
{
	char cmd = 'o';
	int value;
	
	//speed typically B115200 (for bluetooth), but testing over wired requires B38400
	Serial cue("/dev/ttyS0", B38400);
	if(!cue.Setup()) return 1; //setup serial, open port, if it doesn't work, quit with error
	
	printf("Connected Successfully!\n");
	
	int flags = fcntl(0, F_GETFL); /* get current file status flags */
	flags |= O_NONBLOCK;		/* turn off blocking flag */
	fcntl(0, F_SETFL, flags);		/* set up non-blocking read */	

	printf("Enter Command (h for help):");

	do	//menu loop(main program)
	{
		do	//terminal input loop
		{
			cmd = getchar();
			int type = cue.checkInput(&value);
			if(type == BUTTON){
				printf("button %d pressed\n", value);
			}
			if(type == BATTERY){
				printf("Battery at %d\n", value);
			}
			usleep(1);
		} while(cmd == '\n' || cmd == ' ');	
		
		switch(cmd)
		{
			case EOF:
			break;
			
			case 'e': //exit command
			break;
			
			case 'h': //help menu
				printf("\n\tAvailable Commands:\n");
				printf("\tb     - check Battery level\n");
				printf("\tl,r,s - Light Left/Right/Stop LED\n");
				printf("\to     - Turn Off all LEDs\n");
				printf("\th     - print this menu\n");
				printf("\te     - Exit program\n\n");
				printf("Enter Command (h for help):");
			break;			
			
			default: //regular commands (
				cue.Command(cmd);
				printf("Enter Command (h for help):");
			break;
		}
	} while(cmd != 'e');
	cue.Close();	//make sure to close the port
  return 0;
}
