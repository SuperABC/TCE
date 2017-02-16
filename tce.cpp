#include "tce.h"

extern text Text;
extern frame Frame;
extern mouse Mouse;
extern editor Editor;
extern file File;

text::text() {
	content = (unsigned short *)malloc(80 * 25 * sizeof(unsigned short));
	changed = (unsigned short *)malloc(80 * 25 * sizeof(unsigned short));
}
void text::update() {
	for (int i = 0; i < 80 * 25; i++) {
		if (changed[i] != content[i]) {
			content[i] = changed[i];
			renew(content[i], i % 80, i / 80);
		}
	}
}
void text::setcolor(int bgc, int fgc) {
	color = (bgc << 4)|fgc;
}
void text::setcolor(char color, int x, int y) {
	int pos = y * 80 + x;
	changed[pos] &= 0x00FF;
	changed[pos] |= (color << 8);
}
void text::setbgcolor(char color, int x, int y) {
	int pos = y * 80 + x;
	changed[pos] &= 0x0FFF;
	changed[pos] |= ((color&0x0F) << 12);
}
void text::setfgcolor(char color, int x, int y) {
	int pos = y * 80 + x;
	changed[pos] &= 0xF0FF;
	changed[pos] |= ((color & 0x0F) << 8);
}
void text::putchar(char c, int x, int y) {
	int pos = y * 80 + x;
	changed[pos] = ((short)color << 8)|(short)c;
}
void text::putstring(char *s, int x, int y) {
	for (int i = 0; s[i] != '\0'; i++) {
		putchar(s[i], x++, y);
	}
}
short text::getpoint(int x, int y) {
	return changed[y * 80 + x];
}
void text::gettext(int left, int up, int right, int bottom, textMap *text) {
	text->width = right - left + 1;
	text->height = bottom - up + 1;
	text->content = (short*)malloc(text->width * text->height * sizeof(short));
	for (int i = 0; i < text->height; i++) {
		memcpy(text->content + (i*text->width), changed + (up + i) * 80 + left, text->width * sizeof(short));
	}
}
void text::puttext(int left, int up, const textMap *text) {
	for (int i = 0; i < text->height; i++) {
		memcpy(changed + (up + i) * 80 + left, text->content + (i*text->width), text->width * sizeof(short));
	}
}

editor::editor() {
	cursorPos = { 0, 0 };
	for (int i = 0; i < 22 * 78; i++) {
		content.content[i] = ((BLUE << 12) | (YELLOW << 8) | 0);
	}
}
void editor::show() {
	Text.puttext(1, 2, &content);
}
void editor::operate(int bioK) {
	if (bioK >= 0x20 && bioK < 0x80) {
		content.content[cursorPos.y*width + cursorPos.x] &= 0xFF00;
		content.content[cursorPos.y*width + cursorPos.x] |= bioK;
		cursorPos.x = (cursorPos.x + 1) % width;
		cursorPos.y += (cursorPos.x ? 0 : 1);
		show();
	}
	if (bioK == SG_BACKS) {
		content.content[cursorPos.y*width + cursorPos.x] &= 0xFF00;
		content.content[cursorPos.y*width + cursorPos.x] |= 0;
		cursorPos.x = (!cursorPos.x && !cursorPos.y) ? cursorPos.x : (cursorPos.x - 1) % width;
		cursorPos.y -= (cursorPos.x||!cursorPos.y ? 0 : 1);
		show();
	}
}
void editor::operate(vecThree bioM) {

}

