
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "AI.h"
using namespace std;


/*  Communications stuff */
#define BUFFSIZE 128000
#define VERBOSE 1      			
char name[100];
char login[512];
bool echo = false;
bool visualize = false;
bool manualExit = false;
bool showScore = true;
char *loginEcho = "false";
int new_fd;	               			// new connection on new_fd           
struct sockaddr_in their_addr; 	// connector's address information
struct sockaddr_in my_addr;    	// my address information
struct hostent *he;
double buffer[BUFFSIZE];
char message[BUFFSIZE];
char serverAck[20];
//enum MessageType {}; // should implement enums here to clear up what the messages mean


/*  Gameplay stuff */
fizGWindow window(1200, 600, 0.1);
fizGShot animShot;
fizGTable table;
fizGTableState state(window, table), previousState(window, table), currentState(window, table), animState(window, table);
fizGCue myCue(window, table.getLength(), table.getWidth());
AI player;
double a, b, theta, phi, v;
double cue_x = 0;
double cue_y = 0;
double sigmaA = 0.0, sigmaB = 0.0, sigmaTheta = 0.0, sigmaPhi = 0.0, sigmaV = 0.0;


/* default parameters */
int tableNumber = 1;
int gameLength = 600;
int breakStrategy = 3;
int numberOfGames = 3;
int noiseType = 1;
int visMode = 0;
bool messaged_received = false;
bool onRobot = false;
bool ack_received = false;


/* Client communication functions */
void recvString( char[] , char );
void setState (char[] );
void sendString( char* );
void ack();

void sendString(char *str)
{
	if (send(new_fd, (char *) str, strlen(str), 0) == -1) perror("ERROR sending message");              
}

void ack()
{
	char *ackString = " ack~";
	sendString(ackString);
}

