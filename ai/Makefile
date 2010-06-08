GSL_LIBPATH = /usr/lib
GSL_INCPATH = /usr/include/gsl
GLUT_INCPATH = /usr/include/GL
GLUT_LIBPATH = /usr/bin
POOLFIZPATH = .

CPPSRC = AI.cpp box_muller.cpp probtable.cpp shotutil.cpp velocitytable.cpp
OBJECTS = $(CPPSRC:.cpp=.o)

TEST_CPP = main.cpp
TEST_OBJ = $(TEST_CPP:.cpp=.o)
TEST_OUTPUT = aiMain
	
INCLUDES = AI.h

CC = g++

# OPTIMIZE = -O3 -finline-functions -ffast-math
# OPTIMIZE = -O2 -finline-functions -ffast-math
# OPTIMIZE = -O1 -finline-functions -ffast-math
OPTIMIZE = -g

CFLAGS= $(OPTIMIZE) -I$(GSL_INCPATH) -I$(POOLFIZPATH) -Wall -Wno-deprecated

LIBES = -lm -lc \
	-L$(POOLFIZPATH) -lpoolfiz \
	-L$(GSL_LIBPATH) -lgsl -lgslcblas \
	-L$(GLUT_LIBPATH) -lglut -lGLU -lGL

LIBRARY = libAI

default: TEST

all: TEST

install: lib

lib:	$(INCLUDES) $(OBJECTS) 	
# 	g++ -Wl,-soname,$(LIBRARY).so -o $(LIBRARY).so $(OBJECTS) $(CFLAGS) $(LIBES)
#	ar cruv $(LIBRARY).a $(OBJECTS)
TEST: lib $(TEST_OBJ) $(INCLUDES)
	g++ -g3 -Wall -o $(TEST_OUTPUT) $(TEST_OBJ) $(CFLAGS) -L. -lAI

main:
	g++ -g3 -Wall -o $(TEST_OUTPUT) $(TEST_OBJ) $(CFLAGS) -L. -lAI
	
lib:
	g++ -g -Wall -shared -o libAI.a $(OBJECTS) $(CFLAGS) $(LIBES)

	
.c~.o: 
	($(CC) $(CFLAGS) -c $*.c)

.cpp~.o: 
	($(CC) $(CFLAGS) -c $*.cpp)

.cpp.o:	
	($(CC) $(CFLAGS) -c $(<F))

.C.o:	
	($(CC) $(CFLAGS) -c $(<F))

clean:
	rm -f $(OBJECTS) main.o $(LIBRARY).a


