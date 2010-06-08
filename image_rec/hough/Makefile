LIBS = -L/usr/local/lib -lcv -lcvaux -lcxcore -lhighgui -lstdc++
INCLUDE = -I/usr/local/include/opencv

all: lib
	g++ $(INCLUDE) -Wall -g -o circleFinder main.cpp -L. -lcircleFinder $(LIBS)
	
lib: circleFinder.cpp circleFinder.h
	g++ -shared $(INCLUDE) -Wall -g -o libcircleFinder.a circleFinder.cpp $(LIBS)
	
clean:
	rm libcircleFinder.a circleFinder
