GSL_LIBPATH = /usr/lib
GSL_INCPATH = /usr/include/gsl
POOLFIZPATH = ../poolfiz/bin/

OBJECTS = AI.o
	
INCLUDES = AI.h

CC = g++

# OPTIMIZE = -O3 -finline-functions -ffast-math
# OPTIMIZE = -O2 -finline-functions -ffast-math
# OPTIMIZE = -O1 -finline-functions -ffast-math
OPTIMIZE = -g

CFLAGS= $(OPTIMIZE) -I$(GSL_INCPATH) -I$(POOLFIZPATH) -Wall -Wno-deprecated

LIBES = -lm -lc \
	-L$(GSL_LIBPATH) -lgsl \

LIBRARY = libAI

default: AI

all: AI

install: AI

AI:	$(INCLUDES) $(OBJECTS) 	
# 	g++ -Wl,-soname,$(LIBRARY).so --shared -o $(LIBRARY).so $(OBJECTS) $(CFLAGS) $(LIBES)
	ar cruv $(LIBRARY).a $(OBJECTS)
	
.c~.o: 
	($(CC) $(CFLAGS) -c $*.c)

.cpp~.o: 
	($(CC) $(CFLAGS) -c $*.cpp)

.cpp.o:	
	($(CC) $(CFLAGS) -c $(<F))

.C.o:	
	($(CC) $(CFLAGS) -c $(<F))

clean:
	rm -f $(OBJECTS) $(LIBRARY).a