frame::frame() {
	ctrling = shifting = alting = draging = 0;
	menu = window = 0;
	Text.setcolor(LIGHTGRAY, BLACK);
	Text.putstring("  File    Edit   Tools    Run     Help                                          ", 0, 0);
	Text.setcolor((char)(LIGHTGRAY << 4 | RED), 2, 0);
	Text.setcolor((char)(LIGHTGRAY << 4 | RED), 10, 0);
	Text.setcolor((char)(LIGHTGRAY << 4 | RED), 17, 0);
	Text.setcolor((char)(LIGHTGRAY << 4 | RED), 26, 0);
	Text.setcolor((char)(LIGHTGRAY << 4 | RED), 34, 0);
	Text.setcolor(BLUE, YELLOW);
	for (int i = 80; i < 80 * 25; i++)Text.putchar(' ', i % 80, i / 80);
	Text.setcolor(LIGHTBLUE, WHITE);
	for (int i = 81; i < 159; i++)Text.putchar('=', i % 80, i / 80);
	for (int i = 1921; i < 1999; i++)Text.putchar('=', i % 80, i / 80);
	for (int i = 80; i < 2000; i+=80)Text.putchar('|', i % 80, i / 80);
	for (int i = 159; i < 2000; i+=80)Text.putchar(' ', i % 80, i / 80);
	Text.putchar('*', 0, 1);
	Text.putchar('*', 0, 24);
}
void frame::loop() {
	Mouse.update();
	vecThree click = { -1, -1, -1 };
	int key = 0;
	if (biosMouse(1).m)click = biosMouse(0);
	if (biosKey(1))key = biosKey(0);
	switch (key) {
	case SG_LCTRL | 0x8000:
	case SG_RCTRL | 0x8000:
		ctrling = true;
		break;
	case SG_LCTRL:
	case SG_RCTRL:
		ctrling = false;
		break;
	case SG_LSHIFT | 0x8000:
	case SG_RSHIFT | 0x8000:
		shifting = true;
		break;
	case SG_LSHIFT:
	case SG_RSHIFT:
		shifting = false;
		break;
	case SG_LALT | 0x8000:
	case SG_RALT | 0x8000:
		alting = true;
		break;
	case SG_LALT:
	case SG_RALT:
		alting = false;
		break;
	case SG_ESC:
		if (opening) {
			opening = false;
			restore();
		}
	}
	if (key)Editor.operate(key^0x8000);
	if (click.x < 0 || click.y < 0)return;
	vecTwo clickF = Mouse.grid(click.x, click.y);
	if (!opening) {
		if (inRect(clickF.x, clickF.y, 0, 0, 7, 0))list(FILES);
		else if (inRect(clickF.x, clickF.y, 8, 0, 15, 0))list(EDIT);
		else if (inRect(clickF.x, clickF.y, 16, 0, 23, 0))list(TOOLS);
		else if (inRect(clickF.x, clickF.y, 24, 0, 31, 0))list(RUN);
		else if (inRect(clickF.x, clickF.y, 32, 0, 39, 0))list(HELP);
		Editor.operate(click);
	}
	else if (menu != NONE) {
		if (inRect(clickF.x, clickF.y, 0, 0, 7, 0)) {
			restore();
			if(menu != FILES)list(FILES);
			else opening = false;
		}
		else if (inRect(clickF.x, clickF.y, 8, 0, 15, 0)) {
			restore();
			if(menu != EDIT)list(EDIT);
			else opening = false;
		}
		else if (inRect(clickF.x, clickF.y, 16, 0, 23, 0)) {
			restore();
			if(menu != TOOLS)list(TOOLS);
			else opening = false;
		}
		else if (inRect(clickF.x, clickF.y, 24, 0, 31, 0)) {
			restore();
			if(menu != RUN)list(RUN);
			else opening = false;
		}
		else if (inRect(clickF.x, clickF.y, 32, 0, 39, 0)) {
			restore();
			if(menu != HELP)list(HELP);
			else opening = false;
		}
	}
}
void frame::list(int menu) {
	static int menuHeight[5] = { 7, 10, 4, 4, 2 };
	txt = (textMap*)malloc(sizeof(textMap));
	opening = true;
	this->menu = menu;
	Mouse.restore();
	Text.gettext(8 * (menu - 1), 0, 8 * menu + 15, menuHeight[menu - 1] + 3, txt);
	for (int i = 0; i < 8; i++)Text.setbgcolor(LIGHTGREEN, (menu - 1) * 8 + i, 0);
	Text.setcolor(LIGHTGRAY, BLACK);
	switch (menu) {
	case FILES:
		Text.putstring("--------------------", 8 * (menu - 1), 1);
		Text.putstring("* New file      F1 *", 8 * (menu - 1), 2);
		Text.putstring("* Open file     F2 *", 8 * (menu - 1), 3);
		Text.putstring("* Save current  F3 *", 8 * (menu - 1), 4);
		Text.putstring("* Save as          *", 8 * (menu - 1), 5);
		Text.putstring("* ---------------- *", 8 * (menu - 1), 6);
		Text.putstring("* Close file       *", 8 * (menu - 1), 7);
		Text.putstring("* Close TCE     F4 *", 8 * (menu - 1), 8);
		Text.putstring("--------------------", 8 * (menu - 1), 9);
		for(int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 2);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 3);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 4);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 5);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 7);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 8);
		break;
	case EDIT:
		Text.putstring("--------------------", 8 * (menu - 1), 1);
		Text.putstring("* Undo      Ctrl+Z *", 8 * (menu - 1), 2);
		Text.putstring("* Redo      Ctrl+Y *", 8 * (menu - 1), 3);
		Text.putstring("* ---------------- *", 8 * (menu - 1), 4);
		Text.putstring("* Cut       Ctrl+X *", 8 * (menu - 1), 5);
		Text.putstring("* Copy      Ctrl+C *", 8 * (menu - 1), 6);
		Text.putstring("* Paste     Ctrl+V *", 8 * (menu - 1), 7);
		Text.putstring("* Delete           *", 8 * (menu - 1), 8);
		Text.putstring("* Goto      Ctrl+G *", 8 * (menu - 1), 9);
		Text.putstring("* ---------------- *", 8 * (menu - 1), 10);
		Text.putstring("* Clipboard        *", 8 * (menu - 1), 11);
		Text.putstring("--------------------", 8 * (menu - 1), 12);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 2);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 3);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 5);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 6);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 7);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 8);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 9);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 11);
		break;
	case TOOLS:
		Text.putstring("--------------------", 8 * (menu - 1), 1);
		Text.putstring("* Setting       F6 *", 8 * (menu - 1), 2);
		Text.putstring("* ---------------- *", 8 * (menu - 1), 3);
		Text.putstring("* Search           *", 8 * (menu - 1), 4);
		Text.putstring("* Replace          *", 8 * (menu - 1), 5);
		Text.putstring("--------------------", 8 * (menu - 1), 6);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 2);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 4);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 5);
		break;
	case RUN:
		Text.putstring("--------------------", 8 * (menu - 1), 1);
		Text.putstring("* Compile       F5 *", 8 * (menu - 1), 2);
		Text.putstring("* Run          F10 *", 8 * (menu - 1), 3);
		Text.putstring("* ---------------- *", 8 * (menu - 1), 4);
		Text.putstring("* Debug         F9 *", 8 * (menu - 1), 5);
		Text.putstring("--------------------", 8 * (menu - 1), 6);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 2);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 3);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 5);
		break;
	case HELP:
		Text.putstring("--------------------", 8 * (menu - 1), 1);
		Text.putstring("* Version          *", 8 * (menu - 1), 2);
		Text.putstring("* Help             *", 8 * (menu - 1), 3);
		Text.putstring("--------------------", 8 * (menu - 1), 4);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 2);
		for (int i = 0; i < 16; i++)Text.setfgcolor(DARKGRAY, 8 * (menu - 1) + i + 2, 3);
		break;
	}
	Mouse.renew();
}
void frame::open(int window) {

}
void frame::restore() {
	Mouse.restore();
	Text.puttext(8 * (menu - 1), 0, txt);
	Mouse.renew();
	free(txt);
}

