GL_INCPATH = /usr/include/w32api/GL
GL_LIBPATH = /usr/lib
GLUT_INCPATH = /usr/include/GL
GLUT_LIBPATH = /usr/bin
#X_INCPATH = /usr/X11R6/include/X11
#X_LIBPATH = /usr/X11R6/lib
GSL_LIBPATH = /lib/
POOLFIZPATH = ../../poolfiz/bin/

CC = g++

# OPTIMIZE = -O3 -finline-functions -ffast-math
# OPTIMIZE = -O2 -finline-functions -ffast-math
# OPTIMIZE = -O1 -finline-functions -ffast-math
OPTIMIZE = -g

CFLAGS= $(OPTIMIZE) -I$(GL_INCPATH) -I$(GLUT_INCPATH) -I$(POOLFIZPATH) \
#	-I$(X_INCPATH) 

LIBES = -lm -lc \
	-L$(POOLFIZPATH) -lpoolfiz \
	-L$(GLUT_LIBPATH) -lglut -lGLU -lGL \
	-L$(GSL_LIBPATH) -lgsl -lgslcblas \
#	-L$(X_LIBPATH) -lXmu -lX11 -lXt -lXext -lXi
	
OBJECTS = dbViz.o

TARGET = dbViz

all : $(TARGET)

$(TARGET) : $(INCLUDES) $(OBJECTS)
	$(CC) $(OPTIMIZE) $(CFLAGS) $(OBJECTS) $(LIBES) -o $@ 

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

