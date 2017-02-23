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
//�ı�ģʽ
class text {
private:
	unsigned short *content;
	unsigned short *changed;
	const int width = 80, height = 25;
	char color;

	//��ȡ����ɫ
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
	//��ȡǰ��ɫ
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
	//����ĳ����Ϣ
	void renew(short c, int x, int y) {
		RGB b = bgc(c), f = fgc(c);
		setColor(b.r, b.g, b.b);
		putQuad(x * 8, y * 16, x * 8 + 7, y * 16 + 15, SOLID_FILL);
		setColor(f.r, f.g, f.b);
		putChar((char)c, x * 8, y * 16 - 2);
	}
public:
	text();

	//������Ļ���е�
	void update();
	//����������ɫ
	void setcolor(int bgc, int fgc);
	//����ĳ����ɫ
	void setcolor(char color, int x, int y);
	//����ĳ�㱳��ɫ
	void setbgcolor(char color, int x, int y);
	//����ĳ��ǰ��ɫ
	void setfgcolor(char color, int x, int y);
	//��������ɫд�ַ�
	void putchar(char c, int x, int y);
	//��������ɫд�ַ���
	void putstring(char *s, int x, int y);
	//��ȡĳ����Ϣ
	short getpoint(int x, int y);
	//���ƾ�������
	void gettext(int left, int up, int right, int bottom, textMap *text);
	//ճ����������
	void puttext(int left, int up, const textMap *text);
};

//�ļ���ȡ
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

//���ݱ༭��
class editor {
private:
	//��ǰ��Ļ��������
	const textMap content = { 78, 22, (short*)malloc(78 * 22 * sizeof(short)) };
	const int width = 78, height = 22;
	//�������
	vecTwo cursorPos;
	//������
	unsigned char *clipBoard;
	//�����Զ���ȫ
	bool complete = false;
	//���¹�����
	int endMax = 0;
	//��βλ��
	int endL[22] = { 0 };
	//����λ��
	int tabS[22] = { 0 };
	enum LANGUAGE;
	//�ؼ��ּ���
	static char *keyWord[5][100];
	void cut(vecTwo start, vecTwo end);
	void copy(vecTwo start, vecTwo end);
	void paste(vecTwo pos);
	//����ƥ��
	void match();
	//���������βλ��
	void check();
public:
	editor();
	//�����˸
	void cursor();
	//�༭������������Ļ
	void show();
	//�༭������
	void suspend();
	//����
	void clear();
	//�������
	int putRow(int pos, int line);
	//��Ӧ���̲���
	void operate(int bioK, bool comp = true);
	//��Ӧ���������
	void operate(vecThree bioM);
	//��Ӧ�����ֲ���
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
//�ܹ���
class frame {
private:
	bool ctrling, shifting, alting, draging;
	int menu, window, alertReturn;
	static const char *newFileAlert, *saveCurrentAlert, *saveAsAlert, *closeFileAlert, *closeTCEAlert,
		*undoAlert, *redoAlert, *cutAlert, *copyAlert, *pasteAlert, *deleteAlert, *gotoAlert, *clipboardAlert,
		*settingAlert, *searchAlert, *replaceAlert,
		*compileAlert, *runAlert, *debugAlert;
	//�˵��Ƿ��
	bool opening = false;
	//������������
	textMap *txt;
	//��Ӧ����
	vect tmpVect;
	//�򿪲˵�
	void list(int menu);
	//�򿪴���
	void open(int window);
	//�ָ��˵��봰��
	void restore();
public:
	frame();
	friend functions;
	//��ѭ��
	void loop();
	void alert(const char *content, int type);
	friend void editor::operate(vecThree bioM);
	friend void editor::operate(int bioK, bool comp);
};

//��괦��
class mouse {
private:
	int x, y;
	int pressed;
	//��һλ������
	unsigned char oldc;
public:
	//��դ���������
	vecTwo grid();
	//��դ����������
	vecTwo grid(int x, int y);
	//�ճ�����
	void update();
	//�����ָ���һλ��
	void restore();
	//����������һλ��
	void renew();
};

bool operator==(vecTwo a, vecTwo b);
bool operator==(vecThree a, vecThree b);
LPWSTR widen(char *src);
