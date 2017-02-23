#include "tce.h"

text Text;
frame Frame;
mouse Mouse;
editor Editor;
file File;

void sgSetup(){
	initWindow(640, 400, "TC Emulator");
	initMouse(SG_COORDINATE);
	initKey();
}
void sgLoop() {
	hideMouse();
	Frame.loop();
	Text.update();
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
