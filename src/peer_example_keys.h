enum KEYS {
	KEY_F1          = 0x101,
	KEY_F2          = 0x102,
	KEY_F3          = 0x103,
	KEY_F4          = 0x104,
	KEY_F5          = 0x105,
	KEY_F6          = 0x106,
	KEY_F7          = 0x107,
	KEY_F8          = 0x108,
	KEY_F9          = 0x109,
	KEY_F10         = 0x10a,
	KEY_F11         = 0x10b,
	KEY_F12         = 0x10c,

	//command keys
	KEY_ESCAPE      = 0x01b,
	KEY_END         = 0x16b,
	KEY_HOME        = 0x16a,
	KEY_PAGE_UP     = 0x168,
	KEY_PAGE_DOWN   = 0x169,
	KEY_DELETE      = 0x07f,
	KEY_BACKSPACE   = 0x008,
	KEY_INSERT      = 0x16c,

	//combination keys
	KEY_LEFT_SHIFT  = 0x170,
	KEY_RIGHT_SHIFT = 0x171,
	KEY_LEFT_CTRL   = 0x172,
	KEY_RIGHT_CTRL  = 0x173,
	KEY_LEFT_ALT    = 0x174,

	//special characters
	KEY_BACK_QUOTE  = 0x0b2,
	KEY_AMPERSAND   = 0x026,
	KEY_DBL_QUOTE   = 0x022, //double quote
	KEY_QUOTE       = 0x027,
	KEY_UNDERSCORE  = 0x05f,
	KEY_TAB         = 0x009,
	KEY_CIRCUMFLEX  = 0x05e,
	KEY_DOLLAR      = 0x024,
	KEY_LESS_THAN   = 0x03c, //<
	KEY_GREA_THAN   = 0x03e, //>
	KEY_COMA        = 0x02c,
	KEY_SEMI_COLON  = 0x03b,
	KEY_COLON       = 0x03a,
	KEY_EXCLAMATION = 0x021,
	KEY_SPACE       = 0x020,
	KEY_RETURN      = 0x00d,
	KEY_TILDE       = 0x07e,
	KEY_HASHTAG     = 0x023,
	KEY_PARENTH_B   = 0x028, //parentheses begin
	KEY_PARENTH_E   = 0x029, //            end
	KEY_BRACKETS_B  = 0x05b, //brackets begin
	KEY_BRACKETS_E  = 0x05d, //         end
	KEY_BRACES_B    = 0x07b, //braces begin
	KEY_BRACES_E    = 0x07d, //       end
	KEY_POINT       = 0x02e,
	KEY_PARAGRAPH   = 0x0a7,
	KEY_DEGREE      = 0x0b0,
	KEY_PIPE        = 0x07c,
	KEY_BACKSLASH   = 0x05c,
	KEY_AT          = 0x040,
	KEY_POUND       = 0x0a3,
	KEY_DIERESIS    = 0x0a8,
	KEY_BLOCK       = 0x0a4,
	KEY_ACUTE       = 0x060,
	KEY_MU          = 0x0b5,
	KEY_QUESTION    = 0x03f,

	//numbers & maths
	KEY_0        = 0x030,
	KEY_1        = 0x031,
	KEY_2        = 0x032,
	KEY_3        = 0x033,
	KEY_4        = 0x034,
	KEY_5        = 0x035,
	KEY_6        = 0x036,
	KEY_7        = 0x037,
	KEY_8        = 0x038,
	KEY_9        = 0x039,
	KEY_PLUS     = 0x02b,
	KEY_MINUS    = 0x02d,
	KEY_ASTERISK = 0x02a,
	KEY_SLASH    = 0x02f,
	KEY_EQUAL    = 0x03d,
	KEY_PERCENT  = 0x025,

	//arrows
	KEY_LEFT  = 0x164,
	KEY_UP    = 0x165,
	KEY_RIGHT = 0x166,
	KEY_DOWN  = 0x167,

	//capital letters
	KEY_A = 0x041,
	KEY_B = 0x042,
	KEY_C = 0x043,
	KEY_D = 0x044,
	KEY_E = 0x045,
	KEY_F = 0x046,
	KEY_G = 0x047,
	KEY_H = 0x048,
	KEY_I = 0x049,
	KEY_J = 0x04a,
	KEY_K = 0x04b,
	KEY_L = 0x04c,
	KEY_M = 0x04d,
	KEY_N = 0x04e,
	KEY_O = 0x04f,
	KEY_P = 0x050,
	KEY_Q = 0x051,
	KEY_R = 0x052,
	KEY_S = 0x053,
	KEY_T = 0x054,
	KEY_U = 0x055,
	KEY_V = 0x056,
	KEY_W = 0x057,
	KEY_X = 0x058,
	KEY_Y = 0x059,
	KEY_Z = 0x05a,

	//minimal letters
	KEY_a = 0x061,
	KEY_b = 0x062,
	KEY_c = 0x063,
	KEY_d = 0x064,
	KEY_e = 0x065,
	KEY_f = 0x066,
	KEY_g = 0x067,
	KEY_h = 0x068,
	KEY_i = 0x069,
	KEY_j = 0x06a,
	KEY_k = 0x06b,
	KEY_l = 0x06c,
	KEY_m = 0x06d,
	KEY_n = 0x06e,
	KEY_o = 0x06f,
	KEY_p = 0x070,
	KEY_q = 0x071,
	KEY_r = 0x072,
	KEY_s = 0x073,
	KEY_t = 0x074,
	KEY_u = 0x075,
	KEY_v = 0x076,
	KEY_w = 0x077,
	KEY_x = 0x078,
	KEY_y = 0x079,
	KEY_z = 0x07a,

	//non-ASCII letters
	KEY_A_ACCENT_E = 0x0e9, //acute accentuated E
	KEY_G_ACCENT_E = 0x0e8, //grave accentuated E
	KEY_G_ACCENT_A = 0x0e0, //grave accentuated A
	KEY_G_ACCENT_U = 0x0f9, //grave accentuated U
	KEY_CEDILLA_C  = 0x0e7
};
