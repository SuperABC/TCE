#pragma once
#include "screen.h"
#define inRect(x, y, xx, yy, xxx, yyy) (((x)>=(xx)&&(x)<=(xxx))&&((y)>=(yy)&&(y)<=(yyy)))

enum COLORS;
struct textMap {
	int width, height;
	short *content;
};
class text {
private:
	unsigned short *content;
	unsigned short *changed;
	const int width = 80, height = 25;
	char color;

	RGB bgc(short c) {
		c >>= 12;
		c &= 0x000F;
		switch (c) {
		case BLACK: return{ 0, 0, 0 };
		case BLUE: return{ 0, 0, 127 };
		case GREEN: return{ 0, 127, 0 };
		case CYAN: return{ 0, 127, 127 };
		case RED: return{ 127, 0, 0 };
		case MAGENTA: return{ 127, 0, 127 };
		case BROWN: return{ 127, 127, 0 };
		case DARKGRAY: return{ 127, 127, 127 };
		case LIGHTGRAY: return{ 191, 191, 191 };
		case LIGHTBLUE: return{ 0, 0, 255 };
		case LIGHTGREEN: return{ 0, 255, 0 };
		case LIGHTCYAN: return{ 0, 255, 255 };
		case LIGHTRED: return{ 255, 0, 0 };
		case LIGHTMAGENTA: return{ 255, 0, 255 };
		case YELLOW: return{ 255, 255, 0 };
		case WHITE: return{ 255, 255, 255 };
		}
		return{ 0, 0, 0 };
	}
	RGB fgc(short c) {
		c >>= 8;
		c &= 0x000F;
		switch (c) {
		case BLACK: return{ 0, 0, 0 };
		case BLUE: return{ 0, 0, 127 };
		case GREEN: return{ 0, 127, 0 };
		case CYAN: return{ 0, 127, 127 };
		case RED: return{ 127, 0, 0 };
		case MAGENTA: return{ 127, 0, 127 };
		case BROWN: return{ 127, 127, 0 };
		case DARKGRAY: return{ 127, 127, 127 };
		case LIGHTGRAY: return{ 191, 191, 191 };
		case LIGHTBLUE: return{ 0, 0, 255 };
		case LIGHTGREEN: return{ 0, 255, 0 };
		case LIGHTCYAN: return{ 0, 255, 255 };
		case LIGHTRED: return{ 255, 0, 0 };
		case LIGHTMAGENTA: return{ 255, 0, 255 };
		case YELLOW: return{ 255, 255, 0 };
		case WHITE: return{ 255, 255, 255 };
		}
		return{ 0, 0, 0 };
	}
	void renew(short c, int x, int y) {
		RGB b = bgc(c), f = fgc(c);
		setColor(b.r, b.g, b.b);
		putQuad(x * 8, y * 16, x * 8 + 7, y * 16 + 15, SOLID_FILL);
		setColor(f.r, f.g, f.b);
		putChar((char)c, x * 8, y * 16 - 2);
	}
public:
	text();
	void update();
	void setcolor(int bgc, int fgc);
	void setcolor(char color, int x, int y);
	void setbgcolor(char color, int x, int y);
	void setfgcolor(char color, int x, int y);
	void putchar(char c, int x, int y);
	void putstring(char *s, int x, int y);
	short getpoint(int x, int y);
	void gettext(int left, int up, int right, int bottom, textMap *text);
	void puttext(int left, int up, const textMap *text);
};

class editor {
private:
	const int width = 78, height = 22;
	vecTwo cursorPos;
	unsigned char *clipBoard;
	const textMap content = {78, 22, (short*)malloc(78*22*sizeof(short))};
	int endL[22] = { 0 };
	int tabS[22] = { 0 };
	enum LANGUAGE;
	static char *keyWord[5][100];
	void show();
	void cut(vecTwo start, vecTwo end);
	void copy(vecTwo start, vecTwo end);
	void paste(vecTwo pos);
	void match();
	void check();
public:
	editor();
	void cursor();
	void operate(vecThree bioM);
	void operate(int bioK);
};

enum MENUS {
	NONE,
	FILES,
	EDIT,
	TOOLS,
	RUN,
	HELP
};
enum WINDOWS {
	EMPTY_WINDOW,
	FILE_OPEN,
	FILE_SAVE,
	EDIT_GOTO,
	EDIT_CLIPBOARD,
	TOOLS_SETTING,
	TOOLS_SEARCH,
	TOOLS_REPLACE,
	HELP_VERSION,
	HELP_HELP
};
class frame {
private:
	bool ctrling, shifting, alting, draging;
	int menu, window;
	bool opening = false;
	textMap *txt;
	void list(int menu);
	void open(int window);
	void restore();
public:
	frame();
	void loop();
	friend void editor::operate(vecThree bioM);
	friend void editor::operate(int bioK);
};

class mouse {
private:
	int x, y;
	int pressed;
	unsigned char oldc;
public:
	vecTwo grid();
	vecTwo grid(int x, int y);
	void update();
	void restore();
	void renew();
};

class file {
private:
	char *fileName;
	char *fileFolder;
	char *fileContent;
public:
	file();
	bool open(char *fileName);
	bool save(char *fileName);
};

bool operator==(vecTwo a, vecTwo b);
bool operator==(vecThree a, vecThree b);