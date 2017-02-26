#pragma once
#include "sgl.h"
#include "direct.h"

#define SCREEN_WIDTH 108
#define SCREEN_HEIGHT 32
#define EDITOR_WIDTH 105
#define EDITOR_HEIGHT 28
#define MAX_KEYWORD_LEN 12
#define LANGUAGE_NUM 1

#define inRect(x, y, xx, yy, xxx, yyy) (((x)>=(xx)&&(x)<=(xxx))&&((y)>=(yy)&&(y)<=(yyy)))

//窗口回调函数
class functions {
public:
	static void closeTCE();
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
	textMap content = { EDITOR_WIDTH, EDITOR_HEIGHT,
		(short*)malloc(EDITOR_WIDTH * EDITOR_HEIGHT * sizeof(short)) };
	const int width = EDITOR_WIDTH, height = EDITOR_HEIGHT;
	//光标坐标
	vecTwo cursorPos;
	//剪贴板
	unsigned char *clipBoard;
	//括号自动补全
	bool complete = false;
	//上下光标对齐
	int endMax = 0;
	//行尾位置
	int endL[EDITOR_HEIGHT] = { 0 };
	//行首位置
	int tabS[EDITOR_HEIGHT] = { 0 };
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
