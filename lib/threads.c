// -------------------------------- IMPORTATIONS --------------------------------

//standard
#include <stdio.h>
#include <stdlib.h>




//multithreading
#include <pthread.h>




//own header
#include "threads.h"








/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Threads [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 Threads by I.A.

        Threads is just an utility program that allows you to manipulate
    threads a little bit more simply than pthread.h does.

    This program only adds 4 functions :
     - thread_create()
     - thread_delete()
     - thread_start()
     - thread_stop()

    08/01/2021 > [0.1.0] :
    - Created threads.c/.h.

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    C_Threads
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








// -------------------------------- THREADS --------------------------------

//create - delete
thread* thread_create(void* (*function)(void*)){
	if(function == NULL){
		printf("RUNTIME ERROR > threads.c : thread_create() : Thread function is NULL.\n");
		return NULL;
	}

	//create new thread instance
	thread* t = malloc(sizeof(thread));
	if(t == NULL){
		printf("FATAL ERROR > threads.c : thread_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set struct values
	t->function = function;
	t->tid = 0; //t->tid will be set when launching the thread

	return t;
}

void thread_delete(thread* t){

	//error cases
	if(t == NULL){
		printf("RUNTIME ERROR > threads.c : thread_delete() : Thread is NULL.\n");
		return;
	}
	if(t->function == NULL){
		printf("RUNTIME ERROR > threads.c : thread_delete() : Thread function is NULL.\n");
		return;
	}

	//liberating
	free(t->function);
	free(t);
}




//start - stop
void thread_start(thread* t){

	//error cases
	if(t == NULL){
		printf("RUNTIME ERROR > threads.c : thread_start() : Thread is NULL.\n");
		return;
	}
	if(t->function == NULL){
		printf("RUNTIME ERROR > threads.c : thread_start() : Thread function is NULL.\n");
		return;
	}

	//launch thread execution
	pthread_create(
		&(t->tid),
		NULL,
		t->function,
		NULL
	);
}

void thread_stop(thread* t, char waitForEnd){ //waitForEnd should be one of the following constants :
                                              // THREAD__WAIT_FOR_END, THREAD__INTERRUPT
	//error cases
	if(t == NULL){
		printf("RUNTIME ERROR > threads.c : thread_stop() : Thread is NULL.\n");
		return;
	}
	if(t->function == NULL){
		printf("RUNTIME ERROR > threads.c : thread_stop() : Thread function is NULL.\n");
		return;
	}

	//stop thread
	if(waitForEnd){
		if( pthread_join(t->tid, NULL) ){
			printf("RUNTIME ERROR > threads.c : thread_stop() : Thread was not detected as \"active\".\n");
			return;
		}
	}else{
		if( pthread_cancel(t->tid) ){
			printf("RUNTIME ERROR > threads.c : thread_stop() : Thread was not detected as \"active\".\n");
			return;
		}
	}
}
