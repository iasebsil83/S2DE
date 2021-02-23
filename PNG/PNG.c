// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>



//png utility
#include <png.h>



//own header
#include "PNG.h"




















/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PNG [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                PNG by I.A.

        PNG is an utility program that allows you to manipulate PNG image
    files a little bit more simply than libpng does (with png.h).
    You can now read or write into a PNG file in only one line.

        The only package required to run this library is libpng. It can be
    easily installed on linux apt systems in typing :
                        sudo apt install libpng-dev

        This library is compatible with S2DE and S3DE, two graphic
    libraries I made in C using glut. They are available here :
                   https://github.com/iasebsil83/S2DE
                   https://github.com/iasebsil83/S3DE

    02/10/2020 > [0.1.0] :
    - Created PNG.c/.h.

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************
    LICENCE :
    C_PNG
    Copyright (C) 2021  Sebastien SILVANO
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */




















// ---------------- BASICS ----------------

//read - write
PNG* png_read(char* fileName){

	//error cases
	if(fileName == NULL){
		printf("RUNTIME ERROR > png.c : png_read() : FileName is NULL.\n");
		return NULL;
	}

	//file management
	FILE* f = fopen(fileName, "rb");
	if(!f){
		printf("RUNTIME ERROR > png.c : png_read() : Unable to open file \"%s\".\n", fileName);
		return NULL;
	}

	//check signature
	unsigned char sig[8];
	if(
		fread(sig, 1, 8, f) != 8 ||
		png_sig_cmp(sig, 0, 8)
	){
		printf("RUNTIME ERROR > png.c : png_read() : Invalid PNG signature for file \"%s\".\n", fileName);
		fclose(f);
		return NULL;
	}

	//init image stucture
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		printf("RUNTIME ERROR > png.c : png_read() : Could not get image structure for file \"%s\".\n", fileName);
		fclose(f);
		return NULL;
	}

	//init image info
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		printf("RUNTIME ERROR > png.c : png_read() : Could not get image info for file \"%s\".\n", fileName);
		fclose(f);
		return NULL;
	}

	//jump into data start position
	if( setjmp(png_jmpbuf(png_ptr)) ){
		printf("RUNTIME ERROR > png.c : png_read() : Could not jump into start data position for file \"%s\".\n", fileName);
		fclose(f);
		return NULL;
	}

	//init reading
	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	//prepare PNG structure
	PNG* image = malloc(sizeof(PNG));
	if(image == NULL){
		printf("FATAL ERROR > png.c : png_read() : Computer refuses to give more memory.\n");
		fclose(f);
		exit(EXIT_FAILURE);
	}

	//set dimensions
	image->width  = png_get_image_width (png_ptr, info_ptr);
	image->height = png_get_image_height(png_ptr, info_ptr);
	png_read_update_info(png_ptr, info_ptr);

	//read image data
	if( setjmp(png_jmpbuf(png_ptr)) ){
		printf("RUNTIME ERROR > png.c : png_read() : Could not start reading data for file \"%s\".\n", fileName);
		fclose(f);
		return NULL;
	}

	//prepare temporary data on Y axis
	unsigned char** tempData = malloc(sizeof(unsigned char*) * image->height);
	if(tempData == NULL){
		printf("FATAL ERROR > png.c : png_read() : Computer refuses to give more memory.\n");
		fclose(f);
		exit(EXIT_FAILURE);
	}

	//prepare temporary data on X axis
	for(int y=0; y < image->height; y++){
		tempData[y] = malloc(4 * image->width);
		if(tempData[y] == NULL){
			printf("FATAL ERROR > png.c : png_read() : Computer refuses to give more memory.\n");
			fclose(f);
			exit(EXIT_FAILURE);
		}
	}

	//store data
	png_read_image(png_ptr, tempData);
	fclose(f);

	//prepare definitive data
	image->data = malloc(4 * image->width * image->height);
	if(image->data == NULL){
		printf("FATAL ERROR > png.c : png_read() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//transfer data from temporary to definitive
	size_t index = 0;
	for(int y=image->height-1; y >= 0; y--){
		for(int x=0; x < 4*image->width; x += 4){
			image->data[index++] =
				(tempData[y][x  ] << 24) |
				(tempData[y][x+1] << 16) |
				(tempData[y][x+2] <<  8) |
				 tempData[y][x+3]
			;
		}
	}

	//free temporary data
	for(int y=0; y < image->height; y++){
		free(tempData[y]);
	}
	free(tempData);

	return image;
}

void png_write(PNG* image, char* fileName){
	//error cases
	if(fileName == NULL){
		printf("RUNTIME ERROR > png.c : png_write() : FileName is NULL.\n");
		return;
	}
	if(image == NULL){
		printf("RUNTIME ERROR > png.c : png_write() : PNG instance is NULL.\n");
		return;
	}

	//not defined yet
	printf("RUNTIME ERROR > png.c : png_write() : Function is not defined yet.\n");
}

