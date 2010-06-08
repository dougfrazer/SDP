LIBS = -L/usr/local/lib -lcv -lcvaux -lcxcore -lhighgui -lstdc++ \
       -L. -lAI -lcamera
INCLUDE = -I/usr/local/include/opencv -I../../ai -I../../camera -I../hough

all: lib lineLoop main.cpp
	g++ $(INCLUDE) -Wall -g -o lineFinder main.cpp $(LIBS) -L. -llinefinder

lib: lineFinder.cpp
	g++ $(INCLUDE) -Wall -g -shared -o liblinefinder.a lineFinder.cpp

lineLoop: lib lineFinderLoop.cpp
	g++ $(INCLUDE) -Wall -g -o lineFinderLoop lineFinderLoop.cpp $(LIBS) -llinefinder
	
#pictureLoop: pictureLoop.cpp
#	g++ $(INCLUDE) -Wall -g -o pictureLoop pictureLoop.cpp $(LIBS)

	
#lib: circleFinder.cpp
#	g++ -shared $(INCLUDE) -Wall -g -o libcircleFinder.a circleFinder.cpp $(LIBS)
	
clean:
	rm lineFinder lineFinderLoop liblinefinder.a
