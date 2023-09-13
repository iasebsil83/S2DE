// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

//basename
#include <libgen.h>

//threads
#include "../lib/threads.h"

//bipipes
#include "../lib/bipipes.h"

//processes
#include "../lib/processes.h"

//keycode map
#include "peer_example_keys.h"






// ---------------- DECLARATIONS ----------------

//mouse events
#define MOUSE_LEFT   '\0'
#define MOUSE_MIDDLE '\1'
#define MOUSE_RIGHT  '\2'

//global variables
#define GET false
#define SET true

//communication bipipe
#define COM_BIPIPE_LENGTH 128

//graphic subprocess options
#define TERMINATE_WHEN_ESCAPE
#define GRAPHIC_SUBPROCESS_TEMPORIZATION    500000 // (in us)

//additional options
#define VERBOSE_INVALID_CHARACTERS






// ---------------- GETTERS - SETTERS ----------------

//global variables
bipipe* comBipipe(bool action, bipipe* value){ //communication bipipe
	static bipipe* com_bipipe = NULL;
	if(action){ com_bipipe = value; } //set
	return com_bipipe;                //get
}

proc* graphicSubprocess(bool action, proc* value){
	static proc* graphic_subprocess = NULL;
	if(action){ graphic_subprocess = value; } //set
	return graphic_subprocess;                //get
}

thread* manageEventThread(bool action, thread* value){
	static thread* manageEvent_thread = NULL;
	if(action){ manageEvent_thread = value; } //set
	return manageEvent_thread;                //get
}






// ---------------- TOOLS ----------------

//conversions
unsigned int hex2halfbyte(char hex){
	if(hex < '0' || hex > 'f' || (hex > '9' && hex < 'a')){
		fprintf(stderr, "RUNTIME ERROR > peer_example.c : hex2halfbyte() : Invalid hex character '%c'.\n", hex);
		return 0;
	}

	//regular case
	if(hex >= 'a'){ return 0xa + hex-'a'; }
	return hex-'0';
}

unsigned char hex2byte(char* hex){
	return \
		(hex2halfbyte(hex[0]) << 4) | \
		(hex2halfbyte(hex[1])     )   \
	;
}

unsigned short hex2short(char* hex){
	return \
		(hex2halfbyte(hex[0]) << 12) | \
		(hex2halfbyte(hex[1]) <<  8) | \
		(hex2halfbyte(hex[2]) <<  4) | \
		(hex2halfbyte(hex[3])      )   \
	;
}

unsigned int hex2int(char* hex){
	return \
		(hex2halfbyte(hex[0]) << 28) | \
		(hex2halfbyte(hex[1]) << 24) | \
		(hex2halfbyte(hex[2]) << 20) | \
		(hex2halfbyte(hex[3]) << 16) | \
		(hex2halfbyte(hex[4]) << 12) | \
		(hex2halfbyte(hex[5]) <<  8) | \
		(hex2halfbyte(hex[6]) <<  4) | \
		(hex2halfbyte(hex[7])      )   \
	;
}






// ---------------- EXECUTION ----------------

//each event
void keyPressed(short k){
	switch(k){

		//escape key
		case KEY_ESCAPE: {
			#ifdef TERMINATE_WHEN_ESCAPE

			//kill graphic subprocess (we could have just sent "I" to him but to be sure we prefer killing him)
			proc* graphic_subprocess = graphicSubprocess(GET, NULL);
			proc_stop(graphic_subprocess, PROC__STOP_KILL);

			//end execution
			exit(EXIT_SUCCESS);
			#endif
		} break;
	}
	printf("[PEER EXAMPLE] Received KEY_PRESSED event (%04x).\n", k);
}

void keyReleased(short k){
	switch(k){
	}
	printf("[PEER EXAMPLE] Received KEY_RELEASED event (%04x).\n", k);
}

void mousePressed(unsigned char button, unsigned int x, unsigned y){
	switch(button){

		case MOUSE_LEFT:
			printf("[PEER EXAMPLE] Received MOUSE_PRESSED event on LEFT button (%u,%u).\n", x,y);
		break;

		case MOUSE_MIDDLE:
			printf("[PEER EXAMPLE] Received MOUSE_PRESSED event on MIDDLE button (%u,%u).\n", x,y);
		break;

		case MOUSE_RIGHT:
			printf("[PEER EXAMPLE] Received MOUSE_PRESSED event on RIGHT button (%u,%u).\n", x,y);
		break;
	}
}

void mouseReleased(unsigned char button, unsigned int x, unsigned y){
	switch(button){

		case MOUSE_LEFT:
			printf("[PEER EXAMPLE] Received MOUSE_RELEASED event on LEFT button (%u,%u).\n", x,y);
		break;

		case MOUSE_MIDDLE:
			printf("[PEER EXAMPLE] Received MOUSE_RELEASED event on MIDDLE button (%u,%u).\n", x,y);
		break;

		case MOUSE_RIGHT:
			printf("[PEER EXAMPLE] Received MOUSE_RELEASED event on RIGHT button (%u,%u).\n", x,y);
		break;
	}
}

void mouseMoved(unsigned int x, unsigned int y){
	printf("[PEER EXAMPLE] Received MOUSE_MOVE event (%u,%u).\n", x,y);
}

void reshaped(unsigned int nx, unsigned int ny){
	printf("[PEER EXAMPLE] Received RESHAPED event (%u,%u).\n", nx,ny);
}



