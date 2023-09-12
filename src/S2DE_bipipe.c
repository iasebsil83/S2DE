// ---------------- IMPORTATIONS ----------------

//standard
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

//basename
#include <libgen.h>

//queues
#include "../lib/queues.h"

//threads
#include "../lib/threads.h"

//processes
#include "../lib/processes.h"

//bipipes
#include "../lib/bipipes.h"

//graphics
#include "../lib/S2DE.h" //2D engine






/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ S2DE bipipe [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Developped using S2DE :

        This application is meant to execute S2DE functions (2D graphical
    interactions) from instructions that will be exchanged using a bipipe.
    Therefore, this program requires a peer application for sending
    graphical orders & receiving events.

        The peer application must be given as argument when executing
    S2DE_bipipe and their arguments themselves will be a tiny bit changed :

    Instead of having :
        myApp arg1 arg2...

    We will have :
        myApp <bpinfo> arg1 arg2...  with <bpinfo>, the informations for
                                     connecting back with the bipipe.

    Here is an example command :
      > ./S2DE_bipipe myApp arg1 arg2...
      #
      # Programs 'S2DE_pipe' & 'myApp' will be executed simultaneously.
      #  - 'S2DE_pipe'                   will wait for orders from myApp &
      #                                  send its event requests as well
      #                                  using the created bipipe.
      #
      #  - 'myApp <bpinfo> arg1 arg2...' will be executed and will have to
      #                                  connect to the created bipipe using
      #                                  <bpinfo>

        Instructions communicating through the bipipe are simple ASCII
    sequences :

      - S2DE_bipipe will receive orders :
        "i0000000011111111<title_name>`"                                    : Initialize window with w,h on 32b & title until '`' found
        "I"                                                                 : Close window
        "c001100"                                                           : Set background color with r,g,b on 3*8b
        "C001100"                                                           : Set front color with r,g,b on 3*8b
        "p00000000111111110000000011111111"                                 : Draw point with x,y on 32b
        "l00000000111111110000000011111111"                                 : Draw line with x1,y1,x2,y2
        "t000000001111111100000000111111110000000011111111"                 : Draw empty triangle with x1,y1,x2,y2,x3,y3 on 32b
        "T000000001111111100000000111111110000000011111111"                 : Same thing but filled
        "r00000000111111110000000011111111"                                 : Draw empty rectangle with x1,y1,x2,y2 on 32b
        "R00000000111111110000000011111111"                                 : Same thing but filled
        "q0000000011111111000000001111111100000000111111110000000011111111" : Draw empty quad with x1,y1,x2,y2,x3,y3,x4,y4 on 32b
        "Q0000000011111111000000001111111100000000111111110000000011111111" : Same thing but filled

      - S2DE_bipipe will send events :
        "k0000"              : Key pressed with key on 32b
        "K0000"              : Key released with key on 32b
        "m01111111100000000" : Mouse press with button on 4b & x,y on 32b
        "M01111111100000000" : Mouse release with button on 4b & x,y on 32b
        "s"                  : Mouse scroll down
        "S"                  : Mouse scroll up
        "n0000000011111111"  : Mouse moved with x,y on 32b
        "z0000000011111111"  : Screen reshaped with new_w,new_h on 32b

    Any other sequence will be skipped.

    08/09/2023 > [0.1.0] :
    - Created S2DE_bipipe.c

    BUGS : .
    NOTES : .

    Contact      : i.a.sebsil83@gmail.com
    Youtube      : https://www.youtube.com/user/IAsebsil83
    Repositories : https://github.com/iasebsil83
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */






// ---------------- DECLARATIONS ----------------

//event variables
extern int S2DE_mouseState; //mouse
extern int S2DE_mouseButton;
extern int S2DE_mouseScroll;
extern int S2DE_mouseX;
extern int S2DE_mouseY;
extern int S2DE_keyState; //keyboard
extern short S2DE_key;
extern unsigned int S2DE_newWidth; //resize
extern unsigned int S2DE_newHeight;
extern unsigned int S2DE_width;
extern unsigned int S2DE_height;

//refresh delay
#define DISPLAY_FPS 10

//global variables
#define GET false
#define SET true
#define DISPLAY_OBJECTS_MAX   50
#define PEER_KILL_WHEN_ESCAPE        //kill peer subprocess if escape key is pressed
#define PEER_TEMPORIZATION    500000 //delay (in us) before starting instruction analysis loop (time for the subprocess to be launched)
#define BIPIPE_LENGTH         128    // (in B)

//additionnal option (mouse move event can overwhelm other events)
//#define IGNORE_MOUSE_MOVE
//#define VERBOSE_INVALID_CHARACTERS






// ---------------- GETTER - SETTERS ----------------

//global variables
bipipe* comBipipe(bool action, bipipe* value){
	static bipipe* com_bipipe = NULL;
	if(action){ com_bipipe = value; } //set
	return com_bipipe;                //get
}

proc* peerSubprocess(bool action, proc* value){
	static proc* peer_subprocess = NULL;
	if(action){ peer_subprocess = value; } //set
	return peer_subprocess;                //get
}

queue* displayObjects(bool action, queue* value){
	static queue* display_objects = NULL;
	if(action){ display_objects = value; } //set
	return display_objects;                //get
}






// ---------------- TOOLS ----------------

//conversions
char halfbyte2hex(unsigned int v){
	if(v > 0xf){
		fprintf(stderr, "RUNTIME ERROR > S2DE_bipipe.c : halfbyte2hex() : Invalid half-byte value '%d'.\n", v);
		return '0';
	}

	//regular case
	if(v < 0xa){ return '0' + v; }
	return 'a' + (v-0xa);
}

void short2hex(char* result, unsigned int s){
	result[0] = halfbyte2hex( (s & 0x000f)      );
	result[1] = halfbyte2hex( (s & 0x00f0) >>  4);
	result[2] = halfbyte2hex( (s & 0x0f00) >>  8);
	result[3] = halfbyte2hex( (s & 0xf000) >> 12);
}

void int2hex(char* result, unsigned int i){
	result[0] = halfbyte2hex( (i & 0x0000000f)      );
	result[1] = halfbyte2hex( (i & 0x000000f0) >>  4);
	result[2] = halfbyte2hex( (i & 0x00000f00) >>  8);
	result[3] = halfbyte2hex( (i & 0x0000f000) >> 12);
	result[4] = halfbyte2hex( (i & 0x000f0000) >> 16);
	result[5] = halfbyte2hex( (i & 0x00f00000) >> 20);
	result[6] = halfbyte2hex( (i & 0x0f000000) >> 24);
	result[7] = halfbyte2hex( (i & 0xf0000000) >> 28);
}

unsigned int hex2halfbyte(char hex){
	if(hex < '0' || (hex > '9' && hex < 'a') || hex > 'f'){
		fprintf(stderr, "RUNTIME ERROR > S2DE_bipipe.c : hex2halfbyte() : Invalid hex character '%c'.\n", hex);
		return 0;
	}

	//regular case
	if(hex >= 'a'){ return 0xa + hex-'a'; }
	return hex-'0';
}

unsigned char hex2byte(char* hex){
	return \
		(hex2halfbyte(hex[0]) <<  4) + \
		(hex2halfbyte(hex[1])      )   \
	;
}

unsigned int hex2int(char* hex){
	return \
		(hex2halfbyte(hex[0]) << 28) + \
		(hex2halfbyte(hex[1]) << 24) + \
		(hex2halfbyte(hex[2]) << 20) + \
		(hex2halfbyte(hex[3]) << 16) + \
		(hex2halfbyte(hex[4]) << 12) + \
		(hex2halfbyte(hex[5]) <<  8) + \
		(hex2halfbyte(hex[6]) <<  4) + \
		(hex2halfbyte(hex[7])      )   \
	;
}




// ---------------- EXECUTION ----------------

//draw from given instruction
void draw(char* sequence){
	unsigned char r,g,b;
	unsigned int  x1,y1, x2,y2, x3,y3, x4,y4;
	switch(sequence[0]){

		//background color
		case 'c':
			r = hex2byte(sequence+1);	g = hex2byte(sequence+2);	b = hex2byte(sequence+3);
			S2DE_setBackColor(r,g,b);
		break;

		//front color
		case 'C':
			r = hex2byte(sequence+1);	g = hex2byte(sequence+2);	b = hex2byte(sequence+3);
			S2DE_setFrontColor(r,g,b);
		break;

		//point
		case 'p':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			S2DE_point(x1,y1);
		break;

		//line
		case 'l':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			S2DE_line(x1,y1, x2,y2);
		break;

		//empty triangle
		case 't':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			x3 = hex2int(sequence+33);	y3 = hex2int(sequence+41);
			S2DE_triangle(x1,y1, x2,y2, x3,y3, false);
		break;

		//full triangle
		case 'T':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			x3 = hex2int(sequence+33);	y3 = hex2int(sequence+41);
			S2DE_triangle(x1,y1, x2,y2, x3,y3, true);
		break;

		//empty rectangle
		case 'r':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			S2DE_rectangle(x1,y1, x2,y2, false);
		break;

		//full rectangle
		case 'R':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			S2DE_rectangle(x1,y1, x2,y2, true);
		break;

		//empty quad
		case 'q':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			x3 = hex2int(sequence+33);	y3 = hex2int(sequence+41);
			x4 = hex2int(sequence+49);	y4 = hex2int(sequence+57);
			S2DE_quad(x1,y1, x2,y2, x3,y3, x4,y4, false);
		break;

		//full quad
		case 'Q':
			x1 = hex2int(sequence+1);	y1 = hex2int(sequence+9);
			x2 = hex2int(sequence+17);	y2 = hex2int(sequence+25);
			x3 = hex2int(sequence+33);	y3 = hex2int(sequence+41);
			x4 = hex2int(sequence+49);	y4 = hex2int(sequence+57);
			S2DE_quad(x1,y1, x2,y2, x3,y3, x4,y4, true);
		break;

		//unknown sequence
		default:
			fprintf(stderr, "INTERNAL ERROR > S2DE_bipipe.c : draw() : Unknown instruction '%c'.\n", sequence[0]);
	}
}



//events
void S2DE_event(int event){
	char* sequence;

	//get global vars
	queue*  display_objects = displayObjects(GET, NULL);
	bipipe* com_bipipe      = comBipipe(GET, NULL);

	//redirect event
	switch(event){

		//display
		case S2DE_DISPLAY:

			//draw each object
			for(size_t q=0; q < display_objects->length; q++){
				draw( queue_get(display_objects, q) );
			}

			//reset queue
			queue_clear(display_objects);
		break;



		//keyboard
		case S2DE_KEYBOARD:

			//special behaviour
			#ifdef PEER_KILL_WHEN_ESCAPE
			if(S2DE_key == S2DE_KEY_ESCAPE){
				proc* peer_subprocess = peerSubprocess(GET, NULL);
				proc_stop(peer_subprocess, PROC__STOP_KILL);
			}
			#endif

			//regular case
			sequence    = malloc(6); //k0000\0
			sequence[5] = '\0';

			//state
			if(S2DE_keyState == S2DE_KEY_PRESSED)
				sequence[0] = 'k';
			else
				sequence[0] = 'K';

			//key
			short2hex(sequence+1, S2DE_key);

			//send sequence
			bipipe_write(com_bipipe, sequence);
			free(sequence);
		break;



		//mouse
		case S2DE_MOUSE_CLICK:
			sequence     = malloc(19); //m01111111100000000\0
			sequence[18] = '\0';

			//state
			if(S2DE_mouseState == S2DE_MOUSE_PRESSED)
				sequence[0] = 'm';
			else
				sequence[0] = 'M';

			//button
			sequence[1] = halfbyte2hex(S2DE_mouseButton);

			//coordinates
			int2hex(sequence+2,  S2DE_mouseX);
			int2hex(sequence+10, S2DE_mouseY);

			//send sequence
			bipipe_write(com_bipipe, sequence);
			free(sequence);
		break;



		//mouse scroll
		case S2DE_MOUSE_SCROLL:
			sequence    = malloc(2); //s\0
			sequence[1] = '\0';

			//scroll
			if(S2DE_mouseScroll == S2DE_SCROLL_DOWN){
				sequence[0] = 's';
			}else{
				sequence[0] = 'S';
			}

			//send sequence
			bipipe_write(com_bipipe, sequence);
			free(sequence);
		break;



		//mouse move
		case S2DE_MOUSE_MOVE:
			#ifndef IGNORE_MOUSE_MOVE
			sequence   = malloc(18); //n0000000011111111\0
			sequence[17] = '\0';
			sequence[ 0] =  'n';

			//coordinates
			int2hex(sequence+1, S2DE_mouseX);
			int2hex(sequence+9, S2DE_mouseY);

			//send sequence
			bipipe_write(com_bipipe, sequence);
			free(sequence);
			#endif
		break;



		//timed execution
		case S2DE_TIMER:
			S2DE_refresh();
		break;



		//resize
		case S2DE_RESIZE:
			sequence     = malloc(18); //z0000000011111111\0
			sequence[17] = '\0';
			sequence[ 0] = 'z';

			//dimensions
			int2hex(sequence+1, S2DE_newWidth);
			int2hex(sequence+9, S2DE_newHeight);

			//send sequence
			bipipe_write(com_bipipe, sequence);
			free(sequence);
		break;
	}
}



//run S2DE main loop
void* S2DE_mainLoop(void* args){

	//set constant periodic timer (must be expressed in ms)
	S2DE_setTimer(1000.f/DISPLAY_FPS);

	//launch S2DE
	S2DE_start();

	return NULL;
}



//analyze incomming instructions
void analyzeIncomming(){

	//get global vars
	queue*  display_objects = displayObjects(GET, NULL);
	bipipe* com_bipipe      = comBipipe(GET, NULL);

	//S2DE main loop
	thread* S2DE_mainLoop_thread;
	bool    S2DE_mainLoop_started = false;

	//receive data
	char*   reception;
	size_t  reception_len;
	while(true){

		//read from bipipe (execution is stuck here, no temporization required)
		reception = bipipe_read(com_bipipe);
		reception_len = strlen(reception);

		//read reception
		size_t r = 0;
		while(r < reception_len){
			switch(reception[r]){



				// START - STOP

				//start graphical loop
				case 'i':

					//S2DE main loop already started
					if(S2DE_mainLoop_started){
						//fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : S2DE main loop has already started.\n", stderr);
					}

					//start S2DE main loop
					else{

						//read arguments
						if(reception_len < r+17){ //bytes required in sequence i0000000011111111 (17)
							fputs("RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : Missing elements in sequence 'i'.\n", stderr);
							continue;
						}
						int width  = hex2int(reception+1);
						int height = hex2int(reception+9);

						//title : get delimiter index
						size_t title_startIndex = r+17;
						size_t title_endIndex   = r+17;
						while(title_endIndex < reception_len){
							if(reception[title_endIndex] == '`'){ break; }
							title_endIndex++;
						}

						//title : no delimiter index in given sequence
						if(title_endIndex == reception_len){
							fputs("RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : No delimiter index in bipipe sequence.\n",     stderr);
							fputs("                                                     Maybe you should use a shorter title name.\n", stderr);
							continue;
						}

						//allocate title
						size_t title_len = title_endIndex - title_startIndex;
						char*  title     = malloc(title_len+1);
						title[title_len] = '\0';
						memcpy(title, reception+title_startIndex, title_len);

						//neutralize argc/argv for S2DE_init()
						int    n_argc   = 0;
						char*  n_argv_p = NULL;
						char** n_argv   = &n_argv_p;

						//init S2DE
						S2DE_init(n_argc,n_argv, title, width,height);

						//start main loop
						S2DE_mainLoop_thread = thread_create(S2DE_mainLoop);
						thread_start(S2DE_mainLoop_thread);
						S2DE_mainLoop_started = true;
					}

				break;

				//end graphic loop
				case 'I':

					//stop S2DE main loop
					if(S2DE_mainLoop_started){ thread_stop(S2DE_mainLoop_thread, THREAD__INTERRUPT); }

					//S2DE main loop not started yet
					else{
						//fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : S2DE main loop has already started.\n", stderr);
					}
				break;



				// DISPLAY

				//set background / front color
				case 'c':	case 'C':
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 7);
					}
				break;

				//draw point
				case 'p':
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 17);
					}
				break;

				//draw line / empty rectangle / full rectangle
				case 'l':
				case 'r':	case 'R':
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 33);
					}
				break;

				//draw empty triangle / full triangle
				case 't':	case 'T':
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 49);
					}
				break;

				//draw empty quad / full quad
				case 'q':	case 'Q':
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 65);
					}
				break;

				//undefined sequence
				default:
					#ifdef VERBOSE_INVALID_CHARACTERS
					fprintf(stderr, "RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : Invalid character '%c'.\n", reception[r])
					#endif
				break;
			}
		}

		//free reception string
		free(reception);
	}
}



//main
int main(int argc, char** argv){

	//missing arguments
	if(argc == 1){
		fputs("FATAL ERROR > S2DE_bipipe.c : main() : Missing argument in first position (peer application name).\n", stderr);
		return EXIT_FAILURE;
	}



	// PEER SUBPROCESS

	//create global display_objects
	displayObjects(SET, queue_create());

	//create global communication bipipe
	bipipe* com_bipipe = comBipipe(SET, bipipe_create(BIPIPE_LENGTH)); //get & set at the same time

	//reformat arguments to fit with peer
	char* peer_fullPath = argv[1];
	argv[0] = basename(peer_fullPath);
	argv[1] = com_bipipe->info;

	//create peer subprocess
	proc* peer_subprocess = peerSubprocess(SET, proc_create(peer_fullPath, argv)); //get & set at the same time

	//launch subprocess (the one who will exchange instructions)
	proc_start(peer_subprocess);
	usleep(PEER_TEMPORIZATION);

	//now close the correct bipipes gates as we are the PARENT process
	bipipe_everybodyJoined(com_bipipe);



	// CURRENT EXECUTION

	//run analyze loop
	analyzeIncomming();

	return EXIT_SUCCESS;
}
