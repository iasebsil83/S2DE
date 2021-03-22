#ifndef S2DE_H
#define S2DE_H








// ---------------- DECLARATIONS ----------------

//keyboard constants
enum S2DE_KEYBOARD{
	S2DE_KEY_PRESSED,
	S2DE_KEY_RELEASED
};

enum S2DE_KEYS{
	//function keys
	S2DE_KEY_F1          = 0x101,
	S2DE_KEY_F2          = 0x102,
	S2DE_KEY_F3          = 0x103,
	S2DE_KEY_F4          = 0x104,
	S2DE_KEY_F5          = 0x105,
	S2DE_KEY_F6          = 0x106,
	S2DE_KEY_F7          = 0x107,
	S2DE_KEY_F8          = 0x108,
	S2DE_KEY_F9          = 0x109,
	S2DE_KEY_F10         = 0x10a,
	S2DE_KEY_F11         = 0x10b,
	S2DE_KEY_F12         = 0x10c,

	//command keys
	S2DE_KEY_ESCAPE      = 0x01b,
	S2DE_KEY_END         = 0x16b,
	S2DE_KEY_HOME        = 0x16a,
	S2DE_KEY_PAGE_UP     = 0x168,
	S2DE_KEY_PAGE_DOWN   = 0x169,
	S2DE_KEY_DELETE      = 0x07f,
	S2DE_KEY_BACKSPACE   = 0x008,
	S2DE_KEY_INSERT      = 0x16c,

	//combination keys
	S2DE_KEY_LEFT_SHIFT  = 0x170,
	S2DE_KEY_RIGHT_SHIFT = 0x171,
	S2DE_KEY_LEFT_CTRL   = 0x172,
	S2DE_KEY_RIGHT_CTRL  = 0x173,
	S2DE_KEY_LEFT_ALT    = 0x174,

	//special characters
	S2DE_KEY_BACK_QUOTE  = 0x0b2,
	S2DE_KEY_AMPERSAND   = 0x026,
	S2DE_KEY_DBL_QUOTE   = 0x022, //double quote
	S2DE_KEY_QUOTE       = 0x027,
	S2DE_KEY_UNDERSCORE  = 0x05f,
	S2DE_KEY_TAB         = 0x009,
	S2DE_KEY_CIRCUMFLEX  = 0x05e,
	S2DE_KEY_DOLLAR      = 0x024,
	S2DE_KEY_LESS_THAN   = 0x03c, //<
	S2DE_KEY_GREA_THAN   = 0x03e, //>
	S2DE_KEY_COMA        = 0x02c,
	S2DE_KEY_SEMI_COLON  = 0x03b,
	S2DE_KEY_COLON       = 0x03a,
	S2DE_KEY_EXCLAMATION = 0x021,
	S2DE_KEY_SPACE       = 0x020,
	S2DE_KEY_RETURN      = 0x00d,
	S2DE_KEY_TILDE       = 0x07e,
	S2DE_KEY_HASHTAG     = 0x023,
	S2DE_KEY_PARENTH_B   = 0x028, //parentheses begin
	S2DE_KEY_PARENTH_E   = 0x029, //            end
	S2DE_KEY_BRACKETS_B  = 0x05b, //brackets begin
	S2DE_KEY_BRACKETS_E  = 0x05d, //         end
	S2DE_KEY_BRACES_B    = 0x07b, //braces begin
	S2DE_KEY_BRACES_E    = 0x07d, //       end
	S2DE_KEY_POINT       = 0x02e,
	S2DE_KEY_PARAGRAPH   = 0x0a7,
	S2DE_KEY_DEGREE      = 0x0b0,
	S2DE_KEY_PIPE        = 0x07c,
	S2DE_KEY_BACKSLASH   = 0x05c,
	S2DE_KEY_AT          = 0x040,
	S2DE_KEY_POUND       = 0x0a3,
	S2DE_KEY_DIERESIS    = 0x0a8,
	S2DE_KEY_BLOCK       = 0x0a4,
	S2DE_KEY_ACUTE       = 0x060,
	S2DE_KEY_MU          = 0x0b5,
	S2DE_KEY_QUESTION    = 0x03f,