void mainLoop ()
{
	while (1) {
		char *jShot, *end, *bad_param, *error, *shotHeader,
		 *type, *targetPlayer, *solidsPlayer, *safetyString,
		 *initState, *params, *eventList, *finalState, *status, *turnTypeString,
		 *gameFlagsString, *endTypeString, *endString;
		
		bool bad_params[5];
		bool myShot, bad_x, bad_y, not_possible, not_behind_line, wasBBL;
		myShot = bad_x = bad_y = not_possible = not_behind_line = wasBBL = false;
			
		char msgcpy[BUFFSIZE];
		char chosenShot[1024];
		char saveName[256];
			
		int messageType, turnType, endType, gameFlags, wins, losses;
	
		recvString(message ,'/');

		sleep(1);		
		
		if (messaged_received) {
			
			ack();
			
			strcpy(msgcpy, message);
			type = strtok(msgcpy, " ");
			messageType = atoi(type);
	
			if (messageType == 0) {
				jShot = strtok(message, "&");
				turnTypeString = strtok(NULL, "&");
				gameFlagsString = strtok(NULL, "&");
			
				turnType = atoi(turnTypeString);
				gameFlags = atoi(gameFlagsString);
				player.setFlags(gameFlags);
				if ((gameFlags&1) == 1) {
					table.playingOnRobot(true);
					onRobot = true;
				}
			
				shotHeader = strtok(jShot, "*");
				initState = strtok(NULL, "*");
			
				char initStateCpy[10000];
				strcpy(initStateCpy, initState);
			
				params = strtok(NULL, "*");
				eventList = strtok(NULL, "*");
				finalState = strtok(NULL, "*");
			
				strtok(shotHeader, " ");
				targetPlayer = strtok(NULL, " ");
				solidsPlayer = strtok(NULL, " ");
			
				if (strcmp(targetPlayer, name) == 0) myShot = true;
				else myShot = false;
	
				setState(initState);
				previousState = state;
	
				if (turnType != 4) {
					a = atof(strtok(params, " "));
					b = atof(strtok(NULL, " "));
					theta = atof(strtok(NULL, " "));
					phi = atof(strtok(NULL, " "));
					v = atof(strtok(NULL, " "));
				}
	
				if (myShot) {
					if (strcmp(solidsPlayer, "null") == 0) player.isOpenTable = true;
					else {
						player.isOpenTable = false;	
						if (strcmp(solidsPlayer, name) == 0) player.onLowBalls = true;
						else player.onLowBalls = false;
					}
	
					player.isSafety = false;
	
					if (turnType != 4) {
						setState(finalState);
						currentState = state;
						player.previousState = previousState;
						player.currentState = currentState;
						player.table = table;
						state = previousState;
						if (turnType == 0) player.shoot();
						else if (turnType == 1) player.positionAndShoot(false);
						else if (turnType == 2) player.positionAndShoot(true);
					}
					else {
						player.currentState = previousState;
						player.breakshot();
					}
				} // end if myShot
				else player.echo();
			} // end if messageType == 0
			else if (messageType == 2) {
				
				myShot = true;
				strtok(message, " ");
				strtok(NULL, " ");
							
				bad_param = strtok(NULL, " ");
				for (int i = 0; i < 5; i++) {
					bad_param = strtok(NULL, " ");
					bad_params[i] = false;
					if (strcmp(bad_param,"true") == 0) bad_params[i] = true;
				}
			
				strtok(NULL, " ");
						
				error = strtok(NULL, " ");
				if (strcmp(error,"true") == 0) bad_x = true;
				
				error = strtok(NULL, " ");
				if (strcmp(error,"true") == 0) bad_y = true;
	
				strtok(NULL, " ");
				
				error = strtok(NULL, " ");
				if (strcmp(error,"true") == 0) not_behind_line = true;
				
				error = strtok(NULL, " ");
				if (strcmp(error,"true") == 0) wasBBL = true;
					
				if (bad_x || bad_y)	not_possible = true;
						
				if (not_possible || not_behind_line) player.repositionAndShoot(not_possible, not_behind_line, wasBBL);
				else player.reshoot(bad_params);
	
			} // end if messageType == 2
			else if (messageType == 3) {
				strtok(message, "!");
				endTypeString = strtok(NULL, "!");
				endString = strtok(NULL, "!");

				endType = atoi(endTypeString);

				myShot = false;

				if (!showScore) endString = strtok(endString,".");
				
				cout << endl << "Client: " << endString << endl;
				if (endType == 1) exit(0);
			}
			
			else myShot = false;
	
			if (player.isSafety) safetyString = "true";
			else safetyString = "false";
			
			if (myShot) {
				sprintf(chosenShot, " 1 %s %.16f %.16f %.16f %.16f %.16f %d %d %.16f %.16f %s/", name, player.a, player.b, 
						player.theta, player.phi, player.v, (int)player.ball, (int)player.pocket, 
						player.cue_x, player.cue_y, safetyString);
				sendString(chosenShot);
				
				if (VERBOSE) {
					cout << endl << "Client: " << name;
					if ( !player.isSafety && (int)player.ball != 0 ) {
						cout << " calls " << (int)player.ball << " ball in ";
						int calledPock = (int)player.pocket;
						string calledPockName = "";
						if (calledPock == 0) calledPockName = "SW pocket";
						else if (calledPock == 1) calledPockName = "W pocket";
						else if (calledPock == 2) calledPockName = "NW pocket";
						else if (calledPock == 3) calledPockName = "NE pocket";
						else if (calledPock == 4) calledPockName = "E pocket";
						else if (calledPock == 5) calledPockName = "SE pocket";
						else calledPockName = "UNKNOWN pocket";
						cout	<< calledPockName;
					}
					else if ( player.isSafety )	cout << " calls safety";
					else cout << " is breaking";
					cout << " {a= " << player.a << " b= " << player.b << " theta= " << player.theta << " phi= " << player.phi << " V= " << player.v << "}" << endl;
				}
				
				if (visualize) {
					if (VERBOSE) cout << endl << "Client: generating shot preview on shotViz...";
					animState = currentState;
					if (turnType == 4) {
						animState.rack8ball();
						animState.setBall(CUE, STATIONARY, fizPoint(player.cue_x, player.cue_y));
					}
					if (turnType == 1 || turnType == 2) {
						animState.setBall(CUE, STATIONARY, fizPoint(player.cue_x, player.cue_y));
					}
					ShotStatus result = animShot.execute(table, animState, player.a, player.b, player.theta, player.phi, player.v);
					
					if (result == 0) {
						system("rm -f shotViz_info.txt");
						system("rm -f shotViz_shot.txt");	
						ofstream info("shotViz_info.txt");
						info << "shotViz_shot.txt ";
						info << name;

						if ( player.onLowBalls && !player.isOpenTable ) info << " Solids ";
						else if ( player.isOpenTable ) info << " ? ";
						else info << " Stripes ";

						if ( player.isOpenTable ) info << "Yes ";
						else info << "No ";

						if ( player.isSafety )	info << "Yes ";
						else info << "No ";

						info << "                 ";
						info << player.ball << " " << player.pocket << endl;
						info.flush();
						info.close();
						
						bool savedOk = animShot.save("shotViz_shot.txt", false);
						if (savedOk) {
							if (manualExit) {
								if (onRobot) system("./shotViz -m -rb &");	
								else system("./shotViz -m &");		
							} else {
								if (onRobot) system("./shotViz -rb &");	
								else system("./shotViz &");		
							}
							if (VERBOSE) cout << "Done." << endl;					
						}
						else {
							if (VERBOSE) cout << "ERROR:  could not save shot result." << endl;
						}
					}
					else {
						if (VERBOSE) cout << "ERROR:  poolfiz returned error for shot result." << endl;
					}
				}			
			}
			messaged_received = false;
		}  // end if message_received
	} // end while
}
	
