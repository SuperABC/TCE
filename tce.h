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

//���ڻص�����
class functions {
public:
	static void closeTCE();
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
	textMap content = { EDITOR_WIDTH, EDITOR_HEIGHT,
		(short*)malloc(EDITOR_WIDTH * EDITOR_HEIGHT * sizeof(short)) };
	const int width = EDITOR_WIDTH, height = EDITOR_HEIGHT;
	//�������
	vecTwo cursorPos;
	//������
	unsigned char *clipBoard;
	//�����Զ���ȫ
	bool complete = false;
	//���¹�����
	int endMax = 0;
	//��βλ��
	int endL[EDITOR_HEIGHT] = { 0 };
	//����λ��
	int tabS[EDITOR_HEIGHT] = { 0 };
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
