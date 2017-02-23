#pragma once
#include "screen.h"
#include "direct.h"

#define MAX_KEYWORD_LEN 12
#define LANGUAGE_NUM 1

#define inRect(x, y, xx, yy, xxx, yyy) (((x)>=(xx)&&(x)<=(xxx))&&((y)>=(yy)&&(y)<=(yyy)))

class functions {
public:
	static void closeTCE();
};

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
struct textMap {
	int width, height;
	short *content;
};
//文本模式
class text {
private:
	unsigned short *content;
	unsigned short *changed;
	const int width = 80, height = 25;
	char color;

	//提取背景色
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
	//提取前景色
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
	//更新某点信息
	void renew(short c, int x, int y) {
		RGB b = bgc(c), f = fgc(c);
		setColor(b.r, b.g, b.b);
		putQuad(x * 8, y * 16, x * 8 + 7, y * 16 + 15, SOLID_FILL);
		setColor(f.r, f.g, f.b);
		putChar((char)c, x * 8, y * 16 - 2);
	}
public:
	text();

	//更新屏幕所有点
	void update();
	//设置内置颜色
	void setcolor(int bgc, int fgc);
	//设置某点颜色
	void setcolor(char color, int x, int y);
	//设置某点背景色
	void setbgcolor(char color, int x, int y);
	//设置某点前景色
	void setfgcolor(char color, int x, int y);
	//用内置颜色写字符
	void putchar(char c, int x, int y);
	//用内置颜色写字符串
	void putstring(char *s, int x, int y);
	//获取某点信息
	short getpoint(int x, int y);
	//复制矩形区域
	void gettext(int left, int up, int right, int bottom, textMap *text);
	//粘贴矩形区域
	void puttext(int left, int up, const textMap *text);
};

//文件存取
class file {
private:
	char *fileName;
	char *fileFolder;
public:
	int editorBegin;
	int editorEnd;
	int fileLength;
	char *fileContent;

	file();
	bool openFile(char *fileName);
	bool saveFile(char *fileName);
	void closeFile();
	char posContent(int pos);
};

//内容编辑器
class editor {
private:
	//当前屏幕区域内容
	const textMap content = { 78, 22, (short*)malloc(78 * 22 * sizeof(short)) };
	const int width = 78, height = 22;
	//光标坐标
	vecTwo cursorPos;
	//剪贴板
	unsigned char *clipBoard;
	//括号自动补全
	bool complete = false;
	//上下光标对齐
	int endMax = 0;
	//行尾位置
	int endL[22] = { 0 };
	//行首位置
	int tabS[22] = { 0 };
	enum LANGUAGE;
	//关键字集合
	static char *keyWord[5][100];
	void cut(vecTwo start, vecTwo end);
	void copy(vecTwo start, vecTwo end);
	void paste(vecTwo pos);
	//高亮匹配
	void match();
	//检测行首行尾位置
	void check();
public:
	editor();
	//鼠标闪烁
	void cursor();
	//编辑器内容送入屏幕
	void show();
	//编辑器挂起
	void suspend();
	//清屏
	void clear();
	//输出单行
	int putRow(int pos, int line);
	//响应键盘操作
	void operate(int bioK, bool comp = true);
	//响应鼠标点击操作
	void operate(vecThree bioM);
	//响应鼠标滚轮操作
	void stroll(int dir);

	friend bool file::openFile(char *fileName);
	friend bool file::saveFile(char *fileName);
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
	HELP_HELP,
	ALERT_MESSAGE
};
enum ALERTTYPE {
	NO_BUTTON = 0,
	OK_BUTTON = 1,
	CANCEL_BUTTON = 2
};
//架构器
class frame {
private:
	bool ctrling, shifting, alting, draging;
	int menu, window, alertReturn;
	static const char *newFileAlert, *saveCurrentAlert, *saveAsAlert, *closeFileAlert, *closeTCEAlert,
		*undoAlert, *redoAlert, *cutAlert, *copyAlert, *pasteAlert, *deleteAlert, *gotoAlert, *clipboardAlert,
		*settingAlert, *searchAlert, *replaceAlert,
		*compileAlert, *runAlert, *debugAlert;
	//菜单是否打开
	bool opening = false;
	//覆盖区域内容
	textMap *txt;
	//响应函数
	vect tmpVect;
	//打开菜单
	void list(int menu);
	//打开窗口
	void open(int window);
	//恢复菜单与窗口
	void restore();
public:
	frame();
	friend functions;
	//主循环
	void loop();
	void alert(const char *content, int type);
	friend void editor::operate(vecThree bioM);
	friend void editor::operate(int bioK, bool comp);
};

//鼠标处理
class mouse {
private:
	int x, y;
	int pressed;
	//上一位置内容
	unsigned char oldc;
public:
	//光栅化鼠标坐标
	vecTwo grid();
	//光栅化输入坐标
	vecTwo grid(int x, int y);
	//照常更新
	void update();
	//单独恢复上一位置
	void restore();
	//单独覆盖下一位置
	void renew();
};

bool operator==(vecTwo a, vecTwo b);
bool operator==(vecThree a, vecThree b);
LPWSTR widen(char *src);
