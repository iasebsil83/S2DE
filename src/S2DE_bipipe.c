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

        This application should NOT be executed as standalone but rather the
    peer application must be executed with S2DE_bipipe as first argument.
    Actually, you will have to give the full path of S2DE_bipipe to allow
    your peer application to locate where to find S2DE_bipipe. Moreover, any
    argument given to your peer application will be transferred to S2DE_bipipe
    as well.

    Instead of having :
        myApp arg1 arg2...

    We will have :
        myApp /path/to/S2DE_bipipe arg1 arg2...

        Instructions communicating through the bipipe are simple ASCII text
    sequences :

      - S2DE_bipipe will receive orders :
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

        Mind that data is expressed in hexadecimal so if you want to add your
    own instructions, please don't use minimal letters from 'a' to 'f'.

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

//delays
#define GRAPHIC_INIT_TEMPORIZATION 100000 // (in us)
#define DISPLAY_FPS                    10

//global variables
#define GET false
#define SET true
#define DISPLAY_OBJECTS_MAX 50
bool    graphic_mainLoop_started = false; //graphic main loop indicator

//additionnal option (mouse move can overwhelm other events)
//#define IGNORE_MOUSE_MOVE
#define VERBOSE_INVALID_CHARACTERS






// ---------------- GETTER - SETTERS ----------------

//global variables
bipipe* comBipipe(bool action, bipipe* value){
	static bipipe* com_bipipe = NULL;
	if(action){ com_bipipe = value; } //set
	return com_bipipe;                //get
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
		fprintf(stderr, "RUNTIME ERROR > S2DE_bipipe.c : hex2halfbyte() : Invalid hex character '%c'.\n", hex);
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
			//printf("BACK seq[");
			//for(int z=0; z < 8; z++){ printf("%c", sequence[1+z]); }
			//printf("] r[%02x] g[%02x] b[%02x] a[%02x]\n", r&0xff, g&0xff, b&0xff, a&0xff);
			S2DE_setBackColor(r,g,b,a);
		break;

		//front color
		case 'O':
			r = hex2byte(sequence+1);	g = hex2byte(sequence+3);	b = hex2byte(sequence+5);	a = hex2byte(sequence+7);
			//printf("FRONT seq[");
			//for(int z=0; z < 8; z++){ printf("%c", sequence[1+z]); }
			//printf("] r[%02x] g[%02x] b[%02x] a[%02x]\n", r&0xff, g&0xff, b&0xff, a&0xff);
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
			//printf("DISPLAY\n");

			//draw each object
			for(size_t q=0; q < display_objects->length; q++){
				draw( queue_get(display_objects, q) );
			}
		break;



		//keyboard
		case S2DE_KEYBOARD:
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
	bipipe* com_bipipe      = comBipipe(GET, NULL);

	//receive data
	char*   reception;
	size_t  reception_len;
	while(true){

		//read from bipipe (execution is stuck here, no temporization required)
		reception = bipipe_read(com_bipipe);
		reception_len = strlen(reception);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : S2DE main loop has already started.\n", stderr);
					}

					//start S2DE main loop
					else{

						//check length
						if(reception_len < r+17){ //bytes required in sequence i0000000011111111 (17)
							fputs("RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : Missing elements in sequence 'i'.\n", stderr);
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
							fputs("RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : No delimiter index in bipipe sequence.\n",     stderr);
							fputs("                                                     Maybe you should use a shorter title name.\n", stderr);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Graphic main loop has not started yet.\n", stderr);
					}
				break;



				// DISPLAY

				//set background / front color
				case 'o':	case 'O':

					//check length
					if(reception_len < r+9){ //bytes required in sequence o00110011 (9)
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Missing elements in sequence 'o/O'.\n", stderr);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Missing elements in sequence 'p'.\n", stderr);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Missing elements in sequence 'l/r/R'.\n", stderr);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Missing elements in sequence 't/T'.\n", stderr);
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
						fputs("RUNTIME ERROR > S2DE_bipipe.c : analizeIncomming() : Missing elements to sequence 'q/Q'.\n", stderr);
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
					fprintf(stderr, "RUNTIME ERROR > S2DE_bipipe.c : analyzeIncomming() : Invalid character '%c'.\n", reception[r]);
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

	//missing arguments
	if(argc == 1){
		fputs("FATAL ERROR > S2DE_bipipe.c : main() : Missing bipipe info at first position.\n", stderr);
		return EXIT_FAILURE;
	}

	//connect to bipipe using given info
	comBipipe(SET, bipipe_join(argv[1])); //get & set at the same time

	//create global display_objects
	displayObjects(SET, queue_create());

	//run analyze loop
	analyzeIncomming();

	return EXIT_SUCCESS;
}
