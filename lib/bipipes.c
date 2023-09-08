// -------------------------------- IMPORTATIONS --------------------------------

//standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




//own header
#include "bipipes.h"








/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Bipipes [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 Bipipes by I.A.

        Bipipes is an utility program that allows you to manipulate linux
    pipes in 2-way easily.

    14/01/2021 > [0.1.0] :
    - Created bipipes.c/.h.
    - Added the demonstration program.

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A..
******************************************************************************************

    LICENCE :

    C_Bipipes
    Copyright (C) 2021 Sebastien SILVANO

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.

    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */








// -------------------------------- HEX TOOLS --------------------------------

//conversions
static int __1hexchar_to_int__(char h){ //output value is on 32 bits but only the 4 LSb are used
	switch(h){
		case '1':
			return 0x1;
		case '2':
			return 0x2;
		case '3':
			return 0x3;
		case '4':
			return 0x4;
		case '5':
			return 0x5;
		case '6':
			return 0x6;
		case '7':
			return 0x7;
		case '8':
			return 0x8;
		case '9':
			return 0x9;
		case 'a':
		case 'A':
			return 0xa;
		case 'b':
		case 'B':
			return 0xb;
		case 'c':
		case 'C':
			return 0xc;
		case 'd':
		case 'D':
			return 0xd;
		case 'e':
		case 'E':
			return 0xe;
		case 'f':
		case 'F':
			return 0xf;
	}

	return 0x0;
}

static int __8hexchar_to_int__(char* h){ // #h# don't require an '\0' at the end
	return                       //example : "ffffff9c" => -100
		( (__1hexchar_to_int__(h[0]) << 28) & 0xf0000000 ) + //byte 4 (MSB)
		( (__1hexchar_to_int__(h[1]) << 24) & 0x0f000000 ) +

		( (__1hexchar_to_int__(h[2]) << 20) & 0x00f00000 ) + //byte 3
		( (__1hexchar_to_int__(h[3]) << 16) & 0x000f0000 ) +

		( (__1hexchar_to_int__(h[4]) << 12) & 0x0000f000 ) + //byte 2
		( (__1hexchar_to_int__(h[5]) <<  8) & 0x00000f00 ) +

		( (__1hexchar_to_int__(h[6]) <<  4) & 0x000000f0 ) + //byte 1 (LSB)
		(  __1hexchar_to_int__(h[7])        & 0x0000000f )
	;
}








// -------------------------------- BIPIPES --------------------------------

//create - delete
bipipe* bipipe_create(unsigned int length){
	if(length == 0){
		printf("RUNTIME ERROR > bipipes.c : bipipe_create() : Cannot allocate 0 bytes.\n");
		return NULL;
	}

	//create new bipipe instance
	bipipe* bp = malloc(sizeof(bipipe));
	if(bp == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set pRcW pipe
	bp->pRcW_fd = malloc(8);
	if(bp->pRcW_fd == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}
	if( pipe( (int*)(bp->pRcW_fd) ) == -1){
		printf("RUNTIME ERROR > bipipes.c : bipipe_create() : One pipe couldn't be created.\n");
		free(bp->pRcW_fd);
		free(bp);
		return NULL;
	}

	//set child pipe
	bp->pWcR_fd = malloc(8);
	if(bp->pWcR_fd == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}
	if( pipe( (int*)(bp->pWcR_fd) ) == -1){
		printf("RUNTIME ERROR > bipipes.c : bipipe_create() : One pipe couldn't be created.\n");
		free(bp->pRcW_fd);
		free(bp->pWcR_fd);
		free(bp);
		return NULL;
	}

	//set length
	bp->length = length;

	//set owner
	bp->isParent = 1; //parents create pipes, children join them

	//set ready state
	bp->ready = 0; //bipie is not ready to use

	//set info
	bp->info = malloc(41);
	if(bp->info == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}
	sprintf(
		bp->info,
		"%08x%08x%08x%08x%08x",
		bp->pRcW_fd[0],
		bp->pRcW_fd[1],
		bp->pWcR_fd[0],
		bp->pWcR_fd[1],
		bp->length
	);

	return bp;
}

void bipipe_delete(bipipe* bp){

	//error cases
	if(bp == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_delete() : Bipipe instance is NULL.\n");
		return;
	}

	//close pipe entries
	if(bp->ready){

		//close only the concerned entries
		if(bp->isParent){
			close( bp->pRcW_fd[0] );
			close( bp->pWcR_fd[1] );
		}else{
			close( bp->pWcR_fd[0] );
			close( bp->pRcW_fd[1] );
		}
	}else{
		//close all entries
		close( bp->pRcW_fd[0] );
		close( bp->pRcW_fd[1] );
		close( bp->pWcR_fd[0] );
		close( bp->pWcR_fd[1] );
	}

	//free info             // (parents are comming from bipipe_create() which uses malloc() for info,
	if(bp->isParent){       //  children are comming from bipipe_join() which uses a given char*)
		free(bp->info);
	}

	//free bipipe
	free(bp->pRcW_fd);
	free(bp->pWcR_fd);
	free(bp);
}




//join - everybodyJoined
bipipe* bipipe_join(char* info){

	//error case
	if(info == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_join() : Info is NULL.\n");
		return NULL;
	}

	//create new bipipe instance
	bipipe* bp = malloc(sizeof(bipipe));
	if(bp == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_join() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set length
	bp->length = __8hexchar_to_int__( info+32 );
	if(bp->length == 0){
		printf("RUNTIME ERROR > bipipes.c : bipipe_join() : Cannot allocate 0 bytes.\n");
		free(bp);
		return NULL;
	}

	//set pRcW
	bp->pRcW_fd = malloc(8);
	if(bp->pRcW_fd == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_join() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}
	bp->pRcW_fd[0] = __8hexchar_to_int__( info    );
	bp->pRcW_fd[1] = __8hexchar_to_int__( info+ 8 );

	//set child pipe
	bp->pWcR_fd = malloc(8);
	if(bp->pWcR_fd == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_join() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}
	bp->pWcR_fd[0] = __8hexchar_to_int__( info+16 );
	bp->pWcR_fd[1] = __8hexchar_to_int__( info+24 );

	//close unconcerned entries
	close( bp->pRcW_fd[0] );
	close( bp->pWcR_fd[1] );

	//set owner
	bp->isParent = 0; //parents create pipes, children join them

	//set ready state
	bp->ready = 1; //children are always ready

	//set info
	bp->info = info;

	return bp;
}

void bipipe_everybodyJoined(bipipe* bp){ //WARNING ! Only parent process bipipes can use this function

	//error cases
	if(bp == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_everybodyJoined() : Bipipe instance is NULL.\n");
		return;
	}
	if(!bp->isParent){
		printf("RUNTIME ERROR > bipipes.c : bipipe_everybodyJoined() : Only parent process bipipes can use this function.\n");
		return;
	}
	bp->ready = 1;

	//close the parent unconcerned sides
	close( bp->pRcW_fd[1] );
	close( bp->pWcR_fd[0] );
}




//read - write
char* bipipe_read(bipipe* bp){

	//error cases
	if(bp == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_read() : Bipipe instance is NULL.\n");
		return NULL;
	}
	if(!bp->ready){
		printf("RUNTIME ERROR > bipipes.c : bipipe_read() : Bipipe is not ready (please call bipipe_everybodyJoined()).\n");
		return NULL;
	}

	//prepare data
	char* data = malloc(bp->length);
	if(data == NULL){
		printf("FATAL ERROR > bipipes.c : bipipe_read() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//read data
	if(bp->isParent){
		if( read(bp->pRcW_fd[0], data, bp->length) <= 0){
			printf("RUNTIME ERROR > bipipes.c : bipipe_read() : Could not read from bipipe (parent process side).\n");
			return NULL;
		}
	}else{
		if( read(bp->pWcR_fd[0], data, bp->length) <= 0){
			printf("RUNTIME ERROR > bipipes.c : bipipe_read() : Could not read from bipipe (child process side).\n");
			return NULL;
		}
	}

	return data;
}

void bipipe_write(bipipe* bp, char* data){ // data will not be free after use
                                           //WARNING ! data MUST HAVE length >= bp->length ('\0' IS NOT DETERMINATIVE)
	//error cases
	if(bp == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_write() : Bipipe instance is NULL.\n");
		return;
	}
	if(!bp->ready){
		printf("RUNTIME ERROR > bipipes.c : bipipe_write() : Bipipe is not ready (please call bipipe_everybodyJoined()).\n");
		return;
	}
	if(data == NULL){
		printf("RUNTIME ERROR > bipipes.c : bipipe_write() : Given data is NULL.\n");
		return;
	}

	//read data
	if(bp->isParent){
		if( write(bp->pWcR_fd[1], data, bp->length) <= 0){
			printf("RUNTIME ERROR > bipipes.c : bipipe_write() : Could not write from bipipe (parent process side).\n");
		}
	}else{
		if( write(bp->pRcW_fd[1], data, bp->length) <= 0){
			printf("RUNTIME ERROR > bipipes.c : bipipe_write() : Could not write from bipipe (child process side).\n");
		}
	}
}
