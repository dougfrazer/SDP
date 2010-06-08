GL_INCPATH = /usr/include/w32api/GL
GL_LIBPATH = /usr/lib/w32api
GLUT_INCPATH = /usr/include/GL
GLUT_LIBPATH = /usr/bin
GSL_LIBPATH = /usr/lib

POOLFIZPATH = ../poolfiz/bin/
AI_LIBPATH = ../ai/
AI_INCPATH = ../ai/

OBJECTS = Client.o

CC = g++

# OPTIMIZE = -O3 -finline-functions -ffast-math
# OPTIMIZE = -O2 -finline-functions -ffast-math
# OPTIMIZE = -O1 -finline-functions -ffast-math
OPTIMIZE = -g

CFLAGS= $(OPTIMIZE) -Wno-deprecated -I$(GL_INCPATH) -I$(GLUT_INCPATH) -I$(POOLFIZPATH) -I$(AI_INCPATH)

LIBES = -lm -lc \
	-L$(POOLFIZPATH) -lpoolfiz \
	-L$(AI_LIBPATH) -lAI \
	-L$(GLUT_LIBPATH) -lglut -lGLU -lGL \
	-L$(GSL_LIBPATH) -lgsl -lgslcblas \

TARGET = Client

default: $(TARGET)

Client: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBES) -o $@ 

Client.o: Client.cpp
	$(CC) $(CFLAGS) -c Client.cpp -I$(POOLFIZPATH) -I$(AI_INCPATH)

clean:
	rm -f $(OBJECTS) $(TARGET)



