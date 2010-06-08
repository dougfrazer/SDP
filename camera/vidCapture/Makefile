LIBS = -L/usr/local/lib -lcv -lcvaux -lcxcore -lhighgui -lstdc++
INCLUDE = -I/usr/local/include/opencv

all: lib
#	g++ $(INCLUDE) -Wall -g -o frameCapture main.cpp -L. -lcircleFinder $(LIBS)
	
lib: frameCapture.cpp
	g++ $(INCLUDE) -Wall -g -o frameCapture frameCapture.cpp $(LIBS)
	
clean:
	rm frameCapture