//every event
void* manageEvents(void* args){

	//get communication bipipe
	bipipe* com_bipipe = comBipipe(GET, NULL);

	//listening indefinitely
	while(true){

		//receiving from bipipe (execution is stopped here, no temporization required)
		char*  reception     = bipipe_read(com_bipipe);
		size_t reception_len = strlen(reception);

		//read reception
		size_t r = 0;
		while(r < reception_len){
			switch(reception[r]){

				//key pressed
				case 'k':

					//read values
					if(reception_len < r+5){ //bytes required in sequence k0000 (5)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'k'.\n", stderr);
						break;
					}

					//call the corresponding callback
					keyPressed( hex2short(reception+r+1) );

					//shift reading head
					r += 5;
				break;

				//key released
				case 'K':

					//read values
					if(reception_len < r+5){ //bytes required in sequence K0000 (5)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'K'.\n", stderr);
						break;
					}

					//call the corresponding callback
					keyReleased( hex2short(reception+r+1) );

					//shift reading head
					r += 5;
				break;



				//mouse pressed
				case 'm':

					//read values
					if(reception_len < r+18){ //bytes required in sequence m01111111100000000 (18)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'm'.\n", stderr);
						break;
					}

					//call the corresponding callback
					mousePressed(
						hex2byte(reception+r+1),
						hex2int(reception+r+2),
						hex2int(reception+r+10)
					);

					//shift reading head
					r += 18;
				break;



				//mouse released
				case 'M':

					//read values
					if(reception_len < r+18){ //bytes required in sequence M01111111100000000 (18)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'M'.\n", stderr);
						break;
					}

					//call the corresponding callback
					mouseReleased(
						hex2byte(reception+r+1),
						hex2int(reception+r+2),
						hex2int(reception+r+10)
					);

					//shift reading head
					r += 18;
				break;



				//mouse moved
				case 'n':

					//read values
					if(reception_len < r+17){ //bytes required in sequence n0000000011111111 (17)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'n'.\n", stderr);
						break;
					}

					//call the corresponding callback
					mouseMoved(
						hex2int(reception+r+1),
						hex2int(reception+r+9)
					);

					//shift reading head
					r += 17;
				break;



				//reshaped window
				case 'z':

					//read values
					if(reception_len < r+17){ //bytes required in sequence z0000000011111111 (17)
						fputs("RUNTIME ERROR > peer_example.c : manageEvents() : Missing elements in sequence 'z'.\n", stderr);
						break;
					}

					//call the corresponding callback
					reshaped(
						hex2int(reception+r+1),
						hex2int(reception+r+9)
					);

					//shift reading head
					r += 17;
				break;



				//undefined sequence
				default:
					#ifdef VERBOSE_INVALID_CHARACTERS
					fprintf(stderr, "RUNTIME ERROR > peer_example.c : manageEvents() : Invalid character '%c'.\n", reception[r]);
					#endif
				break;
			}

			//increase reading head
			r++;
		}

		//free reception string
		free(reception);
	}
}



//main
int main(int argc, char** argv){



	// GRAPHIC SUBPROCESS

	//missing S2DE_bipipe path argument
	if(argc == 1){
		fputs("FATAL_ERROR > peer_example.c : main() : Missing S2DE_bipipe path at first position.\n", stderr);
		return EXIT_FAILURE;
	}

	//create communication bipipe
	bipipe* com_bipipe = comBipipe(SET, bipipe_create(COM_BIPIPE_LENGTH)); //get & set at the same time

	//reformat arguments to fit with peer
	char* graphic_subprocess_fullPath = argv[1];
	argv[0] = basename(graphic_subprocess_fullPath);
	argv[1] = com_bipipe->info;

	//create & start graphic subprocess
	proc* graphic_subprocess = graphicSubprocess(SET, proc_create(graphic_subprocess_fullPath, argv)); //get & set at the same time
	proc_start(graphic_subprocess);
	usleep(GRAPHIC_SUBPROCESS_TEMPORIZATION);

	//set bipipe as we are a PARENT process
	bipipe_everybodyJoined(com_bipipe);

	//event listening thread
	thread* manageEvent_thread = manageEventThread(SET, thread_create(manageEvents, NULL)); //get & set at the same time
	thread_start(manageEvent_thread);



	// EXECUTION

	//send orders to S2DE : create window
	bipipe_write(com_bipipe, "i00000438000001e0my Wonderful window`");

	//send orders to S2DE : draw some things
	puts("Displaying something for 8s.");
	bipipe_write(com_bipipe, "o89f5f5c2");                                         //set background color to light cyan
	bipipe_write(com_bipipe, "O0000ffff");                                         //set front color to blue
	bipipe_write(com_bipipe, "R00000032000000320000006400000064");                 //draw filled rectangle at (50,50, 100,100)
	bipipe_write(com_bipipe, "Off0000ff");                                         //set font color to red
	bipipe_write(com_bipipe, "t000000c8000000c8000000e1000000fa000000af000000fa"); //draw empty triangle at (200,200, 225,250, 175,250)
	usleep(8000000);

	//reset display
	puts("Displaying something else.");
	bipipe_write(com_bipipe, "h");
	usleep(1000000);

	//draw something else
	bipipe_write(com_bipipe, "of5c2c2ff");                                         //set background color to light red
	bipipe_write(com_bipipe, "O00ff00ff");                                         //set front color to green
	bipipe_write(com_bipipe, "R00000032000000320000006400000064");                 //draw filled rectangle at (50,50, 100,100)
	bipipe_write(com_bipipe, "O000000ff");                                         //set font color to black
	bipipe_write(com_bipipe, "t000000c8000000c8000000e1000000fa000000af000000fa"); //draw empty triangle at (200,200, 225,250, 175,250)

	//main loop
	while(1){
		usleep(1000);
	}

	return EXIT_SUCCESS;
}
