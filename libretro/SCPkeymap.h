

/* scancode key to SAM COUPE scan code mapping table */
const char keyboard_translation[512] =
#if 1
{
	/* SCP,  PC Code */
	-1,    /* 0 */ 	-1,    /* 1 */ 	-1,    /* 2 */ 	-1,    /* 3 */ 	-1,    /* 4 */ 	-1,    /* 5 */ 	-1,    /* 6 */ 	-1,    /* 7 */
	-1,  /* SDLK_BACKSPACE=8 */
	-1,  /* SDLK_TAB=9 */
	-1,    /* 10 */ -1,    /* 11 */
	-1,  /* SDLK_CLEAR = 12 */
	-1,  /* SDLK_RETURN = 13 */
	-1,    /* 14 */	-1,    /* 15 */	-1,    /* 16 */ -1,    /* 17 */	-1,    /* 18 */
	-1,    /* SDLK_PAUSE = 19 */
	-1,    /* 20 */	-1,    /* 21 */	-1,    /* 22 */	-1,    /* 23 */	-1,    /* 24 */	-1,    /* 25 */	-1,    /* 26 */
	-1,  /* SDLK_ESCAPE = 27 */
	-1,    /* 28 */	-1,    /* 29 */	-1,    /* 30 */	-1,    /* 31 */
	-1,  /* SDLK_SPACE = 32 */
	-1,    /* SDLK_EXCLAIM = 33 */
	-1,    /* SDLK_QUOTEDBL = 34 */
	-1,    /* SDLK_HASH = 35 */
	-1,    /* SDLK_DOLLAR = 36 */
	-1,    /* 37 */
	-1,    /* SDLK_AMPERSAND = 38 */
	-1,  /* SDLK_QUOTE = 39 */
	-1,  /* SDLK_LEFTPAREN = 40 */
	-1,  /* SDLK_RIGHTPAREN = 41 */
	-1,    /* SDLK_ASTERISK = 42 */
	-1,  /* SDLK_PLUS = 43 */
	-1,  /* SDLK_COMMA = 44 */
	-1,  /* SDLK_MINUS = 45 */
	-1,  /* SDLK_PERIOD = 46 */
	-1,  /* SDLK_SLASH = 47 */
	-1,  /* SDLK_0 = 48 */
	-1,  /* SDLK_1 = 49 */
	-1,  /* SDLK_2 = 50 */
	-1,  /* SDLK_3 = 51 */
	-1,  /* SDLK_4 = 52 */
	-1,  /* SDLK_5 = 53 */
	-1,  /* SDLK_6 = 54 */
	-1,  /* SDLK_7 = 55 */
	-1,  /* SDLK_8 = 56 */
	-1,  /* SDLK_9 = 57 */
	-1,    /* SDLK_COLON = 58 */
	-1,    /* SDLK_SEMICOLON = 59 */
	-1,    /* SDLK_LESS = 60 */
	-1,  /* SDLK_EQUALS = 61 */
	-1,    /* SDLK_GREATER  = 62 */
	-1,    /* SDLK_QUESTION = 63 */
	-1,    /* SDLK_AT = 64 */
	-1,    /* 65 */  /* Skip uppercase letters */
	-1,    /* 66 */	-1,    /* 67 */	-1,    /* 68 */	-1,    /* 69 */	-1,    /* 70 */	-1,    /* 71 */	-1,    /* 72 */	-1,    /* 73 */	-1,    /* 74 */
	-1,    /* 75 */	-1,    /* 76 */ -1,    /* 77 */	-1,    /* 78 */	-1,    /* 79 */	-1,    /* 80 */	-1,    /* 81 */	-1,    /* 82 */	-1,    /* 83 */
	-1,    /* 84 */	-1,    /* 85 */	-1,    /* 86 */	-1,    /* 87 */	-1,    /* 88 */	-1,    /* 89 */	-1,    /* 90 */
	-1,  /* SDLK_LEFTBRACKET = 91 */
	-1,  /* SDLK_BACKSLASH = 92 */     /* Might be 0x60 for UK keyboards */
	-1,  /* SDLK_RIGHTBRACKET = 93 */
	-1,   /* SDLK_CARET = 94 */
	-1,    /* SDLK_UNDERSCORE = 95 */
	-1,    /* SDLK_BACKQUOTE = 96 */
/*A*/-1,
/*B*/-1,
/*C*/-1,
/*D*/-1,
/*E*/-1,
/*F*/-1,
/*G*/-1,
/*H*/-1,
/*I*/-1,
/*J*/-1,
/*K*/-1,
/*L*/-1,
/*M*/-1,
/*N*/-1,
/*O*/-1,
/*P*/-1,
/*Q*/-1,
/*R*/-1,
/*S*/-1,
/*T*/-1,
/*U*/-1,
/*V*/-1,
/*W*/-1,
/*X*/-1,
/*Y*/-1,
/*Z*/-1,
	-1,    /* 123 */	-1,    /* 124 */	-1,    /* 125 */	-1,    /* 126 */
	-1,  /* SDLK_DELETE = 127 */
	/* End of ASCII mapped keysyms */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 128-143*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 144-159*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 160-175*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 176-191*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 192-207*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 208-223*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 224-239*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 240-255*/
	/* Numeric keypad: */
	-1,    /* SDLK_KP0 = 256 */
	-1,    /* SDLK_KP1 = 257 */
	-1,    /* SDLK_KP2 = 258 */
	-1,    /* SDLK_KP3 = 259 */
	-1,    /* SDLK_KP4 = 260 */
	-1,    /* SDLK_KP5 = 261 */
	-1,    /* SDLK_KP6 = 262 */
	-1,    /* SDLK_KP7 = 263 */
	-1,    /* SDLK_KP8 = 264 */
	-1,    /* SDLK_KP9 = 265 */
	-1 ,    /* SDLK_KP_PERIOD = 266 */
	-1,    /* SDLK_KP_DIVIDE = 267 */
	-1,    /* SDLK_KP_MULTIPLY = 268 */
	-1,    /* SDLK_KP_MINUS = 269 */
	-1,    /* SDLK_KP_PLUS = 270 */
	-1,    /* SDLK_KP_ENTER = 271 */
	-1,    /* SDLK_KP_EQUALS = 272 */
	/* Arrows + Home/End pad */
	-1,    /* SDLK_UP = 273 */
	-1,    /* SDLK_DOWN = 274 */
	-1,    /* SDLK_RIGHT = 275 */
	-1,    /* SDLK_LEFT = 276 */
	-1,    /* SDLK_INSERT = 277 */
	-1,    /* SDLK_HOME = 278 */
	-1,    /* SDLK_END = 279 */
	-1,    /* SDLK_PAGEUP = 280 */
	-1,    /* SDLK_PAGEDOWN = 281 */
	/* Function keys */
	-1,    /* SDLK_F1 = 282 */
	-1,    /* SDLK_F2 = 283 */
	-1,    /* SDLK_F3 = 284 */
	-1,    /* SDLK_F4 = 285 */
	-1,    /* SDLK_F5 = 286 */
	-1,    /* SDLK_F6 = 287 */
	-1,    /* SDLK_F7 = 288 */
	-1,    /* SDLK_F8 = 289 */
	-1,    /* SDLK_F9 = 290 */
	-1,    /* SDLK_F10 = 291 */
	-1,      /* SDLK_F11 = 292 */
	-1,       /* SDLK_F12 = 293 */
	-1  ,    /* SDLK_F13 = 294 */
	-1,      /* SDLK_F14 = 295 */
	-1,      /* SDLK_F15 = 296 */
	-1,      /* 297 */	-1,      /* 298 */	-1,      /* 299 */
	/* Key state modifier keys */
	-1 ,      /* SDLK_NUMLOCK = 300 */
	-1,    /* SDLK_CAPSLOCK = 301 */
	-1,    /* SDLK_SCROLLOCK = 302 */
	-1,    /* SDLK_RSHIFT = 303 */
	-1,    /* SDLK_LSHIFT = 304 */
	-1,    /* SDLK_RCTRL = 305 */
	-1,    /* SDLK_LCTRL = 306 */
	-1,    /* SDLK_RALT = 307 */
	-1,    /* SDLK_LALT = 308 */
	-1,      /* SDLK_RMETA = 309 */
	-1,      /* SDLK_LMETA = 310 */
	-1,      /* SDLK_LSUPER = 311 */
	-1,      /* SDLK_RSUPER = 312 */
	-1,      /* SDLK_MODE = 313 */     /* "Alt Gr" key */
	-1,      /* SDLK_COMPOSE = 314 */
	/* Miscellaneous function keys */
	-1,    /* SDLK_HELP = 315 */
	-1,    /* SDLK_PRINT = 316 */
	-1,      /* SDLK_SYSREQ = 317 */
	-1,      /* SDLK_BREAK = 318 */
	-1,      /* SDLK_MENU = 319 */
	-1,      /* SDLK_POWER = 320 */
	-1,      /* SDLK_EURO = 321 */
	-1     /* SDLK_UNDO = 322 */
};
#endif

