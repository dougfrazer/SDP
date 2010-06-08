all:
	g++ -g -I../QMIN/poolfiz/bin -I../camera -I../ai -I/usr/local/include/opencv -I../image_rec/hough subwin.cpp -L../image_rec/lines -llinefinder -L../camera -lcamera -L../image_rec/hough -lcircleFinder -L/usr/bin -lglut -lgsl -lgslcblas -L../QMIN/poolfiz/bin -lpoolfiz -L../ai -lAI -L../serial -lSerial -lpthread -o ui
	
nocam:
	g++ -g -I../QMIN/poolfiz/bin -I../camera -I../ai -I/usr/local/include/opencv -I../image_rec/hough subwin.cpp -L../image_rec/lines -llinefinder -L../camera -lcamera -L../image_rec/hough -lcircleFinder -L/usr/bin -lglut -lgsl -lgslcblas -L../QMIN/poolfiz/bin -lpoolfiz -L../ai -lAI -L../serial -lSerial -lpthread -o ui -DNO_CAMERAS
