// ---------------- IMPORTATIONS ----------------

//standard
#include <stdlib.h>
#include <stdio.h>



//graphics
#include "S2DE.h" //2D motor












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prog [V.V.V] ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Developped using S2DE :

    Blablabla about the project.

    DD/MM/YYYY > [V.V.V] :
    - Added something.
    - Added something else.

    DD/MM/YYYY > [V.V.V] :
        - Added something that wasn't before.
        - Fixed a bug.
        - Added something else.
        - Added something else again.

    BUGS : Active bugs in last version
    NOTES : Notes

    Contact : ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- DECLARATIONS ----------------

// ---- window ----
const int maxl = 700;	const int maxl_2 = 350;
const int maxh = 700;	const int maxh_2 = 350;



//event variables
extern int S2DE_mouseState; //mouse
extern int S2DE_mouseButton;
extern int S2DE_mouseX;
extern int S2DE_mouseY;
extern int S2DE_keyState; //keyboard
extern short S2DE_key;
extern int S2DE_newWidth; //resize
extern int S2DE_newHeight;



// ---- hashed images ----
int littleH_width  = 250;
int littleH_height = 120;
int* littleH_data = NULL;
int bigH_width  = 50;
int bigH_height = 300;
int* bigH_data = NULL;












// ---------------- EXECUTION ----------------

// ---- events ----
void S2DE_event(int event){
	switch(event){

		//display
		case S2DE_DISPLAY:
			//background
			S2DE_setColor(255,255,255);
			S2DE_rectangle(0,0, maxl,maxh, 1);

			//blue filled rectangle at the bottom left corner
			S2DE_setColor(  0,  0,255);
			S2DE_rectangle(100,100, 200,200, 1);

			//red unfilled rectangle at the top right corner
			S2DE_setColor(255,  0,  0);
			S2DE_rectangle(maxl-100,maxh-100, maxl-200,maxh-200, 0);

			//littleH
			S2DE_imageRGBA(
				maxl_2,maxh_2,
				littleH_width,littleH_height,
				littleH_data
			);

			//bigH
			S2DE_imageRGBA(
				maxl_2/2,maxh_2,
				bigH_width,bigH_height,
				bigH_data
			);
		break;



		//keyboard
		case S2DE_KEYBOARD:
			//state
			if(S2DE_keyState == S2DE_KEY_PRESSED)
				printf("KEY_PRESSED : ");
			else
				printf("KEY_RELEASED : ");

			//key
			switch(S2DE_key){
				case S2DE_KEY_ESCAPE:
					exit(EXIT_SUCCESS);
				break;
				case S2DE_KEY_LEFT:
					printf("Left arrow\n");
				break;
				case S2DE_KEY_UP:
					printf("Up arrow\n");
				break;
				case S2DE_KEY_RIGHT:
					printf("Right arrow\n");
				break;
				case S2DE_KEY_DOWN:
					printf("Down arrow\n");
				break;
			}
		break;



		//mouse
		case S2DE_MOUSECLICK:
			//state
			if(S2DE_mouseState == S2DE_MOUSE_PRESSED)
				printf("MOUSE_PRESSED : ");
			else
				printf("MOUSE_RELEASED : ");

			//button
			switch(S2DE_mouseButton){
				case S2DE_LEFT_BUTTON:
					printf("Left button at (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
				break;
				case S2DE_MIDDLE_BUTTON:
					printf("Middle button (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
				break;
				case S2DE_RIGHT_BUTTON:
					printf("Right button at (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
				break;
			}
		break;



		//mouse move
		case S2DE_MOUSEMOVE:
			printf("Mouse is moving passively to (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
		break;



		//timed execution
		case S2DE_TIMER:
			S2DE_refresh();
		break;



		//resize
		case S2DE_RESIZE:
			printf("Reshaping window to %ix%i\n", S2DE_newWidth,S2DE_newHeight);
		break;
	}
}



// ---- main ----
int main(int argc, char **argv){

	//creating littleH
	littleH_data = malloc(littleH_width*littleH_height*4); //RGBA => 4 bytes per pixel
	unsigned char blink;
	int index=0;
	for(int y=0; y < littleH_height; y++){
		for(int x=0; x < littleH_width; x++){
			blink = (x+y)%8 ? 255 : 0;
			littleH_data[index++] = S2DE_setPixelRGBA(blink, blink, blink, 255);
		}
	}

	//creating bigH
	bigH_data = malloc(bigH_width*bigH_height*4); //RGBA => 4 bytes per pixel
	index=0;
	for(int y=0; y < bigH_height; y++){
		for(int x=0; x < bigH_width; x++){
			blink = (x+y)%16 ? 255 : 0;
			bigH_data[index++] = S2DE_setPixelRGBA(blink, blink, 255, 255);
		}
	}

	//init S2DE
	S2DE_init(argc,argv, "Program Name [V.V.V]", maxl,maxh);
	S2DE_setTimer(100);

	//launch S2DE
	printf("Starting S2DE [0.1.3]\n");
	S2DE_start();

	return 0;
}
