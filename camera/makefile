CPPFILES = avilib.c color.c getPic.c utils.c v4l2uvc.c
HEADERS = avilib.h color.h dynctrl-logitech.h getPic.h huffman.h utils.h uvc_compat.h uvcvideo.h v4l2uvc.h


#define C_COMPILER when compiling for C and not C++
all: lib testing.c
	gcc -g -o getPic testing.c -L. -lcamera -DC_COMPILER

lib: $(CPPFILES) $(HEADERS)
	gcc -g -shared -o libcamera.a $(CPPFILES) -lpthread -DC_COMPILER

clean: 
	rm libcamera.a getPic
	
