#include "tce.h"

enum COLORS {
	BLACK,       /* dark colors */
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	DARKGRAY,
	LIGHTGRAY,    /* light colors */
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

enum editor::LANGUAGE {
	CL_C,
	CL_CPP,
	CL_CS,
	CL_JAVA,
	CL_ASM,
};
char *editor::keyWord[5][100] = { {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union", "unsigned", "void",
														"for", "do", "while", "break", "continue", "if", "else", "goto", "switch", "case", "default", "return", 
														"auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile"} };