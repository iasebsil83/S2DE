// ---------------- IMPORTATIONS ----------------

//standard
#include <stdlib.h>
#include <stdio.h>



//graphics
#include "../lib/S2DE.h" //2D motor












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












// ---------------- EXECUTION ----------------

// ---- events ----
void S2DE_event(int event){
	switch(event){

		//display
		case S2DE_DISPLAY:
			//background
			S2DE_setColor(255,255,255);
			S2DE_rectangle(0,0, S2DE_width,S2DE_height, 1);

			//blue filled rectangle at the bottom left corner
			S2DE_setColor(  0,  0,255);
			S2DE_rectangle(100,100, 200,200, 1);

			//red unfilled rectangle at the top right corner
			S2DE_setColor(255,  0,  0);
			S2DE_rectangle(
				S2DE_width-100, S2DE_height-100,
				S2DE_width-200, S2DE_height-200,
				0
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
		case S2DE_MOUSE_CLICK:
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



		//mouse scroll
		case S2DE_MOUSE_SCROLL:
			switch(S2DE_mouseScroll){
				case S2DE_SCROLL_UP:
					printf("Scrolling up at (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
				break;
				case S2DE_SCROLL_DOWN:
					printf("Scrolling down at (%i,%i)\n", S2DE_mouseX,S2DE_mouseY);
				break;
			}
		break;



		//mouse move
		case S2DE_MOUSE_MOVE:
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
int main(int argc, char** argv){

	//init S2DE
	S2DE_init(argc,argv, "Program Name [V.V.V]", 700,700);
	S2DE_setTimer(100);

	//launch S2DE
	printf("Starting S2DE [0.1.6]\n");
	S2DE_start();

	return EXIT_SUCCESS;
}
