// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



//graphic engine
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <glut.h>
#endif



//own header
#include "S2DE.h"












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S2DE [0.1.6] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Simple 2Dimensional Engine

    Developped using freeglut3 (or just GLUT), a graphical 2D/3D engine.

    To install glut on linux-apt systems, just install the package :
                               freeglut3-dev

        This engine is only a simple way to access to graphical
    fuctionnalities provided by the GLUT C library itself.
        S2DE is not producing any graphical improvements on GLUT.
    However, this program is made in order to give a much more SIMPLE
    access to all the basic functionnalities of GLUT.

        Note that GLUT was not made to be SIMPLE but rather EDITABLE.
    The main purpose of S2DE is to give an easy way for anybody to create
    graphical contents in a few tiny lines.
        This might not be the case for you so if you are doing something that
    needs some more precise graphical configurations and you are not afraid
    about lots of settings, you should rather use the original GLUT functions.

    This library was inspired by Ghislain OUDINET's GfxLib (coded in C,
    using GLUT) and Processing 3 (for codes in Java).

    16/07/2020 > [0.1.0] :
    - Creating the whole engine (functions, constants and default program).

    31/07/2020 > [0.1.1] :
    - Added S2DE_imageRGBA() (data must be in RGBA format, 4 bytes per pixel).
    - Added S2DE_setPixelRGBA(). Useful to create images manually.

    03/08/2020 > [0.1.2] :
    - Added error messages in S2DE_text() and S2DE_init().

    10/09/2020 > [0.1.3] :
    - Changed event handling system : Passed from one function per event type
      to only one function with event id.
    - Remapped all keyboard keys.
    - Added detection of released events.
    - Removed unecessary libraries.

    22/11/2020 > [0.1.4] :
    - Fixed bug : diagonals appeared in rectangles and quads.

    08/12/2020 > [0.1.5] :
    - Fixed bug : Mouse Y coordinate is inverted.
    - Added external variables S2DE_width & S2DE_height.
    - Modified S2DEL_reshape() :
      Now, S2DE_newWidth & S2DE_newHeight are set before
      S2DE_RESHAPE event, and then
      S2DE_width & S2DE_height are set.

    22/03/2021 > [0.1.6] :
    - Added S2DE_MOUSE_SCROLL event.

    BUGS : .
    NOTES : S2DE is now compatible with another library I made
    for PNG image manipulation : PNG.c/.h.
    It is available here :
            https://github.com/iasebsil83/C_PNG
    Use S2DE_imageRGBA() to draw image on screen.


    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    S2DE
    Copyright (C) 2020  Sebastien SILVANO
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.

    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- INITIALISATION ----------------

//window
static int S2DE_window               = -1;
static int S2DE_timedExecution_delay = -1;



//event variables
int S2DE_mouseState         = 0; //mouse
int S2DE_mouseButton        = 0;
int S2DE_mouseScroll        = 0;
int S2DE_mouseX             = 0;
int S2DE_mouseY             = 0;
int S2DE_keyState           = 0; //keyboard
unsigned short S2DE_key     = 0;
unsigned int S2DE_newWidth  = 0; //resize
unsigned int S2DE_newHeight = 0;
unsigned int S2DE_width     = 0;
unsigned int S2DE_height    = 0;



//event handler
extern void S2DE_event(int event);












// ---------------- LOCAL EVENT HANDLERS (S2DEL) ----------------

//timed executions
static void S2DEL_timedExecution(int i){
	if(i < 0)
		S2DE_refresh();
	else{
		if(S2DE_timedExecution_delay >= 0){
			glutTimerFunc(
				(unsigned int)S2DE_timedExecution_delay,
				S2DEL_timedExecution,
				S2DE_timedExecution_delay
			);
			S2DE_event(S2DE_TIMER);
		}
	}
}



//display
static void S2DEL_display(){
	glFlush();
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S2DE_event(S2DE_DISPLAY);
	glutSwapBuffers();
}



//keyboard
static void S2DEL_keyPressed(GLubyte g, int x,int y){
	S2DE_key = g;
	S2DE_keyState = S2DE_KEY_PRESSED;
	S2DE_event(S2DE_KEYBOARD);
}

static void S2DEL_keyPressed_special(int keyCode, int x,int y){
	S2DE_key = 256 + (unsigned char)keyCode;
	S2DE_keyState = S2DE_KEY_PRESSED;
	S2DE_event(S2DE_KEYBOARD);
}

static void S2DEL_keyReleased(GLubyte g, int x,int y){
	S2DE_key = g;
	S2DE_keyState = S2DE_KEY_RELEASED;
	S2DE_event(S2DE_KEYBOARD);
}

static void S2DEL_keyReleased_special(int keyCode, int x,int y){
	S2DE_key = 256 + (unsigned char)keyCode;
	S2DE_keyState = S2DE_KEY_RELEASED;
	S2DE_event(S2DE_KEYBOARD);
}



//mouse
static void S2DEL_mouseButton(int button, int state, int x,int y){
	S2DE_mouseX = x;
	S2DE_mouseY = S2DE_height - y;
	S2DE_mouseState = state;

	//scroll
	if(button == 3 || button == 4){
		if(state == S2DE_MOUSE_PRESSED){
			S2DE_mouseScroll = button;
			S2DE_event(S2DE_MOUSE_SCROLL);
		}
	}else{
		S2DE_mouseButton = button;
		S2DE_event(S2DE_MOUSE_CLICK);
	}
}

