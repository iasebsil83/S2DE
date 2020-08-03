// ---------------- IMPORTATIONS ----------------

//compatibility
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/CGLMacro.h>
	#include <GLUT/glut.h>		// Header File For The GLut Library
#else
	#include <glut.h>			// Header File For The GLut Library
	#include <X11/Xlib.h>
	#include <GL/glx.h>
#endif



//standard
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



//own header
#include "S2DE.h"












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S2DE [0.1.2] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- INITIALIZATION ----------------

//global vars
static int S2DE_window = -1;
static int S2DE_timedExecution_delay = -1;



//event handlers
extern void S2DE_timedExecution();
extern void S2DE_display();
extern void S2DE_keyPressed(char key);
extern void S2DE_mousePressed(int x,int y, int state);
extern void S2DE_mouseMoved(int x,int y);
extern void S2DE_reshape(int newWidth,int newHeight);












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
			S2DE_timedExecution();
		}
	}
}



//display
static void S2DEL_display(){
	glFlush();
	glClearColor(0.f,0.f,0.f,0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S2DE_display();
	glutSwapBuffers();
}



//keyboard
static void S2DEL_keyPressed(GLubyte g, int x,int y){
	char c = (char)g;
	switch(c){
		case 8:
			S2DE_keyPressed(S2DE_KEY_BACKSPACE);
		break;
		case 9:
			S2DE_keyPressed(S2DE_KEY_TAB);
		break;
		case 13:
			S2DE_keyPressed(S2DE_KEY_ENTER);
		break;
		case 27:
			S2DE_keyPressed(S2DE_KEY_ESCAPE);
		break;
		case 127:
			S2DE_keyPressed(S2DE_KEY_DELETE);
		break;
		default:
			S2DE_keyPressed(c);
	}
}

static void S2DEL_keyPressed_special(int keyCode, int x,int y){
	switch(keyCode){
		case GLUT_KEY_F1:
			S2DE_keyPressed(S2DE_KEY_F1);
		break;
		case GLUT_KEY_F2:
			S2DE_keyPressed(S2DE_KEY_F2);
		break;
		case GLUT_KEY_F3:
			S2DE_keyPressed(S2DE_KEY_F3);
		break;
		case GLUT_KEY_F4:
			S2DE_keyPressed(S2DE_KEY_F4);
		break;
		case GLUT_KEY_F5:
			S2DE_keyPressed(S2DE_KEY_F5);
		break;
		case GLUT_KEY_F6:
			S2DE_keyPressed(S2DE_KEY_F6);
		break;
		case GLUT_KEY_F7:
			S2DE_keyPressed(S2DE_KEY_F7);
		break;
		case GLUT_KEY_F8:
			S2DE_keyPressed(S2DE_KEY_F8);
		break;
		case GLUT_KEY_F9:
			S2DE_keyPressed(S2DE_KEY_F9);
		break;
		case GLUT_KEY_F10:
			S2DE_keyPressed(S2DE_KEY_F10);
		break;
		case GLUT_KEY_F11:
			S2DE_keyPressed(S2DE_KEY_F11);
		break;
		case GLUT_KEY_F12:
			S2DE_keyPressed(S2DE_KEY_F12);
		break;
		case GLUT_KEY_UP:
			S2DE_keyPressed(S2DE_KEY_UP);
		break;
		case GLUT_KEY_DOWN:
			S2DE_keyPressed(S2DE_KEY_DOWN);
		break;
		case GLUT_KEY_LEFT:
			S2DE_keyPressed(S2DE_KEY_LEFT);
		break;
		case GLUT_KEY_RIGHT:
			S2DE_keyPressed(S2DE_KEY_RIGHT);
		break;
		case GLUT_KEY_PAGE_UP:
			S2DE_keyPressed(S2DE_KEY_PAGE_UP);
		break;
		case GLUT_KEY_PAGE_DOWN:
			S2DE_keyPressed(S2DE_KEY_PAGE_DOWN);
		break;
		case GLUT_KEY_HOME:
			S2DE_keyPressed(S2DE_KEY_HOME);
		break;
		case GLUT_KEY_END:
			S2DE_keyPressed(S2DE_KEY_END);
		break;
		case GLUT_KEY_INSERT:
			S2DE_keyPressed(S2DE_KEY_INSERT);
		break;
		default:
			S2DE_keyPressed(S2DE_KEY_UNDEFINED);
	}
}



//mouse
static void S2DEL_mousePressed(int button, int state, int x,int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
				S2DE_mousePressed(x,y, S2DE_LEFT_PRESSED);
			else
				S2DE_mousePressed(x,y, S2DE_LEFT_RELEASED);
		break;
		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN)
				S2DE_mousePressed(x,y, S2DE_MIDDLE_PRESSED);
			else
				S2DE_mousePressed(x,y, S2DE_MIDDLE_RELEASED);
		break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN)
				S2DE_mousePressed(x,y, S2DE_RIGHT_PRESSED);
			else
				S2DE_mousePressed(x,y, S2DE_RIGHT_RELEASED);
		break;
	}
}

static void S2DEL_mouseMoved(int x,int y){
	S2DE_mouseMoved(x,y);
}

static void S2DEL_mouseMoved_passive(int x,int y){
	S2DE_mouseMoved(x,y);
}



//window reshape
static void S2DEL_reshape(int width,int height){
	glViewport(0,0, width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f,(GLdouble)width, 0.f,(GLdouble)height, -1.f,1.f);
	glMatrixMode(GL_MODELVIEW);
	S2DE_reshape(width,height);
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
		printf("RUNTIME ERROR > S2DE.c : S2DE_text() : ");
		printf("Text is NULL.\n");
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
void S2DE_setTimedExecution(int ms){
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
void S2DE_init(int argc, char** argv, const char* name, int width,int height){
	//error case
	if(name == NULL){
		printf("RUNTIME ERROR > S2DE.c : S2DE_init() : ");
		printf("Cannot init window, name is NULL.\n");
		return;
	}

	//compatibility
	#ifdef __linux__
		int majOGL;
		int minOGL;
		Display *dsp = XOpenDisplay(NULL);
		if(dsp != NULL){
			glXQueryVersion(dsp, &majOGL, &minOGL);
			XCloseDisplay(dsp);
		}
	#endif

	//init window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(width, height);
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
}



//start - stop
void S2DE_start(){
	//set local S2DE event handlers (S2DEL)
	glutDisplayFunc      (S2DEL_display           );
	glutKeyboardFunc     (S2DEL_keyPressed        );
	glutSpecialFunc      (S2DEL_keyPressed_special);
	glutMouseFunc        (S2DEL_mousePressed      );
	glutMotionFunc       (S2DEL_mouseMoved        );
	glutPassiveMotionFunc(S2DEL_mouseMoved_passive);
	glutReshapeFunc      (S2DEL_reshape           );

	//launch event loop
	glutMainLoop();
}

void S2DE_stop(){
	glutDestroyWindow(S2DE_window);
	exit(EXIT_SUCCESS);
}
