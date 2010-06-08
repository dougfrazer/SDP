GLUT_INCPATH = /usr/include/GL
GLUT_LIBPATH = /usr/bin
#X_INCPATH = /usr/X11R6/include/X11
#X_LIBPATH = /usr/X11R6/lib
GSL_LIBPATH = /lib/
POOLFIZPATH = ../../poolfiz/bin/
GLUIINCPATH = ../../../lib/glui/src/include/GL
GLUIPATH = ../../../lib/glui/src/lib/

CC = g++

#	OPTIMIZE = -O3 -finline-functions -ffast-math
#	OPTIMIZE = -O2 -finline-functions -ffast-math
#	OPTIMIZE = -O1 -finline-functions -ffast-math
OPTIMIZE = -g

CFLAGS= $(OPTIMIZE) -I$(GLUT_INCPATH) -I$(POOLFIZPATH)\
#	-I$(X_INCPATH) 

LIBES = -lm -lc \
	-L$(POOLFIZPATH) -lpoolfiz \
	-L$(GLUT_LIBPATH) -lglut -lGLU -lGL \
	-L$(GSL_LIBPATH) -lgsl -lgslcblas \
#	-L$(X_LIBPATH) -lXmu -lX11 -lXt -lXext -lXi
	
OBJECTS = glutsubwin.o

TARGET = glutsubwin

all : $(TARGET)
$(TARGET) : $(INCLUDES) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBES) -o $@

.c~.o: 
	($(CC) $(CFLAGS) -c $*.c)

.cpp~.o: 
	($(CC) $(CFLAGS) -c $*.cpp)

.cpp.o:	
	($(CC) $(CFLAGS) -c $(<F))

.C.o:	
	($(CC) $(CFLAGS) -c $(<F))

OBJECTS: $(INCLUDES)

clean:
	-rm -f $(OBJECTS) $(TARGET)

