SRC = ./
INC = ./
LIBS = 
CC = g++
CFLAGS = -Wall -g -O3 


# -----------------------------------------------------------------------
# Makefile script for building all programs and docs

all: test lib

# -----------------------------------------------------------------------

# Makefile script for generating serial port test program
test: serial.o test.o
	$(CC) $(LIBS) $(CFLAGS) serial.o test.o -o test

test.o: $(SRC)/test.cpp $(SRC)/serial.cpp $(INC)/serial.h
	$(CC) $(CFLAGS)-c $(SRC)/test.cpp
	
lib: serial.o test.o
	$(CC) -shared $(LIBS) $(CFLAGS) serial.o -o libSerial.a

serial.o: $(SRC)/serial.cpp $(INC)/serial.h
	$(CC) $(CFLAGS)-c $(SRC)/serial.cpp
	
lib: serial.o serial.h
	$(CC) $(LIBS) $(CFLAGS) -shared -o libSerial.a serial.o

# ---------------------------------------------------------------------

#----------------clean old objects for rebuilding --------------------
clean:
	rm *.o -f
