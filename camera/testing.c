/*******************************************************************************
#	 	luvcview: Sdl video Usb Video Class grabber           .        #
#This package work with the Logitech UVC based webcams with the mjpeg feature. #
#All the decoding is in user space with the embedded jpeg decoder              #
#.                                                                             #
# 		Copyright (C) 2005 2006 Laurent Pinchart &&  Michel Xhaard     #
#                                                                              #
# This program is free software; you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation; either version 2 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    #
#                                                                              #
*******************************************************************************/

#include "getPic.h"


int main(int argc, char** argv) {
	int num = atoi(argv[1]);

	int width = 1600;
	int height = 1200;

	if(argc > 2){
	   width = atoi(argv[2]);
	   height = atoi(argv[3]);
	}

	initCamera(num, width, height);

	//int i = 0;
	//for(; i < 10; i++)

	if(argc > 2){
		getPicture(num, argv[4]);
	}
	else
   		getPicture(num, "test.pnm");
   		
   	closeCamera(num);

}