static void S2DEL_mouseMoved(int x,int y){
	S2DE_mouseX = x;
	S2DE_mouseY = S2DE_height - y;
	usleep(1);
	if(!S2DE_mouseScroll){
		S2DE_event(S2DE_MOUSE_MOVE);
	}else{
		S2DE_mouseScroll = 0;
	}
}



//window reshape
static void S2DEL_reshape(int width,int height){
	glViewport(0,0, width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f,(GLdouble)width, 0.f,(GLdouble)height, -1.f,1.f);
	glMatrixMode(GL_MODELVIEW);
	S2DE_newWidth  = width;
	S2DE_newHeight = height;
	S2DE_event(S2DE_RESIZE);
	S2DE_width  = width;
	S2DE_height = height;
}












// ---------------- UTILITIES ----------------

//useful
void S2DE_refresh(){
	glutPostRedisplay();
}

void S2DE_fullScreen(){
	glutFullScreen();
}

void S2DE_setColor(int r, int v, int b){
	glColor3f(r/255.f, v/255.f, b/255.f);
}

void S2DE_setThickness(float thickness){
	glPointSize(thickness);
	glLineWidth(thickness);
}



//graphics
void S2DE_point(float x, float y){
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}

void S2DE_line(float x1,float y1, float x2,float y2){
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}

void S2DE_triangle(float x1,float y1, float x2,float y2, float x3,float y3, int filled){
	if(filled){
		glBegin(GL_TRIANGLES);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
		glEnd();
	}else{
		S2DE_line(x1,y1, x2,y2);
		S2DE_line(x2,y2, x3,y3);
		S2DE_line(x3,y3, x1,y1);
	}
}

void S2DE_rectangle(float x1,float y1, float x2,float y2, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y1);
			glVertex2f(x2,y2);
			glVertex2f(x1,y2);
		glEnd();

		//GPU weakness : remove diagonals
		float thickness;
		glGetFloatv(GL_LINE_WIDTH, &thickness);
		glLineWidth(1.f);
		S2DE_line(x1,y1, x2,y2);
		glLineWidth(thickness);
	}else{
		S2DE_line(x1,y1, x2,y1);
		S2DE_line(x2,y1, x2,y2);
		S2DE_line(x2,y2, x1,y2);
		S2DE_line(x1,y2, x1,y1);
	}
}

void S2DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
			glVertex2f(x4,y4);
		glEnd();

		//GPU weakness : remove diagonals
		float thickness;
		glGetFloatv(GL_LINE_WIDTH, &thickness);
		glLineWidth(1.f);
		S2DE_line(x1,y1, x3,y3);
		glLineWidth(thickness);
	}else{
		S2DE_line(x1,y1, x2,y2);
		S2DE_line(x2,y2, x3,y3);
		S2DE_line(x3,y3, x4,y4);
		S2DE_line(x4,y4, x1,y1);
	}
}



//text
void S2DE_text(char* text, float size, float x,float y){

	//moving over display (use of the 3rd dimension)
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(size,size,size);

	//error case
	if(text == NULL){
		printf("RUNTIME ERROR > S2DE.c : S2DE_text() : Text is NULL.\n");
		return;
	}

	//write text character per character
	while(*text)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *text++);
	glPopMatrix();
}



//images
void S2DE_imageRGBA(int x,int y, int width,int height, int* data){
	glRasterPos2i(x,y);
	glDrawPixels(width,height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
}

int S2DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	return (r << 24) + (g << 16) + (b <<  8) + a;
}



//timed executions
void S2DE_setTimer(int ms){
	if(S2DE_timedExecution_delay < 0 && ms >= 0)
		glutTimerFunc(
			(unsigned int)ms,
			S2DEL_timedExecution,
			ms
		);

	//set new timedExecution delay
	S2DE_timedExecution_delay = ms;
}












// ---------------- BASICS -----------------

//init
void S2DE_init(int argc, char** argv, const char* name, unsigned int width,unsigned int height){

	//error case
	if(name == NULL){
		printf("RUNTIME ERROR > S2DE.c : S2DE_init() : Cannot init window, name is NULL.\n");
		return;
	}

	//init attributes
	S2DE_width  = width;
	S2DE_height = height;

	//init window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(S2DE_width, S2DE_height);
	S2DE_window = glutCreateWindow(name);

	//some settings
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_NORMALIZE);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glPointSize(2.f);

	//set local S2DE event handlers (S2DEL)
	glutDisplayFunc      (S2DEL_display            );
	glutKeyboardFunc     (S2DEL_keyPressed         );
	glutSpecialFunc      (S2DEL_keyPressed_special );
	glutKeyboardUpFunc   (S2DEL_keyReleased        );
	glutSpecialUpFunc    (S2DEL_keyReleased_special);
	glutMouseFunc        (S2DEL_mouseButton        );
	glutMotionFunc       (S2DEL_mouseMoved         );
	glutPassiveMotionFunc(S2DEL_mouseMoved         );
	glutReshapeFunc      (S2DEL_reshape            );
}



//start - stop
void S2DE_start(){
	glutMainLoop();
}

void S2DE_stop(){
	glutDestroyWindow(S2DE_window);
	exit(EXIT_SUCCESS);
}