void setState(char* stateString)
{
	char **balls = new char*[16];
	balls[0] = strtok(stateString, "+");
	
	for (int j = 1; j < 16; j++) {		
		balls[j] = strtok(NULL, "+");
	}
	
	char *ballStateString, *ballPosString, *ballXString, *ballYString;
	int ballState, ballXInt, ballYInt;
	double ballX, ballY;

	for (int j = 0; j < 16; j++) {
		strtok(balls[j], "-");
		ballStateString = strtok(NULL, "-");
		ballState = atoi(ballStateString);
		ballPosString = strtok(NULL, "-");
		ballXString = strtok(ballPosString, " ");
		ballYString = strtok(NULL, " ");
		ballX = atof(ballXString);
		ballY = atof(ballYString);
		state.setBall((BallType)j, (BallState)ballState, fizPoint(ballX, ballY));
	}
}

void connect(int port, char* myhost)
{
	if ((he=gethostbyname(myhost))==NULL) perror("ERROR:  gethostbyname failed");

	if ((new_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) perror("ERROR:  invalid socket");

	their_addr.sin_family = AF_INET;        
	their_addr.sin_port = htons(port);      
	their_addr.sin_addr = *((struct in_addr *) he->h_addr); 
	bzero(&(their_addr.sin_zero), 8);        

	if (connect(new_fd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("ERROR:  could not connect");
		exit(1);
	}
}

void recvString(char* str, char terminal)
{
	int numbytes = 0;
	int end = 0;
	int i, j;
	char temp[BUFFSIZE];

	fd_set read_fds;

	int rv;

	FD_ZERO(&read_fds);

	struct timeval timeZero;

	timeZero.tv_sec = 0;
	timeZero.tv_usec = 0;

	FD_SET(new_fd, &read_fds);

	rv = select(new_fd + 1, &read_fds, NULL , NULL, &timeZero);

	if (rv !=0)	{
		if ((numbytes=recv(new_fd, temp, BUFFSIZE, MSG_PEEK))==-1) perror("recv");
		if (temp[numbytes-1]==terminal) {
			j = 0;
			while (!end) {
				if ((numbytes=recv(new_fd, temp, BUFFSIZE, 0))==-1) perror("recv");
				for (i=0; i<numbytes; i++) {
					if (temp[i]!=terminal) {
						str[j] = temp[i];	
						j++;
					}
				}
				if (temp[i-1]==terminal) {
					end = 1;
					messaged_received = true;
				}
			}
			str[j] = '\0';
		}
	}
	
	//ack();
}

int main(int argc, char *argv[])
{
	player.a = 0;
	player.b = 0;
	player.theta = 0;
	player.phi = 0;
	player.v = 0;
	player.ball = (BallType)0;
	player.pocket = (Pocket)0;

	if (argc == 2 && strcmp(argv[1], "?") == 0) {
		cout << endl << "Client: Detailed command line option help:"
			<< endl << "   New table:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUMBER> <NUMBER OF GAMES> <MAX GAME TIME (SECS)> <BREAK MODE (1=winner breaks, 2=loser breaks, 3=alternating breaks)> <Y/N TO ECHO> <VISUALIZATION MODE (0 = no vis., 1 = vis. w/ auto exit, 2 = vis. w/ man. exit)> <Y/N TO SHOW SCORE> <NOISE TYPE (0 = no noise, 1 = Gaussian)>"
			<< endl << endl << "   Specify noise:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUMBER> <NUMBER OF GAMES> <MAX GAME TIME (SECS)> <BREAK MODE (1 = winner breaks, 2 = loser breaks, 3 = alternating breaks)> <Y/N TO ECHO> <VISUALIZATION MODE (0 = no vis., 1 = vis. w/ auto exit, 2 = vis. w/ man. exit)> <Y/N TO SHOW SCORE> <NOISE TYPE (0 = no noise, 1 = Gaussian)> <SIGMA A> <SIGMA B> <SIGMA THETA> <SIGMA PHI> <SIGMA V>" 
			<< endl << endl << "   Join table:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUMBER> <Y/N ECHO> <VISUALIZATION MODE (0 = no vis., 1 = vis. w/ auto exit, 2 = vis. w/ man. exit)> <Y/N TO SHOW SCORE>" 
			<< endl << endl;
		exit(0);
	}
	else if (argc == 8) {
		sprintf(name, "%s", argv[3]);
		tableNumber = atoi(argv[4]);
		
		if ((strcmp(argv[5], "y") == 0) || (strcmp(argv[5], "Y") == 0)) echo = true;
		if (echo) loginEcho = "true";
		else loginEcho = "false";

		visMode = atoi(argv[6]);
		if (visMode == 0) {
			visualize = false;
			manualExit = false;
		} else if (visMode == 1) {
			visualize = true;
			manualExit = false;
		} else if (visMode == 2) {
			visualize = true;
			manualExit = true;
		}
		if ((strcmp(argv[7], "y") == 0) || (strcmp(argv[7], "Y") == 0)) showScore = true;
		else showScore = false;
		
	}
	else if (argc == 12) {
		sprintf(name, "%s", argv[3]);
		tableNumber = atoi(argv[4]);
		numberOfGames = atoi(argv[5]);
		gameLength = atoi(argv[6]);
		breakStrategy = atoi(argv[7]);
		
		if ((strcmp(argv[8], "y") == 0) || (strcmp(argv[8], "Y") == 0)) echo = true;
		if (echo) loginEcho = "true";
		else loginEcho = "false";

		visMode = atoi(argv[9]);
		if (visMode == 0) {
			visualize = false;
			manualExit = false;
		} else if (visMode == 1) {
			visualize = true;
			manualExit = false;
		} else if (visMode == 2) {
			visualize = true;
			manualExit = true;
		}
		if ((strcmp(argv[10], "y") == 0) || (strcmp(argv[10], "Y") == 0)) showScore = true;
		else showScore = false;

		noiseType = atoi(argv[11]);	
	}
	else if (argc == 17) {
		sprintf(name, "%s", argv[3]);
		tableNumber = atoi(argv[4]);
		numberOfGames = atoi(argv[5]);
		gameLength = atoi(argv[6]);
		breakStrategy = atoi(argv[7]);
		
		if ((strcmp(argv[8], "y") == 0) || (strcmp(argv[8], "Y") == 0)) echo = true;
		if (echo) loginEcho = "true";
		else loginEcho = "false";
			
		visMode = atoi(argv[9]);
		if (visMode == 0) {
			visualize = false;
			manualExit = false;
		} else if (visMode == 1) {
			visualize = true;
			manualExit = false;
		} else if (visMode == 2) {
			visualize = true;
			manualExit = true;
		}
		if ((strcmp(argv[10], "y") == 0) || (strcmp(argv[10], "Y") == 0)) showScore = true;
		else showScore = false;
		
		noiseType = 2;
		sigmaA = atof(argv[12]);
		sigmaB = atof(argv[13]);
		sigmaTheta = atof(argv[14]);
		sigmaPhi = atof(argv[15]);
		sigmaV = atof(argv[16]);
	}
	else {
		cout << endl << "Client: Invalid parameters.  Usage examples:" 
			<< endl << "   New table:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUM> <NUM GAMES> <MAX TIME> <BREAK MODE> <ECHO (Y/N)> <VIS. MODE> <SHOW SCORE (Y/N)> <NOISE TYPE>" 
			<< endl << endl << "   Specify noise:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUM> <NUM GAMES> <MAX TIME> <BREAK MODE> <ECHO (Y/N)> <VIS. MODE> <SHOW SCORE (Y/N)> <NOISE TYPE> <SIGMA A> <SIGMA B> <SIGMA THETA> <SIGMA PHI> <SIGMA V>" 
			<< endl << endl << "   Join table:  ./Client <IP ADDY> <PORT> <AI NAME> <TABLE NUM> <Y/N ECHO> <VIS. MODE> <SHOW SCORE (Y/N)>" 
			<< endl << endl << "   For detailed command line option help:  ./Client ?" << endl << endl;
		exit(0);
	}

	sprintf(login, " 4 %s null %s %d false %d %d %d %d %.16f %.16f %.16f %.16f %.16f/", name, loginEcho, tableNumber, gameLength, breakStrategy, numberOfGames, noiseType, sigmaA, sigmaB, sigmaTheta, sigmaPhi, sigmaV);
	
	int port = atoi(argv[2]);
	cout << endl << "Client: Logging into server " << argv[1] << " on port " << port << "...";
	cout.flush();
	connect(port, argv[1]);
	sendString(login);
	cout << name << " logged into table " << tableNumber << " successfully." << endl;

	mainLoop();

	return 0;
}
