/***** PROGRAM THAT READS ANY KEY HIT OF THE KEYBOARD*****/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

void *read_char(void* arg)
{
	char c;
	int fd;
	printf("Entering routine to read character.........\n");
	while(1) {
		c = getchar();
		fd = open("fifo", O_WRONLY | O_NONBLOCK);
		if(c == '\n')
			c = getchar();
		write(fd, &c, 1);
		if(isalnum(c)) {
			exit(1);
		}
		close(fd);
	}
}

int main()
{
	int i;
	pthread_t tid1;
	i = mkfifo("fifo", 0666);
	if(i < 0) {
		printf("Problems creating the fifo\n");
		if(errno == EEXIST) {
			printf("fifo already exists\n");
		}
		printf("errno is set as %d\n", errno);
	}
	//i = pthread_create(&tid1, NULL, read_char, NULL);
	
	printf("size of double: %d\n", sizeof(double));
	
	double c = 32;
	int fd;
	printf("Entering routine to read character.........\n");
	while(1) {
		//c = getchar();
		fd = open("fifo", O_WRONLY);
		//if(c == '\n')
		//	c = getchar();
		//write(fd, &c, 1);
		//if(isalnum(c)) {
		//	exit(1);
		//}
		
		write(fd, &c, sizeof(double));
		c++;
		if(c == 10) exit(1);
		sleep(2);
		
	}
	close(fd);
	
	if(i == 0) while(1);
	return 0;
}

