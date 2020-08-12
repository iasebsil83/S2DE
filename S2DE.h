// ---------------- DECLARATIONS ----------------

//special key constants
enum S2DE_KEYBOARD{
	S2DE_KEY_F1        = (char) -1,
	S2DE_KEY_F2        = (char) -2,
	S2DE_KEY_F3        = (char) -3,
	S2DE_KEY_F4        = (char) -4,
	S2DE_KEY_F5        = (char) -5,
	S2DE_KEY_F6        = (char) -6,
	S2DE_KEY_F7        = (char) -7,
	S2DE_KEY_F8        = (char) -8,
	S2DE_KEY_F9        = (char) -9,
	S2DE_KEY_F10       = (char)-10,
	S2DE_KEY_F11       = (char)-11,
	S2DE_KEY_F12       = (char)-12,
	S2DE_KEY_UP        = (char)-13,
	S2DE_KEY_DOWN      = (char)-14,
	S2DE_KEY_LEFT      = (char)-15,
	S2DE_KEY_RIGHT     = (char)-16,
	S2DE_KEY_PAGE_UP   = (char)-17,
	S2DE_KEY_PAGE_DOWN = (char)-18,
	S2DE_KEY_HOME      = (char)-19,
	S2DE_KEY_END       = (char)-20,
	S2DE_KEY_INSERT    = (char)-21,
	S2DE_KEY_BACKSPACE = (char)-22,
	S2DE_KEY_TAB       = (char)-23,
	S2DE_KEY_ENTER     = (char)-24,
	S2DE_KEY_ESCAPE    = (char)-25,
	S2DE_KEY_DELETE    = (char)-26,
	S2DE_KEY_UNDEFINED = (char)-27
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








// ---------------- UTILITIES ----------------

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

//images
void S2DE_imageRGBA(int x,int y, int width,int height, int* data);
int S2DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

//timed executions
void S2DE_setTimedExecution(int ms);








// ---------------- BASICS -----------------

//init
void S2DE_init(int argc, char** argv, const char* name, int width,int height);

//start - stop
void S2DE_start();
void S2DE_stop();
