// ---------------- DECLARATIONS ----------------

//special key constants
enum S2DE_KEYBOARD{
	S2DE_KEY_F1        =  -1,
	S2DE_KEY_F2        =  -2,
	S2DE_KEY_F3        =  -3,
	S2DE_KEY_F4        =  -4,
	S2DE_KEY_F5        =  -5,
	S2DE_KEY_F6        =  -6,
	S2DE_KEY_F7        =  -7,
	S2DE_KEY_F8        =  -8,
	S2DE_KEY_F9        =  -9,
	S2DE_KEY_F10       = -10,
	S2DE_KEY_F11       = -11,
	S2DE_KEY_F12       = -12,
	S2DE_KEY_UP        = -13,
	S2DE_KEY_DOWN      = -14,
	S2DE_KEY_LEFT      = -15,
	S2DE_KEY_RIGHT     = -16,
	S2DE_KEY_PAGE_UP   = -17,
	S2DE_KEY_PAGE_DOWN = -18,
	S2DE_KEY_HOME      = -19,
	S2DE_KEY_END       = -20,
	S2DE_KEY_INSERT    = -21,
	S2DE_KEY_BACKSPACE = -22,
	S2DE_KEY_TAB       = -23,
	S2DE_KEY_ENTER     = -24,
	S2DE_KEY_ESCAPE    = -25,
	S2DE_KEY_DELETE    = -26,
	S2DE_KEY_UNDEFINED = -27
};



//mouse constants
enum S2DE_MOUSE{
	S2DE_LEFT_PRESSED    = 0,
	S2DE_LEFT_RELEASED   = 1,
	S2DE_MIDDLE_PRESSED  = 2,
	S2DE_MIDDLE_RELEASED = 3,
	S2DE_RIGHT_PRESSED   = 4,
	S2DE_RIGHT_RELEASED  = 5
};








//// ---------------- UTILITIES ----------------

//useful
void S2DE_refresh();
void S2DE_fullScreen();
void S2DE_setColor(int r, int v, int b);
void S2DE_setThickness(float thickness);

//graphics
void S2DE_point(float x, float y);
void S2DE_line(float x1,float y1, float x2,float y2);
void S2DE_triangle(float x1,float y1, float x2,float y2, float x3,float y3, int filled);
void S2DE_rectangle(float x1,float y1, float x2,float y2, int filled);
void S2DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled);

//text
void S2DE_text(char* text, float size, float x,float y);

//timed executions
void S2DE_setTimedExecution(int ms);








// ---------------- BASICS -----------------

//init
void S2DE_init(int argc, char** argv, const char* name, int width,int height);

//start - stop
void S2DE_start();
void S2DE_stop();
