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

// ---- hashed images ----
int littleH_width  = 250;
int littleH_height = 120;
int* littleH_data = NULL;
int bigH_width  = 50;
int bigH_height = 300;
int* bigH_data = NULL;












// ---------------- EXECUTION ----------------

// ---- event handlers ----
void S2DE_timedExecution(){
	S2DE_refresh();
}

void S2DE_display(){
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
}

void S2DE_keyPressed(char key){
	switch(key){
		case 'z':
		break;
		case 'q':
		break;
		case 's':
		break;
		case 'd':
		break;
		case S2DE_KEY_UP:
		break;
		case S2DE_KEY_DOWN:
		break;
		case S2DE_KEY_LEFT:
		break;
		case S2DE_KEY_RIGHT:
		break;
	}
	printf("Pressing key '%c'\n", key);
}

void S2DE_mousePressed(int x,int y, int state){
	switch(state){
		case S2DE_LEFT_PRESSED:
			printf("Left clicking at (%i,%i)\n", x,y);
		break;
		case S2DE_MIDDLE_PRESSED:
			printf("Middle clicking at (%i,%i)\n", x,y);
		break;
		case S2DE_RIGHT_PRESSED:
			printf("Right clicking at (%i,%i)\n", x,y);
		break;
	}
}

void S2DE_mouseMoved(int x,int y){
	printf("Mouse is moving passively to (%i,%i)\n", x,y);
}

void S2DE_reshape(int newWidth,int newHeight){
	printf("Reshaping window to %ix%i\n", newWidth,newHeight);
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
	S2DE_setTimedExecution(100);

	//launch S2DE
	printf("Starting S2DE [0.1.1]\n");
	S2DE_start();

	return 0;
}

