#include "tce.h"

frame *Frame;
mouse *Mouse;
editor *Editor;
file *File;

void sgSetup(){
	initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TC Emulator", TEXT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		Frame = new frame();
		Mouse = new mouse();
		Editor = new editor();
		File = new file();
	}
	hideMouse();
	Frame->loop();
}

LPWSTR widen(char *src) {
	int rt;
	LPWSTR rs;
	rt = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	rs = (LPWSTR)malloc(rt * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, src, -1, rs, rt * sizeof(wchar_t));
	return rs;
}
bool operator==(vecTwo a, vecTwo b) {
	return (a.x == b.x) && (a.y == b.y);
}
bool operator==(vecThree a, vecThree b) {
	return (a.x == b.x) && (a.y == b.y) && (a.m == b.m);
}
