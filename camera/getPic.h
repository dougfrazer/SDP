#ifndef GET_PIC_H
#define GET_PIC_H

#ifdef C_COMPILER
	void initCamera(int deviceNum, int width, int height);
	void getPicture(int deviceNum, char* fileName);
	void closeCamera(int deviceNum);

#else
//so we can call these functions from c++ programs (i.e. the ui)
	extern "C"{
		void initCamera(int deviceNum, int width, int height);
		void getPicture(int deviceNum, char* fileName);
		void closeCamera(int deviceNum);
	}

void initCamera(int deviceNum);
#endif
#endif
