// ---------------- IMPORTATIONS ----------------

//standard
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

//queues
#include "../lib/queues.h"

//threads
#include "../lib/threads.h"

//graphics
#include "../lib/S2DE.h" //2D engine






/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ S2DE pipe [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Developped using S2DE :

        This application is meant to execute S2DE functions (2D graphical
    interactions) from instructions that will be exchanged using a pipe.
    Therefore, this program requires a peer application for sending
    graphical orders. You can also send graphical orders yourself using a
    terminal.

        A better way to use this program is to pipe it with another program
    which role is to give graphicals orders as text in its regular stdout.
    Because only one pipe is communicating between S2DE_pipe and the main
    application, events will not be transfered back to the main app.

    ./myAppSendingOrders | ./S2DE_pipe

        Instructions communicating through the pipe are simple ASCII text
    sequences :

      - S2DE_pipe will receive orders :
        "i0000000011111111<title_name>`"                                    : Initialize window with w,h on 32b & title until '`' found
        "I"                                                                 : Close window
        "o00110011"                                                         : Set background color with r,g,b,a on 4*8b
        "O00110011"                                                         : Set front color with r,g,b,a on 4*8b
        "p00000000111111110000000011111111"                                 : Draw point with x,y on 32b
        "l00000000111111110000000011111111"                                 : Draw line with x1,y1,x2,y2
        "t000000001111111100000000111111110000000011111111"                 : Draw empty triangle with x1,y1,x2,y2,x3,y3 on 32b
        "T000000001111111100000000111111110000000011111111"                 : Same thing but filled
        "r00000000111111110000000011111111"                                 : Draw empty rectangle with x1,y1,x2,y2 on 32b
        "R00000000111111110000000011111111"                                 : Same thing but filled
        "q0000000011111111000000001111111100000000111111110000000011111111" : Draw empty quad with x1,y1,x2,y2,x3,y3,x4,y4 on 32b
        "Q0000000011111111000000001111111100000000111111110000000011111111" : Same thing but filled
        "h"                                                                 : Reset display elements queue

      - S2DE_pipe will NOT send events.

    Any other sequence will be skipped.

        Mind that data is expressed in hexadecimal so if you want to add your
    own instructions, please don't use minimal letters from 'a' to 'f'.

    09/03/2024 > [0.1.0] :
    - Created S2DE_pipe.c from S2DE_bipipe.c.

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

//delays
#define DISPLAY_FPS 10

//global variables
#define GET false
#define SET true
#define RECEPTION_LENGTH_MAX       128
#define GRAPHIC_INIT_TEMPORIZATION 100000 // (in us)
#define DISPLAY_OBJECTS_MAX        50
bool    graphic_mainLoop_started = false; //graphic main loop indicator

//additionnal options
#define VERBOSE_INVALID_CHARACTERS






// ---------------- GETTER - SETTERS ----------------

//global variables
queue* displayObjects(bool action, queue* value){
	static queue* display_objects = NULL;
	if(action){ display_objects = value; } //set
	return display_objects;                //get
}






// ---------------- TOOLS ----------------

//conversions
char halfbyte2hex(unsigned int v){
	if(v > 0xf){
		fprintf(stderr, "RUNTIME ERROR > S2DE_pipe.c : halfbyte2hex() : Invalid half-byte value '%d'.\n", v);
		return '0';
	}

	//regular case
	if(v < 0xa){ return '0' + v; }
	return 'a' + (v-0xa);
}

void short2hex(char* result, unsigned short s){
	result[0] = halfbyte2hex( (s >> 12) & 0xf);
	result[1] = halfbyte2hex( (s >>  8) & 0xf);
	result[2] = halfbyte2hex( (s >>  4) & 0xf);
	result[3] = halfbyte2hex(  s        & 0xf);
}

void int2hex(char* result, unsigned int i){
	result[0] = halfbyte2hex( (i >> 28) & 0xf);
	result[1] = halfbyte2hex( (i >> 24) & 0xf);
	result[2] = halfbyte2hex( (i >> 20) & 0xf);
	result[3] = halfbyte2hex( (i >> 16) & 0xf);
	result[4] = halfbyte2hex( (i >> 12) & 0xf);
	result[5] = halfbyte2hex( (i >>  8) & 0xf);
	result[6] = halfbyte2hex( (i >>  4) & 0xf);
	result[7] = halfbyte2hex(  i        & 0xf);
}

unsigned int hex2halfbyte(char hex){
	if(hex < '0' || hex > 'f' || (hex > '9' && hex < 'a')){
		fprintf(stderr, "RUNTIME ERROR > S2DE_pipe.c : hex2halfbyte() : Invalid hex character '%c'.\n", hex);
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

//draw from given instruction
void draw(char* sequence){
	unsigned char r,g,b,a;
	unsigned int  x1,y1, x2,y2, x3,y3, x4,y4;
	switch(sequence[0]){

		//background color
		case 'o':
			r = hex2byte(sequence+1);	g = hex2byte(sequence+3);	b = hex2byte(sequence+5);	a = hex2byte(sequence+7);
			S2DE_setBackColor(r,g,b,a);
		break;

		//front color
		case 'O':
			r = hex2byte(sequence+1);	g = hex2byte(sequence+3);	b = hex2byte(sequence+5);	a = hex2byte(sequence+7);
			S2DE_setFrontColor(r,g,b,a);
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
			fprintf(stderr, "INTERNAL ERROR > S2DE_pipe.c : draw() : Unknown instruction '%c'.\n", sequence[0]);
	}
}



//events
void S2DE_event(int event){
	queue* display_objects = displayObjects(GET, NULL);

	//redirect event (care about display only here)
	switch(event){
		case S2DE_DISPLAY:
			for(size_t q=0; q < display_objects->length; q++){
				draw( queue_get(display_objects, q) );
			}
		break;
	}
}



//run graphic main loop
void* graphic_mainLoop(void* args){
	int*   int_args      = args;
	char** title_address = (char**)(int_args+2);

	//neutralize argc/argv for S2DE_init()
	int    n_argc   = 0;
	char*  n_argv_p = NULL;
	char** n_argv   = &n_argv_p;


	//init S2DE                                ---width--- --height---
	S2DE_init(n_argc,n_argv, title_address[0], int_args[0],int_args[1]);

	//set constant periodic timer (must be expressed in ms)
	S2DE_setTimer(1000.f/DISPLAY_FPS);

	//launch S2DE main loop
	S2DE_start();

	return NULL;
}



//analyze incomming instructions
void analyzeIncomming(){

	//get global vars
	queue*  display_objects = displayObjects(GET, NULL);

	//receive data
	char*   reception = malloc(RECEPTION_LENGTH_MAX+1);
	size_t  reception_len;
	while(true){

		//read from stdin
		reception_len = 0;
		while(true){
			int c = fgetc(stdin);
			if(c == EOF || c == '\n' || reception_len == RECEPTION_LENGTH_MAX-1){ break; }
			reception[reception_len] = c;
			reception_len++;
		}
		reception[reception_len] = '\0';
		//printf("reception_len[%i]\n", reception_len);
		//printf("reception[%s]\n", reception);

		//read reception
		size_t r = 0;
		while(r < reception_len){
			switch(reception[r]){



				// START - STOP

				//start graphical loop
				case 'i':

					//graphic main loop already started
					if(graphic_mainLoop_started){
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : S2DE main loop has already started.\n", stderr);
					}

					//start S2DE main loop
					else{

						//check length
						if(reception_len < r+17){ //bytes required in sequence i0000000011111111 (17)
							fputs("RUNTIME ERROR > S2DE_pipe.c : analyzeIncomming() : Missing elements in sequence 'i'.\n", stderr);
							break;
						}

						//prepare graphic main loop arguments
						int* graphic_mainLoop_args = malloc(2*sizeof(int) + sizeof(char*));
						graphic_mainLoop_args[0] = hex2int(reception+r+1); //width
						graphic_mainLoop_args[1] = hex2int(reception+r+9); //height

						//title : get delimiter index
						size_t title_startIndex = r+17;
						size_t title_endIndex   = r+17;
						while(title_endIndex < reception_len){
							if(reception[title_endIndex] == '`'){ break; }
							title_endIndex++;
						}

						//title : no delimiter index in given sequence
						if(title_endIndex == reception_len){
							fputs("RUNTIME ERROR > S2DE_pipe.c : analyzeIncomming() : No delimiter index in pipe sequence.\n",     stderr);
							fputs("                                                   Maybe you should use a shorter title name.\n", stderr);
							break;
						}

						//allocate title
						size_t title_len = title_endIndex - title_startIndex;
						char*  title     = malloc(title_len+1);
						title[title_len] = '\0';
						memcpy(title, reception+title_startIndex, title_len);

						//attached title to args (after the 2 first int)
						char** title_embedded_location = (char**)(graphic_mainLoop_args+2);
						title_embedded_location[0] = title;

						//start main loop
						thread* graphic_mainLoop_thread = thread_create(graphic_mainLoop, graphic_mainLoop_args);
						thread_start(graphic_mainLoop_thread);
						graphic_mainLoop_started = true;

						//graphic init temporization
						usleep(GRAPHIC_INIT_TEMPORIZATION);

						//shift reading head
						r += title_endIndex;
					}
				break;

				//end graphic loop
				case 'I':

					//end signal reached => stop all
					if(graphic_mainLoop_started){ exit(EXIT_SUCCESS); }

					//graphic main loop not started yet
					else{
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Graphic main loop has not started yet.\n", stderr);
					}
				break;



				// DISPLAY

				//set background / front color
				case 'o':	case 'O':

					//check length
					if(reception_len < r+9){ //bytes required in sequence o00110011 (9)
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Missing elements in sequence 'o/O'.\n", stderr);
						break;
					}

					//add to drawing queue
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 9);
					}

					//shift reading head
					r += 9;
				break;

				//draw point
				case 'p':

					//check length
					if(reception_len < r+17){ //bytes required in sequence p0000000011111111 (17)
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Missing elements in sequence 'p'.\n", stderr);
						break;
					}

					//add to drawing queue
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 17);
					}

					//shift reading head
					r += 17;
				break;

				//draw line / empty rectangle / full rectangle
				case 'l':
				case 'r':	case 'R':

					//check length
					if(reception_len < r+33){ //bytes required in sequence l00000000111111110000000011111111 (33)
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Missing elements in sequence 'l/r/R'.\n", stderr);
						break;
					}

					//add to drawing queue
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 33);
					}

					//shift reading head
					r += 33;
				break;

				//draw empty triangle / full triangle
				case 't':	case 'T':

					//check length
					if(reception_len < r+49){ //bytes required in sequence t000000001111111100000000111111110000000011111111 (49)
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Missing elements in sequence 't/T'.\n", stderr);
						break;
					}

					//add to drawing queue
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 49);
					}

					//shift reading head
					r += 49;
				break;

				//draw empty quad / full quad
				case 'q':	case 'Q':

					//check length
					if(reception_len < r+65){ //bytes required in sequence q0000000011111111000000001111111100000000111111110000000011111111 (65)
						fputs("RUNTIME ERROR > S2DE_pipe.c : analizeIncomming() : Missing elements to sequence 'q/Q'.\n", stderr);
						break;
					}

					//add to drawing queue
					if(display_objects->length < DISPLAY_OBJECTS_MAX){
						queue_append(display_objects, reception+r, 65);
					}

					//shift reading head
					r += 65;
				break;

				//reset display elements
				case 'h':
					queue_clear(display_objects); //reset drawing queue
				break;

				//undefined sequence
				default:
					#ifdef VERBOSE_INVALID_CHARACTERS
					fprintf(stderr, "RUNTIME ERROR > S2DE_pipe.c : analyzeIncomming() : Invalid character '%c'.\n", reception[r]);
					#endif
				break;
			}

			//increase reading head
			r++;
		}
	}

	//free reception string
	free(reception);
}



//main
int main(int argc, char** argv){

	//create global display_objects
	displayObjects(SET, queue_create());

	//run analyze loop
	analyzeIncomming();

	return EXIT_SUCCESS;
}
