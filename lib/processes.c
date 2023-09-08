// -------------------------------- IMPORTATIONS --------------------------------

//standard
#include <stdio.h>
#include <stdlib.h>




//processes management
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <signal.h>
#include <sys/wait.h>




//own header
#include "processes.h"








/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Processes [0.1.1] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 Processes by I.A.

        Processes is just an utility program that allows you to manipulate
    processes a little bit more simply than standard c does.

    This program only adds 4 functions :
     - proc_create()
     - proc_delete()
     - proc_start()
     - proc_stop()

    11/01/2021 > [0.1.0] :
    - Created processes.c/.h.
    - Created demonstration program.

    20/01/2021 > [0.1.1] :
    - Added stop modes in proc_stop().
    - Removed more_strings.c/.h in demonstration program.
    - proc_delete() is now liberating also #args# (given in proc_create()).

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    C_Processes
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








// -------------------------------- PROCESSES --------------------------------

//create - delete
proc* proc_create(char* exePath, char** args){
	if(exePath == NULL || args == NULL){
		printf("RUNTIME ERROR > processes.c : proc_create() : Process executable path or arguments are NULL.\n");
		return NULL;
	}

	//create new process
	proc* p = malloc(sizeof(proc));
	if(p == NULL){
		printf("FATAL ERROR > processes.c : proc_create() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set struct values
	p->exePath = exePath;
	p->args = args;
	p->pid = 0;

	return p;
}

void proc_delete(proc* p){ //will free #args# but not #exePath# ! (allowing you to use a string litteral for #exePath#)

	//error cases
	if(p == NULL){
		printf("RUNTIME ERROR > processes.c : proc_delete() : Process is NULL.\n");
		return;
	}
	if(p->exePath == NULL || p->args == NULL){
		printf("RUNTIME ERROR > processes.c : proc_delete() : Process executable path or arguments are NULL.\n");
		return;
	}

	//liberation
	char** a = p->args;
	while(*a != NULL){
		free(*a);
		a++;
	}
	free(p->args);
	free(p);
}




//start - stop
void proc_start(proc* p){

	//error cases
	if(p == NULL){
		printf("RUNTIME ERROR > processes.c : proc_start() : Process is NULL.\n");
		return;
	}
	if(p->exePath == NULL || p->args == NULL){
		printf("RUNTIME ERROR > processes.c : proc_start() : Process executable path or arguments are NULL.\n");
		return;
	}

	//launch new process
	pid_t error = fork();
	int errorCode = 0;
	switch(error){

		//cannot create fork
		case -1:
			printf("RUNTIME ERROR > processes.c : proc_start() : Unable to create a subprocess (called for \"%s\").\n", p->exePath);
			return;
		break;

		//fork created
		case 0:
			//set pid
			p->pid = getpid();

			//launch subprocess execution
			errorCode = execv(p->exePath, p->args);

			//error case
			printf("FATAL ERROR > processes.c : proc_start() : New subprocess [%i] was unable to run executable \"%s\" with error code (%i).\n", p->pid, p->exePath, errorCode);
			exit(EXIT_FAILURE);
		break;

		//parent fork continues
		default:
			p->pid = error;
		break;
	}
}

void proc_stop(proc* p, char mode){

	//error cases
	if(p == NULL){
		printf("RUNTIME ERROR > processes.c : proc_stop() : Process is NULL.\n");
		return;
	}
	if(p->exePath == NULL || p->args == NULL){
		printf("RUNTIME ERROR > processes.c : proc_stop() : Process executable path or arguments are NULL.\n");
		return;
	}
	if(p->pid <= 0){
		printf("RUNTIME ERROR > processes.c : proc_stop() : Invalid pid [%i] (must be strict positive).\n", p->pid);
		return;
	}

	//stop process
	switch(mode){

		case PROC__STOP_KILL:
			if( kill(p->pid, SIGKILL) ){
				printf("RUNTIME ERROR > processes.c : proc_stop() : Could not send stop signal to process.\n");
				return;
			}
		break;

		case PROC__STOP_WAIT:
		break;

		default:
			printf("RUNTIME ERROR > processes.c : proc_stop() : Invalid stop mode.\n");
			return;
		break;
	}

	//wait for process reception
	int status;
	if( waitpid(p->pid, &status, 0) == -1){
		printf("RUNTIME ERROR > processes.c : proc_stop() : Problem occured when waiting for process return status.\n");
		return;
	}
}