vecTwo mouse::grid() {
	vecTwo ret;
	ret.x = x / 8;
	ret.y = y / 16;
	return ret;
}
vecTwo mouse::grid(int x, int y) {
	vecTwo ret;
	ret.x = x / 8;
	ret.y = y / 16;
	return ret;
}
void mouse::update() {
	static int first = 1;
	vecTwo newPos = mousePos();
	if (first) {
		x = newPos.x;
		y = newPos.y;
		if (x < 0 || x >= 640 || y < 0 || y >= 400)return;
		pressed = 0;
		oldc = (Text.getpoint(grid().x, grid().y) >> 8);
		Text.setcolor(oldc ^ 0x88, grid().x, grid().y);
		first = 0;
	}
	if (grid(newPos.x, newPos.y) == grid())return;
	Text.setcolor(oldc, grid().x, grid().y);
	oldc = Text.getpoint(grid(newPos.x, newPos.y).x, grid(newPos.x, newPos.y).y) >> 8;
	Text.setcolor(oldc ^ 0x88, grid(newPos.x, newPos.y).x, grid(newPos.x, newPos.y).y);
	x = newPos.x;
	y = newPos.y;
}
void mouse::restore() {
	Text.setcolor(oldc, grid().x, grid().y);
}
void mouse::renew() {
	oldc = Text.getpoint(grid().x, grid().y) >> 8;
	Text.setcolor(oldc ^ 0x88, grid().x, grid().y);
}

file::file() {
	fileName = (char*)malloc(64);
	fileFolder = (char*)malloc(256);
	fileContent = (char*)malloc(64 * 1024);
}