	//numbers & maths
	S2DE_KEY_0        = 0x030,
	S2DE_KEY_1        = 0x031,
	S2DE_KEY_2        = 0x032,
	S2DE_KEY_3        = 0x033,
	S2DE_KEY_4        = 0x034,
	S2DE_KEY_5        = 0x035,
	S2DE_KEY_6        = 0x036,
	S2DE_KEY_7        = 0x037,
	S2DE_KEY_8        = 0x038,
	S2DE_KEY_9        = 0x039,
	S2DE_KEY_PLUS     = 0x02b,
	S2DE_KEY_MINUS    = 0x02d,
	S2DE_KEY_ASTERISK = 0x02a,
	S2DE_KEY_SLASH    = 0x02f,
	S2DE_KEY_EQUAL    = 0x03d,
	S2DE_KEY_PERCENT  = 0x025,

	//arrows
	S2DE_KEY_LEFT  = 0x164,
	S2DE_KEY_UP    = 0x165,
	S2DE_KEY_RIGHT = 0x166,
	S2DE_KEY_DOWN  = 0x167,

	//capital letters
	S2DE_KEY_A = 0x041,
	S2DE_KEY_B = 0x042,
	S2DE_KEY_C = 0x043,
	S2DE_KEY_D = 0x044,
	S2DE_KEY_E = 0x045,
	S2DE_KEY_F = 0x046,
	S2DE_KEY_G = 0x047,
	S2DE_KEY_H = 0x048,
	S2DE_KEY_I = 0x049,
	S2DE_KEY_J = 0x04a,
	S2DE_KEY_K = 0x04b,
	S2DE_KEY_L = 0x04c,
	S2DE_KEY_M = 0x04d,
	S2DE_KEY_N = 0x04e,
	S2DE_KEY_O = 0x04f,
	S2DE_KEY_P = 0x050,
	S2DE_KEY_Q = 0x051,
	S2DE_KEY_R = 0x052,
	S2DE_KEY_S = 0x053,
	S2DE_KEY_T = 0x054,
	S2DE_KEY_U = 0x055,
	S2DE_KEY_V = 0x056,
	S2DE_KEY_W = 0x057,
	S2DE_KEY_X = 0x058,
	S2DE_KEY_Y = 0x059,
	S2DE_KEY_Z = 0x05a,

	//minimal letters
	S2DE_KEY_a = 0x061,
	S2DE_KEY_b = 0x062,
	S2DE_KEY_c = 0x063,
	S2DE_KEY_d = 0x064,
	S2DE_KEY_e = 0x065,
	S2DE_KEY_f = 0x066,
	S2DE_KEY_g = 0x067,
	S2DE_KEY_h = 0x068,
	S2DE_KEY_i = 0x069,
	S2DE_KEY_j = 0x06a,
	S2DE_KEY_k = 0x06b,
	S2DE_KEY_l = 0x06c,
	S2DE_KEY_m = 0x06d,
	S2DE_KEY_n = 0x06e,
	S2DE_KEY_o = 0x06f,
	S2DE_KEY_p = 0x070,
	S2DE_KEY_q = 0x071,
	S2DE_KEY_r = 0x072,
	S2DE_KEY_s = 0x073,
	S2DE_KEY_t = 0x074,
	S2DE_KEY_u = 0x075,
	S2DE_KEY_v = 0x076,
	S2DE_KEY_w = 0x077,
	S2DE_KEY_x = 0x078,
	S2DE_KEY_y = 0x079,
	S2DE_KEY_z = 0x07a,

	//non-ASCII letters
	S2DE_KEY_A_ACCENT_E = 0x0e9, //acute accentuated E
	S2DE_KEY_G_ACCENT_E = 0x0e8, //grave accentuated E
	S2DE_KEY_G_ACCENT_A = 0x0e0, //grave accentuated A
	S2DE_KEY_G_ACCENT_U = 0x0f9, //grave accentuated U
	S2DE_KEY_CEDILLA_C  = 0x0e7
};



//event constants
enum S2DE_EVENTS{
	S2DE_KEYBOARD,
	S2DE_MOUSE_CLICK,
	S2DE_MOUSE_MOVE,
	S2DE_MOUSE_SCROLL,
	S2DE_DISPLAY,
	S2DE_RESIZE,
	S2DE_TIMER
};



//mouse constants
#define S2DE_MOUSE_PRESSED  0x0000
#define S2DE_MOUSE_RELEASED 0x0001
#define S2DE_LEFT_BUTTON    0x0000
#define S2DE_MIDDLE_BUTTON  0x0001
#define S2DE_RIGHT_BUTTON   0x0002
#define S2DE_SCROLL_UP      0x0003
#define S2DE_SCROLL_DOWN    0x0004








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
void S2DE_setTimer(int ms);








// ---------------- BASICS -----------------

//init
void S2DE_init(int argc, char** argv, const char* name, unsigned int width,unsigned int height);

//start - stop
void S2DE_start();
void S2DE_stop();








#endif
