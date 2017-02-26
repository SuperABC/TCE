#include "tce.h"

extern frame *Frame;
extern mouse *Mouse;
extern editor *Editor;
extern file *File;

editor::editor() {
	cursorPos = { 0, 0 };
	for (int i = 0; i < EDITOR_WIDTH * EDITOR_HEIGHT; i++) {
		content.content[i] = ((BLUE << 12) | (YELLOW << 8) | 0);
	}
}
void editor::show() {
	putText(1, 2, &content);
}
void editor::match() {
	char *cc = (char *)malloc(MAX_KEYWORD_LEN);
	int ccn = 0;
	char tmpChar;
	vecTwo tmp = cursorPos;

	//后移一格
	tmpChar = char(content.content[tmp.y*width + tmp.x]);
	if (tmpChar < '0' || (tmpChar > '9'&&tmpChar < 'A') || (tmpChar > 'Z'&&tmpChar < 'a') || tmpChar>'z') {
		if (tmp.x == 0 && tmp.y == 0)return;
		else if (tmp.x == 0) {
			tmp.x = width - 1;
			tmp.y--;
		}
		else tmp.x--;
	}

	//匹配关键字
	tmpChar = char(content.content[tmp.y*width + tmp.x]);
	if ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
		//向后匹配
		while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
			if (tmp.x == 0 && tmp.y == 0)break;
			else if (tmp.x == 0) {
				tmp.x = width - 1;
				tmp.y--;
			}
			else tmp.x--;
			tmpChar = char(content.content[tmp.y*width + tmp.x]);
		}
		//向前匹配
		if (tmp.x == 0 && tmp.y == 0);
		else if (++tmp.x == width) {
			//到达页尾
			if (tmp.y == height - 1) {
				tmp.x--;
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
				while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
					content.content[tmp.y*width + tmp.x] &= 0xF0FF;
					content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
					if (tmp.x == 0 && tmp.y == 0)break;
					else if (tmp.x == 0) {
						tmp.x = width - 1;
						tmp.y--;
					}
					else tmp.x--;
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
				}
				return;
			}
			//未到达页尾
			else {
				tmp.x = 0;
				tmp.y++;
			}
		}
		tmpChar = char(content.content[tmp.y*width + tmp.x]);
		while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
			cc[ccn++] = tmpChar;
			if (++tmp.x == width) {
				if (tmp.y == height - 1) {
					tmp.x--;
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
					while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
						content.content[tmp.y*width + tmp.x] &= 0xF0FF;
						content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
						if (tmp.x == 0 && tmp.y == 0)break;
						else if (tmp.x == 0) {
							tmp.x = width - 1;
							tmp.y--;
						}
						else tmp.x--;
						tmpChar = char(content.content[tmp.y*width + tmp.x]);
					}
					return;
				}
				else {
					tmp.x = 0;
					tmp.y++;
				}
			}
			if (ccn >= MAX_KEYWORD_LEN)break;
			tmpChar = char(content.content[tmp.y*width + tmp.x]);
		}
		cc[ccn] = '\0';

		//关键字高亮显示
		int i;
		for (i = 0; i < 100; i++) {
			if (keyWord[1][i] != NULL && strcmp(keyWord[1][i], cc) == 0) {
				if (tmp.x == 0 && tmp.y == 0)break;
				else if (tmp.x == 0) {
					tmp.x = width - 1;
					tmp.y--;
				}
				else tmp.x--;
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
				while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
					content.content[tmp.y*width + tmp.x] &= 0xF0FF;
					content.content[tmp.y*width + tmp.x] |= (LIGHTCYAN << 8);
					if (tmp.x == 0 && tmp.y == 0)break;
					else if (tmp.x == 0) {
						tmp.x = width - 1;
						tmp.y--;
					}
					else tmp.x--;
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
				}
				break;
			}
		}
		//非关键字正常显示
		if (i == 100) {
			if (tmp.x == 0 && tmp.y == 0)return;
			else if (tmp.x == 0) {
				tmp.x = width - 1;
				tmp.y--;
			}
			else tmp.x--;
			tmpChar = char(content.content[tmp.y*width + tmp.x]);
			while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
				content.content[tmp.y*width + tmp.x] &= 0xF0FF;
				content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
				if (tmp.x == 0 && tmp.y == 0)break;
				else if (tmp.x == 0) {
					tmp.x = width - 1;
					tmp.y--;
				}
				else tmp.x--;
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
			}
		}
	}
	//匹配数字
	else if (char(content.content[tmp.y*width + tmp.x]) >= '0'&&char(content.content[tmp.y*width + tmp.x]) <= '9') {
		//向后匹配
		while (char(content.content[tmp.y*width + tmp.x]) >= '0'&&char(content.content[tmp.y*width + tmp.x]) <= '9') {
			if (tmp.x == 0 && tmp.y == 0)break;
			else if (tmp.x == 0) {
				tmp.x = width - 1;
				tmp.y--;
			}
			else tmp.x--;
			//当前为字母数字组合
			tmpChar = char(content.content[tmp.y*width + tmp.x]);
			if ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z')) {
				//向后匹配
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
				while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z') || (tmpChar >= '0' && tmpChar <= '9')) {
					if (tmp.x == 0 && tmp.y == 0)break;
					else if (tmp.x == 0) {
						tmp.x = width - 1;
						tmp.y--;
					}
					else tmp.x--;
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
				}
				//向前匹配
				if (tmp.x == 0 && tmp.y == 0);
				else if (++tmp.x == width) {
					//到达页尾
					if (tmp.y == height - 1) {
						tmp.x--;
						tmpChar = char(content.content[tmp.y*width + tmp.x]);
						while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z') || (tmpChar >= '0'&&tmpChar <= '9')) {
							content.content[tmp.y*width + tmp.x] &= 0xF0FF;
							content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
							if (tmp.x == 0 && tmp.y == 0)break;
							else if (tmp.x == 0) {
								tmp.x = width - 1;
								tmp.y--;
							}
							else tmp.x--;
							tmpChar = char(content.content[tmp.y*width + tmp.x]);
						}
						return;
					}
					//未到达页尾
					else {
						tmp.x = 0;
						tmp.y++;
					}
				}
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
				while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z') || (tmpChar >= '0' && tmpChar <= '9')) {
					if (++tmp.x == width) {
						//到达页尾
						if (tmp.y == height - 1) {
							tmp.x--;
							tmpChar = char(content.content[tmp.y*width + tmp.x]);
							while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z') || (tmpChar >= '0'&&tmpChar <= '9')) {
								content.content[tmp.y*width + tmp.x] &= 0xF0FF;
								content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
								if (tmp.x == 0 && tmp.y == 0)break;
								else if (tmp.x == 0) {
									tmp.x = width - 1;
									tmp.y--;
								}
								else tmp.x--;
								tmpChar = char(content.content[tmp.y*width + tmp.x]);
							}
							return;
						}
						//未到达页尾
						else {
							tmp.x = 0;
							tmp.y++;
						}
					}
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
				}
				//去除高亮
				if (tmp.x == 0 && tmp.y == 0)break;
				else if (tmp.x == 0) {
					tmp.x = width - 1;
					tmp.y--;
				}
				else tmp.x--;
				tmpChar = char(content.content[tmp.y*width + tmp.x]);
				while ((tmpChar >= 'a'&&tmpChar <= 'z') || (tmpChar >= 'A'&&tmpChar <= 'Z') || (tmpChar >= '0' && tmpChar <= '9')) {
					content.content[tmp.y*width + tmp.x] &= 0xF0FF;
					content.content[tmp.y*width + tmp.x] |= (YELLOW << 8);
					if (tmp.x == 0 && tmp.y == 0)break;
					else if (tmp.x == 0) {
						tmp.x = width - 1;
						tmp.y--;
					}
					else tmp.x--;
					tmpChar = char(content.content[tmp.y*width + tmp.x]);
				}
				return;
			}
		}
		//向前匹配
		if (tmp.x == 0 && tmp.y == 0);
		else if (++tmp.x == width) {
			if (tmp.y == height - 1) {
				tmp.x--;
			}
			else {
				tmp.x = 0;
				tmp.y++;
			}
		}
		while (char(content.content[tmp.y*width + tmp.x]) >= '0'&&char(content.content[tmp.y*width + tmp.x]) <= '9') {
			if (++tmp.x == width) {
				if (tmp.y == height - 1) {
					tmp.x--;
					break;
				}
				else {
					tmp.x = 0;
					tmp.y++;
				}
			}
			if (char(content.content[tmp.y*width + tmp.x]) >= 'a'&&char(content.content[tmp.y*width + tmp.x]) <= 'z')return;
		}
		//数字高亮显示
		if (tmp.x == 0 && tmp.y == 0)return;
		else if (tmp.x == 0) {
			tmp.x = width - 1;
			tmp.y--;
		}
		else tmp.x--;
		while (char(content.content[tmp.y*width + tmp.x]) >= '0'&&char(content.content[tmp.y*width + tmp.x]) <= '9') {
			content.content[tmp.y*width + tmp.x] &= 0xF0FF;
			content.content[tmp.y*width + tmp.x] |= (LIGHTGREEN << 8);
			if (tmp.x == 0 && tmp.y == 0)return;
			else if (tmp.x == 0) {
				tmp.x = width - 1;
				tmp.y--;
			}
			else tmp.x--;
		}
		return;
	}
}
void editor::check() {
	int j;
	//检测行尾位置
	for (int i = 0; i < height; i++) {
		for (j = width; j > 0; j--) {
			if (char(content.content[i*width + j - 1]))break;
		}
		endL[i] = j;
	}
	//检测行首位置
	for (int i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (char(content.content[i*width + j]) != ' ')break;
		}
		tabS[i] = j;
	}
}
void editor::cursor() {
	static int t = 0;
	t = (t + 1) % 160;
	if (t / 100)setCharBgc(LIGHTGRAY, cursorPos.x+1, cursorPos.y+2);
	else setCharBgc(BLUE, cursorPos.x + 1, cursorPos.y + 2);
}
void editor::suspend() {
	setCharBgc(BLUE, cursorPos.x + 1, cursorPos.y + 2);
}
void editor::clear() {
	for (int i = 0; i < width*height; i++) {
		content.content[i] = BLUE << 12 | YELLOW << 8;
	}
}
int editor::putRow(int pos, int line) {
	vecTwo tmp = cursorPos;
	cursorPos.y = line;
	cursorPos.x = 0;
	for (int i = 0; i < width; i++) {
		content.content[cursorPos.y*width + cursorPos.x] = (BLUE << 12 | YELLOW << 8);
	}
	while (File->fileContent[++pos] != '\r'&&cursorPos.x < width - 1) {
		operate(File->fileContent[pos], false);
		if (pos == File->fileLength)return pos;
	}
	for (int i = cursorPos.x; i < width; i++) {
		content.content[line*width + i] &= 0xFF00;
	}
	if (cursorPos.x == width - 1 && File->fileContent[pos] != '\r') {
		content.content[line*width + width - 1] &= 0xFF00;
		content.content[line*width + width - 1] |= File->fileContent[pos];
	}
	cursorPos = tmp;
	return pos;
}
void editor::operate(int bioK, bool comp) {
	if (bioK >= 0x20 && bioK < 0x80) {
		char out = (endL[cursorPos.y] == width) ? content.content[cursorPos.y*width + width - 1] : 0;
		//当前行即将填满且不是最后一行，进行整体下移
		if ((endL[cursorPos.y] == width - 1) && (cursorPos.y < height - 1)) {
			for (int i = height - 2; i > cursorPos.y; i--)memcpy(content.content + (i + 1)*width, content.content + i*width, width * sizeof(short));
			for (int i = 0; i < width; i++)content.content[(cursorPos.y + 1)*width + i] &= 0xFF00;
		}
		//当前行已满且不是最后一行，尾部字符递归填入下一行
		if ((endL[cursorPos.y] == width) && (cursorPos.y < height - 1)) {
			vecTwo tmp = cursorPos;
			cursorPos.x = 0;
			cursorPos.y++;
			operate(out);
			cursorPos = tmp;
		}
		//当前行填入字符
		for (int i = width - 1; i > cursorPos.x; i--)content.content[cursorPos.y*width + i] = content.content[cursorPos.y*width + i - 1];
		content.content[cursorPos.y*width + cursorPos.x] &= 0xFF00;
		content.content[cursorPos.y*width + cursorPos.x] |= bioK;
		cursorPos.x = (cursorPos.x + 1) % width;
		cursorPos.y += (cursorPos.x ? 0 : 1);
		check();

		//括号自动补全
		if (comp) {
			if (bioK == '(') { operate(')'); operate(SG_LEFT); complete = true; }
			if (bioK == '[') { operate(']'); operate(SG_LEFT); complete = true; }
			if (bioK == '{') { operate('}'); operate(SG_LEFT); complete = true; }
			if (bioK == ')'&&complete) { operate(SG_BACKS); operate(SG_RIGHT); complete = false; }
			if (bioK == ']'&&complete) { operate(SG_BACKS); operate(SG_RIGHT); complete = false; }
			if (bioK == '}'&&complete) { operate(SG_BACKS); operate(SG_RIGHT); complete = false; }
		}

		//语法高亮
		if ((bioK >= 'a'&&bioK <= 'z') || (bioK >= 'A'&&bioK <= 'Z')) match();
		else if (bioK >= '0' && bioK <= '9') match();
		else {
			vecTwo tmp = cursorPos;
			if (char(content.content[cursorPos.y*width + cursorPos.x])&&(cursorPos.y*width + cursorPos.x!=width*height-1)) {
				if (++cursorPos.x == width) {
					cursorPos.x = 0;
					cursorPos.y++;
				}
				match();
				if (cursorPos.x == 0 && cursorPos.y == 0);
				else if (cursorPos.x == 0) {
					cursorPos.x = width - 1;
					cursorPos.y--;
				}
				else cursorPos.x--;
			}
			if (cursorPos.x == 0 && cursorPos.y == 0);
			else if (cursorPos.x == 0) {
				cursorPos.x = width - 1;
				cursorPos.y--;
			}
			else cursorPos.x--;
			content.content[cursorPos.y*width + cursorPos.x] &= 0xF0FF;
			content.content[cursorPos.y*width + cursorPos.x] |= (LIGHTGRAY << 8);

			if (cursorPos.x == 0 && cursorPos.y == 0);
			else if (cursorPos.x == 0) {
				cursorPos.x = width - 1;
				cursorPos.y--;
			}
			else cursorPos.x--;
			match();

			cursorPos = tmp;
		}

		show();
	}
	if (bioK < 0) {
		operate(' ');
	}
	if (bioK == SG_TAB) {
		operate(' ');
		operate(' ');
		operate(' ');
		operate(' ');
	}
	if (bioK == SG_BACKS) {
		complete = false;

		//行首退格
		if (cursorPos.x == 0) {
			if (cursorPos.y != 0) {
				//退格后前一行填满
				if (endL[cursorPos.y] + endL[cursorPos.y - 1] > width) {
					//前一行已满
					if (endL[cursorPos.y - 1] == width) {
						content.content[cursorPos.y*width - 1] = content.content[cursorPos.y*width];
						for (int i = 1; i < width; i++) {
							content.content[cursorPos.y*width + i - 1] = content.content[cursorPos.y*width + i];
						}
						content.content[(cursorPos.y + 1)*width - 1] &= 0xFF00;
					}
					//前一行未满
					else {
						memcpy(content.content + (cursorPos.y - 1)*width + endL[cursorPos.y - 1], content.content + cursorPos.y*width, (width - endL[cursorPos.y - 1]) * sizeof(short));
						memcpy(content.content + cursorPos.y*width, content.content + cursorPos.y*width + (width - endL[cursorPos.y - 1]), endL[cursorPos.y - 1] * sizeof(short));
						for (int i = endL[cursorPos.y - 1]; i < width; i++) {
							content.content[cursorPos.y*width + i] &= 0xFF00;
						}
					}

					//当前行已满，递归退格下一行
					if ((endL[cursorPos.y] == width) && (cursorPos.y < height - 1)) {
						endL[cursorPos.y] = endL[cursorPos.y - 1];
						endL[cursorPos.y - 1] = width;
						vecTwo tmp = cursorPos;
						cursorPos.y++;
						operate(SG_BACKS);
						cursorPos = tmp;
					}
				}
				//退格后前一行仍未满
				else {
					if (endL[cursorPos.y - 1] == width)content.content[cursorPos.y*width - 1] &= 0xFF00;
					memcpy(content.content + (cursorPos.y - 1)*width + endL[cursorPos.y - 1], content.content + cursorPos.y*width, endL[cursorPos.y] * sizeof(short));
					for (int i = cursorPos.y; i < height - 1; i++)memcpy(content.content + i*width, content.content + (i + 1)*width, width * sizeof(short));
					for (int i = 0; i < width; i++)content.content[(height - 1)*width + i] &= 0xFF00;
				}
				cursorPos.x = endL[cursorPos.y - 1] < width ? endL[cursorPos.y - 1] : width - 1;
				cursorPos.y -= 1;
			}
		}
		//普通退格
		else {
			if (endL[cursorPos.y] >= cursorPos.x) {
				for (int i = cursorPos.x; i < width; i++) {
					content.content[cursorPos.y*width + i - 1] = content.content[cursorPos.y*width + i];
				}
				content.content[(cursorPos.y + 1)*width - 1] &= 0xFF00;

				//当前行已满，递归退格下一行
				if ((endL[cursorPos.y] == width)&&(cursorPos.y < height - 1)) {
					vecTwo tmp = cursorPos;
					cursorPos.x = 0;
					cursorPos.y++;
					operate(SG_BACKS);
					cursorPos = tmp;
				}
			}
			cursorPos.x--;
		}
		check();

		//语法高亮检查
		match();
		show();
	}
	if (bioK == SG_ENTER) {
		complete = false;

		//页尾换行
		if (cursorPos.y == height - 1) {
			for (int i = 1; i < height; i++) {
				memcpy(content.content + (i - 1)*width, content.content + i*width, width * sizeof(short));
			}
			for (int i = 0; i < width; i++) {
				content.content[cursorPos.y*width + i] &= 0xFF00;
			}
			memcpy(content.content + (height - 1)*width, content.content + (height - 2)*width + cursorPos.x, (width - cursorPos.x) * sizeof(short));
			for (int i = cursorPos.x; i < width; i++) {
				content.content[(cursorPos.y-1)*width + i] &= 0xFF00;
			}
			cursorPos.x = 0;
		}
		//普通换行
		else {
			if (endL[cursorPos.y] < width) {
				for (int i = height - 2; i > cursorPos.y; i--) {
					memcpy(content.content + (i + 1)*width, content.content + i*width, width * sizeof(short));
				}
				for (int i = 0; i < width; i++) {
					content.content[(cursorPos.y + 1)*width + i] &= 0xFF00;
				}
				memcpy(content.content + (cursorPos.y + 1)*width, content.content + cursorPos.y*width + cursorPos.x, (width - cursorPos.x) * sizeof(short));
				for (int i = cursorPos.x; i < width; i++) {
					content.content[cursorPos.y*width + i] &= 0xFF00;
				}
			}
			else if (endL[cursorPos.y] == width && (endL[cursorPos.y + 1] + endL[cursorPos.y] - cursorPos.x < width)) {
				memcpy(content.content + (cursorPos.y + 1)*width + (endL[cursorPos.y] - cursorPos.x), content.content + (cursorPos.y + 1)*width, endL[cursorPos.y + 1] * sizeof(short));
				memcpy(content.content + (cursorPos.y + 1)*width, content.content + cursorPos.y*width + cursorPos.x, (endL[cursorPos.y] - cursorPos.x) * sizeof(short));
				for (int i = cursorPos.x; i < width; i++) {
					content.content[cursorPos.y*width + i] &= 0xFF00;
				}
			}
			else {
				if (cursorPos.y <= height - 2) {
					if (cursorPos.y < height - 2) {
						vecTwo tmp = cursorPos;
						cursorPos.y++;
						operate(SG_ENTER);
						cursorPos = tmp;
					}
					memcpy(content.content + (cursorPos.y + 1)*width + (endL[cursorPos.y] - cursorPos.x), content.content + (cursorPos.y + 1)*width, endL[cursorPos.y + 1] * sizeof(short));
					memcpy(content.content + (cursorPos.y + 1)*width, content.content + cursorPos.y*width + cursorPos.x, (endL[cursorPos.y] - cursorPos.x) * sizeof(short));
					for (int i = cursorPos.x; i < width; i++) {
						content.content[cursorPos.y*width + i] &= 0xFF00;
					}
				}
			}
			cursorPos.y++;
			cursorPos.x = 0;
		}
		check();

		//自动缩进
		if (comp) {
			for (int i = 0; i < tabS[cursorPos.y - 1]; i++)operate(' ');
			if (char(content.content[cursorPos.y*width + cursorPos.x]) == '}') {
				cursorPos.y--;
				cursorPos.x = endL[cursorPos.y];
				operate(SG_ENTER);
				operate(SG_TAB);
			}
		}

		//高亮检测
		vecTwo tmp = cursorPos;
		cursorPos.x++;
		match();
		cursorPos.y--;
		cursorPos.x = endL[tmp.y];
		match();
		cursorPos = tmp;
		show();
	}
	if (bioK == SG_UP) {
		complete = false;
		if (cursorPos.y == 0) {
			if (File->editorBegin == 0)return;
			if (File->posContent(--File->editorBegin) == '\n')File->editorBegin -= 2;
			if (File->editorBegin < 0) {
				File->editorBegin = 0;
				return;
			}
			if (File->posContent(File->editorBegin) == '\n')File->editorBegin++;
			int lineEnd = File->editorBegin, tabNum = 0;
			while (File->editorBegin && File->posContent(--File->editorBegin) != '\n') {
				if (File->posContent(File->editorBegin) == '\t')tabNum++;
			}
			int deltaChar = (lineEnd - File->editorBegin + tabNum*3 - 1) % width + 1;
			tabNum = 0;
			for (int i = 0; i < deltaChar; i++) {
				if (File->posContent(lineEnd - i) == '\t') {
					tabNum++;
					deltaChar -= 3;
				}
			}
			File->editorBegin = (lineEnd - deltaChar > 0) ? (lineEnd - deltaChar) + 1 : (lineEnd - deltaChar);
			File->editorEnd -= endL[height - 1] - tabS[height - 1] + 1;
			while (File->posContent(File->editorEnd) == '\t')File->editorEnd--;
			if (File->posContent(File->editorEnd) == '\n')File->editorEnd--;
			for (int i = height - 1; i > 0; i--) {
				memcpy(content.content + i*width, content.content + (i - 1)*width, width * sizeof(short));
			}
			for (int i = 0; i < width; i++) {
				content.content[i] &= 0xFF00;
			}
			putRow(File->editorBegin-1, 0);
		}
		else cursorPos.y--;
		check();
		if (endL[cursorPos.y] < endMax)cursorPos.x = endL[cursorPos.y];
		else cursorPos.x = endMax;
		show();
	}
	if (bioK == SG_DOWN) {
		complete = false;
		if (cursorPos.y == height - 1) {
			if (File->editorEnd >= File->fileLength)return;
			for (int i = 0; i < width; i++) {
				if (!char(content.content[i])) { File->editorBegin += 2; break; }
				else {
					if (File->posContent(File->editorBegin) == '\t')i += 3;
					File->editorBegin++;
				}
			}
			for (int i = 1; i < height; i++) {
				memcpy(content.content + (i - 1)*width, content.content + i*width, width * sizeof(short));
			}
			for (int i = 0; i < width; i++) {
				content.content[(height-1)*width + i] &= 0xFF00;
			}
			File->editorEnd = putRow(File->editorEnd, height - 1);
		}
		else cursorPos.y++;
		check();
		if (endL[cursorPos.y] < endMax)cursorPos.x = endL[cursorPos.y];
		else cursorPos.x = endMax;
		show();
	}
	if (bioK == SG_LEFT) {
		complete = false;
		if (cursorPos.x == 0) {
			if (cursorPos.y != 0) {
				cursorPos.y--;
				cursorPos.x = endL[cursorPos.y];
			}
		}
		else cursorPos.x--;
		endMax = cursorPos.x;
		show();
	}
	if (bioK == SG_RIGHT) {
		complete = false;
		if (cursorPos.x == endL[cursorPos.y]) {
			if (cursorPos.y < height - 1) {
				cursorPos.y++;
				cursorPos.x = 0;
			}
			else if(File->editorEnd<File->fileLength){
				operate(SG_DOWN);
				operate(SG_UP);
				operate(SG_RIGHT);
			}
		}
		else cursorPos.x++;
		endMax = cursorPos.x;
		show();
	}
}
void editor::operate(vecThree bioM) {
	complete = false;
	vecTwo tmp = Mouse->grid(bioM.x, bioM.y);
	if (tmp.x<1 || tmp.x>width)return;
	if (tmp.y<2 || tmp.y>height + 1)return;
	setCharBgc(BLUE, cursorPos.x + 1, cursorPos.y + 2);
	cursorPos.y = tmp.y - 2;
	cursorPos.x = tmp.x - 1 < endL[cursorPos.y] ? tmp.x - 1 : endL[cursorPos.y];
	endMax = cursorPos.x;
}
void editor::stroll(int dir) {
	if (dir == SG_MIDDLE_BUTTON_UP) {
		vecTwo tmp = cursorPos;
		cursorPos.x = 0;
		cursorPos.y = 0;
		operate(SG_UP);
		operate(SG_UP);
		operate(SG_UP);
		cursorPos = tmp;
	}
	if (dir == SG_MIDDLE_BUTTON_DOWN) {
		vecTwo tmp = cursorPos;
		cursorPos.x = 0;
		cursorPos.y = height-1;
		operate(SG_DOWN);
		operate(SG_DOWN);
		operate(SG_DOWN);
		cursorPos = tmp;
	}
}

void voidFunc(void) {
	return;
}
frame::frame() {
	ctrling = shifting = alting = draging = 0;
	menu = window = alertReturn = 0;
	tmpVect = voidFunc;
	setBfc(LIGHTGRAY, BLACK);
	writeString("  File    Edit   Tools    Run     Help                                                                                          ", 0, 0);
	setCharColor((char)(LIGHTGRAY << 4 | RED), 2, 0);
	setCharColor((char)(LIGHTGRAY << 4 | RED), 10, 0);
	setCharColor((char)(LIGHTGRAY << 4 | RED), 17, 0);
	setCharColor((char)(LIGHTGRAY << 4 | RED), 26, 0);
	setCharColor((char)(LIGHTGRAY << 4 | RED), 34, 0);
	setBfc(BLUE, YELLOW);
	for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		writeChar(' ', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	setBfc(LIGHTBLUE, WHITE);
	for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH*2; i++)
		writeChar('=', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	for (int i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1) - 1; i > SCREEN_WIDTH * (SCREEN_HEIGHT - 2) - 1; i--)
		writeChar('=', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 2); i+= SCREEN_WIDTH)
		writeChar('|', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	for (int i = SCREEN_WIDTH * 2 - 2; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 2); i += SCREEN_WIDTH)
		writeChar('|', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	for (int i = SCREEN_WIDTH * 2 - 1; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i+= SCREEN_WIDTH)
		writeChar(' ', i % SCREEN_WIDTH, i / SCREEN_WIDTH);
	writeChar('*', 0, 1);
	writeChar('*', 0, SCREEN_HEIGHT - 2);
	writeChar('*', SCREEN_WIDTH - 2, 1);
	writeChar('*', SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2);
}
void frame::loop() {
	LoadKeyboardLayout(widen("0x0409"), KLF_ACTIVATE | KLF_SETFORPROCESS);
	Mouse->update();

	vecThree click = { -1, -1, -1 };
	int key = 0;

	if(!opening)Editor->cursor();

	if (biosMouse(1).m)click = biosMouse(0);
	if (biosKey(1))key = biosKey(0);

	//响应键盘动态
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
	case SG_ESC | 0x8000:
		if (opening) {
			opening = false;
			restore();
		}
		break;
	case SG_F4 | 0x8000:
		exit(0);
		break;
	}
	if (key&&!opening)Editor->operate(key^0x8000);

	if (click.x < 0 || click.y < 0)return;
	vecTwo clickF = Mouse->grid(click.x, click.y);

	//响应编辑时鼠标动态
	if (!opening) {
		if (inRect(clickF.x, clickF.y, 0, 0, 7, 0))list(FILES);
		else if (inRect(clickF.x, clickF.y, 8, 0, 15, 0))list(EDIT);
		else if (inRect(clickF.x, clickF.y, 16, 0, 23, 0))list(TOOLS);
		else if (inRect(clickF.x, clickF.y, 24, 0, 31, 0))list(RUN);
		else if (inRect(clickF.x, clickF.y, 32, 0, 39, 0))list(HELP);
		if (click.m == SG_LEFT_BUTTON)Editor->operate(click);
		if ((click.m == SG_MIDDLE_BUTTON_UP) || (click.m == SG_MIDDLE_BUTTON_DOWN))Editor->stroll(click.m);
	}
	//响应列表时鼠标动态
	else if (menu != NONE) {
		//列表切换
		if (inRect(clickF.x, clickF.y, 0, 0, 7, 0)) {
			int tmp = menu;
			restore();
			if(tmp != FILES)list(FILES);
			else {
				opening = false;
				menu = NONE;
			}
		}
		else if (inRect(clickF.x, clickF.y, 8, 0, 15, 0)) {
			int tmp = menu;
			restore();
			if(tmp != EDIT)list(EDIT);
			else {
				opening = false;
				menu = NONE;
			}
		}
		else if (inRect(clickF.x, clickF.y, 16, 0, 23, 0)) {
			int tmp = menu;
			restore();
			if(tmp != TOOLS)list(TOOLS);
			else {
				opening = false;
				menu = NONE;
			}
		}
		else if (inRect(clickF.x, clickF.y, 24, 0, 31, 0)) {
			int tmp = menu;
			restore();
			if(tmp != RUN)list(RUN);
			else {
				opening = false;
				menu = NONE;
			}
		}
		else if (inRect(clickF.x, clickF.y, 32, 0, 39, 0)) {
			int tmp = menu;
			restore();
			if(tmp != HELP)list(HELP);
			else {
				opening = false;
				menu = NONE;
			}
		}

		//文件相关窗口
		if (menu == FILES) {
			if (inRect(clickF.x, clickF.y, 0, 2, 19, 2)) {
				alert(newFileAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 0, 3, 19, 3)) {
				restore();
				opening = false;
				menu = NONE;
				File->openFile("tce.h");
			}
			else if (inRect(clickF.x, clickF.y, 0, 4, 19, 4)) {
				alert(saveCurrentAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 0, 5, 19, 5)) {
				alert(saveAsAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 0, 7, 19, 7)) {
				restore();
				opening = false;
				menu = NONE;
				File->closeFile();
				Editor->show();
			}
			else if (inRect(clickF.x, clickF.y, 0, 8, 19, 8)) {
				alert(closeTCEAlert, OK_BUTTON | CANCEL_BUTTON);
				tmpVect = functions::closeTCE;
			}
		}
		//编辑相关窗口
		if (menu == EDIT) {
			if (inRect(clickF.x, clickF.y, 8, 2, 27, 2)) {
				alert(undoAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 3, 27, 3)) {
				alert(redoAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 5, 27, 5)) {
				alert(cutAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 6, 27, 6)) {
				alert(copyAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 7, 27, 7)) {
				alert(pasteAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 8, 27, 8)) {
				alert(deleteAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 9, 27, 9)) {
				alert(gotoAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 8, 11, 27, 11)) {
				alert(clipboardAlert, OK_BUTTON);
			}
		}
		//工具相关窗口
		if (menu == TOOLS) {
			if (inRect(clickF.x, clickF.y, 16, 2, 35, 2)) {
				alert(settingAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 16, 4, 35, 4)) {
				alert(searchAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 16, 5, 35, 5)) {
				alert(replaceAlert, OK_BUTTON);
			}
		}
		//运行相关窗口
		if (menu == RUN) {
			if (inRect(clickF.x, clickF.y, 24, 2, 43, 2)) {
				alert(compileAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 24, 3, 43, 3)) {
				alert(runAlert, OK_BUTTON);
			}
			else if (inRect(clickF.x, clickF.y, 24, 5, 43, 5)) {
				alert(debugAlert, OK_BUTTON);
			}
		}
		//帮助相关窗口
		if (menu == HELP) {
			if (inRect(clickF.x, clickF.y, 32, 2, 51, 2))open(HELP_VERSION);
			//else if (inRect(clickF.x, clickF.y, 32, 3, 51, 3))open(HELP_HELP);
		}
	}
	//响应窗口时鼠标动态
	else {
		//关闭版本窗口
		if (window == HELP_VERSION&&clickF.x == SCREEN_WIDTH / 2 + 13 && clickF.y == 5) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			opening = false;
		}

		//关闭提示窗口
		if (window == ALERT_MESSAGE&&clickF.x == SCREEN_WIDTH / 2 + 19 && clickF.y == 5) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			alertReturn = NO_BUTTON;
			opening = false;
			tmpVect();
			tmpVect = voidFunc;
		}
		//单确定窗口点击确定
		if (window == ALERT_MESSAGE
			&&((alertReturn & OK_BUTTON) && !(alertReturn & CANCEL_BUTTON))
			&& inRect(clickF.x, clickF.y, SCREEN_WIDTH / 2 - 4, 13, SCREEN_WIDTH / 2 + 3, 13)) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			alertReturn = OK_BUTTON;
			opening = false;
			tmpVect();
			tmpVect = voidFunc;
		}
		//单取消窗口点击取消
		if (window == ALERT_MESSAGE
			&& (!(alertReturn & OK_BUTTON) && (alertReturn & CANCEL_BUTTON))
			&& inRect(clickF.x, clickF.y, SCREEN_WIDTH / 2 - 4, 13, SCREEN_WIDTH / 2 + 3, 13)) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			alertReturn = CANCEL_BUTTON;
			opening = false;
			tmpVect();
			tmpVect = voidFunc;
		}
		//确定取消窗口点击确定
		if (window == ALERT_MESSAGE
			&& ((alertReturn & OK_BUTTON) && (alertReturn & CANCEL_BUTTON))
			&& inRect(clickF.x, clickF.y, SCREEN_WIDTH / 2 - 12, 13, SCREEN_WIDTH / 2 - 5, 13)) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			alertReturn = OK_BUTTON;
			opening = false;
			tmpVect();
			tmpVect = voidFunc;
		}
		//确定取消窗口点击取消
		if (window == ALERT_MESSAGE
			&& ((alertReturn & OK_BUTTON) && (alertReturn & CANCEL_BUTTON))
			&& inRect(clickF.x, clickF.y, SCREEN_WIDTH / 2 + 4, 13, SCREEN_WIDTH / 2 + 11, 13)) {
			restore();
			menu = NONE;
			window = EMPTY_WINDOW;
			alertReturn = CANCEL_BUTTON;
			opening = false;
			tmpVect();
			tmpVect = voidFunc;
		}
	}
}
void frame::list(int menu) {
	static int menuHeight[5] = { 7, 10, 4, 4, 2 };
	txt = (textMap*)malloc(sizeof(textMap));
	opening = true;
	this->menu = menu;

	Editor->suspend();
	Mouse->restore();

	getText(8 * (menu - 1), 0, 8 * menu + 15, menuHeight[menu - 1] + 3, txt);
	for (int i = 0; i < 8; i++)setCharBgc(LIGHTGREEN, (menu - 1) * 8 + i, 0);
	setBfc(LIGHTGRAY, BLACK);
	switch (menu) {
	case FILES:
		writeString("--------------------", 8 * (menu - 1), 1);
		writeString("* New file      F1 *", 8 * (menu - 1), 2);
		writeString("* Open file     F2 *", 8 * (menu - 1), 3);
		writeString("* Save current  F3 *", 8 * (menu - 1), 4);
		writeString("* Save as          *", 8 * (menu - 1), 5);
		writeString("* ---------------- *", 8 * (menu - 1), 6);
		writeString("* Close file       *", 8 * (menu - 1), 7);
		writeString("* Close TCE     F4 *", 8 * (menu - 1), 8);
		writeString("--------------------", 8 * (menu - 1), 9);
		break;
	case EDIT:
		writeString("--------------------", 8 * (menu - 1), 1);
		writeString("* Undo      Ctrl+Z *", 8 * (menu - 1), 2);
		writeString("* Redo      Ctrl+Y *", 8 * (menu - 1), 3);
		writeString("* ---------------- *", 8 * (menu - 1), 4);
		writeString("* Cut       Ctrl+X *", 8 * (menu - 1), 5);
		writeString("* Copy      Ctrl+C *", 8 * (menu - 1), 6);
		writeString("* Paste     Ctrl+V *", 8 * (menu - 1), 7);
		writeString("* Delete           *", 8 * (menu - 1), 8);
		writeString("* Goto      Ctrl+G *", 8 * (menu - 1), 9);
		writeString("* ---------------- *", 8 * (menu - 1), 10);
		writeString("* Clipboard        *", 8 * (menu - 1), 11);
		writeString("--------------------", 8 * (menu - 1), 12);
		break;
	case TOOLS:
		writeString("--------------------", 8 * (menu - 1), 1);
		writeString("* Setting       F6 *", 8 * (menu - 1), 2);
		writeString("* ---------------- *", 8 * (menu - 1), 3);
		writeString("* Search           *", 8 * (menu - 1), 4);
		writeString("* Replace          *", 8 * (menu - 1), 5);
		writeString("--------------------", 8 * (menu - 1), 6);
		break;
	case RUN:
		writeString("--------------------", 8 * (menu - 1), 1);
		writeString("* Compile       F5 *", 8 * (menu - 1), 2);
		writeString("* Run          F10 *", 8 * (menu - 1), 3);
		writeString("* ---------------- *", 8 * (menu - 1), 4);
		writeString("* Debug         F9 *", 8 * (menu - 1), 5);
		writeString("--------------------", 8 * (menu - 1), 6);
		break;
	case HELP:
		writeString("--------------------", 8 * (menu - 1), 1);
		writeString("* Version          *", 8 * (menu - 1), 2);
		writeString("* Help             *", 8 * (menu - 1), 3);
		writeString("--------------------", 8 * (menu - 1), 4);
		break;
	}

	Mouse->renew();
}
void frame::open(int window) {
		restore();
		Mouse->restore();

		switch (window) {
		case HELP_VERSION:
			getText(SCREEN_WIDTH / 2 - 14, 5, SCREEN_WIDTH / 2 + 13, 14, txt);
			setBfc(LIGHTGRAY, BLACK);
			writeString("*--------------------------X", SCREEN_WIDTH / 2 - 14, 5);
			writeString("|  TC Emulater v0.0        |", SCREEN_WIDTH / 2 - 14, 6);
			writeString("|                          |", SCREEN_WIDTH / 2 - 14, 7);
			writeString("|  This is the first ver-  |", SCREEN_WIDTH / 2 - 14, 8);
			writeString("|  sion of TCE. Any sugg-  |", SCREEN_WIDTH / 2 - 14, 9);
			writeString("|  estions are welcomed.   |", SCREEN_WIDTH / 2 - 14, 10);
			writeString("|                          |", SCREEN_WIDTH / 2 - 14, 11);
			writeString("|                          |", SCREEN_WIDTH / 2 - 14, 12);
			writeString("|                          |", SCREEN_WIDTH / 2 - 14, 13);
			writeString("*--------------------------*", SCREEN_WIDTH / 2 - 14, 14);
			setCharColor(char(LIGHTRED << 4 | WHITE), SCREEN_WIDTH / 2 + 13, 5);
			break;
		}

		Mouse->renew();
		menu = NONE;
		this->window = window;
}
void frame::alert(const char *content, int type) {
	const int width = 34, height = 7;
	restore();
	Mouse->restore();

	char *alertLine[height];
	for (int i = 0; i < height; i++) {
		alertLine[i] = (char *)malloc((width+1) * sizeof(char));
		for (int j = 0; j < width; j++) {
			alertLine[i][j] = ' ';
		}
		alertLine[i][width] = '\0';
	}
	vecTwo alertPos = { 0, 0 };
	for (GLuint i = 0; i < strlen(content); i++) {
		if (++alertPos.x == width || content[i] == '\n') {
			alertPos.x = 0;
			alertPos.y++;
			if (content[i] != '\n')i--;
			continue;
		}
		if (alertPos.y == height)break;
		alertLine[alertPos.y][alertPos.x] = content[i];
	}

	getText(SCREEN_WIDTH / 2 - 20, 5, SCREEN_WIDTH / 2 + 19, 14, txt);
	setBfc(LIGHTGRAY, BLACK);
	writeString("*--------------------------------------X", SCREEN_WIDTH / 2 - 20, 5);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 6); writeString(alertLine[0], SCREEN_WIDTH / 2 - 17, 6); writeString("  |", SCREEN_WIDTH / 2 + 17, 6);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 7); writeString(alertLine[1], SCREEN_WIDTH / 2 - 17, 7); writeString("  |", SCREEN_WIDTH / 2 + 17, 7);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 8); writeString(alertLine[2], SCREEN_WIDTH / 2 - 17, 8); writeString("  |", SCREEN_WIDTH / 2 + 17, 8);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 9); writeString(alertLine[3], SCREEN_WIDTH / 2 - 17, 9); writeString("  |", SCREEN_WIDTH / 2 + 17, 9);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 10); writeString(alertLine[4], SCREEN_WIDTH / 2 - 17, 10); writeString("  |", SCREEN_WIDTH / 2 + 17, 10);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 11); writeString(alertLine[5], SCREEN_WIDTH / 2 - 17, 11); writeString("  |", SCREEN_WIDTH / 2 + 17, 11);
	writeString("|  ", SCREEN_WIDTH / 2 - 20, 12); writeString(alertLine[6], SCREEN_WIDTH / 2 - 17, 12); writeString("  |", SCREEN_WIDTH / 2 + 17, 12);
	writeString("|                                      |", SCREEN_WIDTH / 2 - 20, 13);
	writeString("*--------------------------------------*", SCREEN_WIDTH / 2 - 20, 14);
	setCharColor(char(LIGHTRED << 4 | WHITE), SCREEN_WIDTH / 2 + 19, 5);

	setBfc(LIGHTGREEN, CYAN);
	if((type&OK_BUTTON)&&!(type&CANCEL_BUTTON))
		writeString(" O    K ", SCREEN_WIDTH / 2 - 4, 13);
	else if (!(type&OK_BUTTON) && (type&CANCEL_BUTTON))
		writeString(" CANCEL ", SCREEN_WIDTH / 2 - 4, 13);
	else if ((type&OK_BUTTON) && (type&CANCEL_BUTTON)) {
		writeString(" O    K ", SCREEN_WIDTH / 2 - 12, 13);
		writeString(" CANCEL ", SCREEN_WIDTH / 2 + 4, 13);
	}

	Mouse->renew();
	menu = NONE;
	window = ALERT_MESSAGE;
	alertReturn = type;
}
void frame::restore() {
	static vecTwo leftTop[11] = { {0, 0}, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { SCREEN_WIDTH / 2 - 14, 5 }, { 0, 0 }, { SCREEN_WIDTH / 2 - 20, 5 } };
	Mouse->restore();

	//恢复列表
	if (menu != NONE) {
		putText(8 * (menu - 1), 0, txt);
		menu = NONE;
	}

	//恢复窗口
	else {
		putText(leftTop[window].x, leftTop[window].y, txt);
		window = EMPTY_WINDOW;
	}

	Mouse->renew();
	free(txt->content);
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
		oldc = getShort(grid().x, grid().y) >> 8;
		setCharColor(oldc ^ 0x88, grid().x, grid().y);
		first = 0;
	}
	if (grid(newPos.x, newPos.y) == grid())return;
	setCharColor(oldc, grid().x, grid().y);
	oldc = getShort(grid(newPos.x, newPos.y).x, grid(newPos.x, newPos.y).y) >> 8;
	setCharColor(oldc ^ 0x88, grid(newPos.x, newPos.y).x, grid(newPos.x, newPos.y).y);
	x = newPos.x;
	y = newPos.y;
}
void mouse::restore() {
	setCharColor(oldc, grid().x, grid().y);
}
void mouse::renew() {
	oldc = getShort(grid().x, grid().y) >> 8;
	setCharColor(oldc ^ 0x88, grid().x, grid().y);
}

file::file() {
	fileName = (char*)malloc(64);
	fileFolder = (char*)malloc(256);
	fileContent = (char*)malloc(64 * 1024);
	fileLength = 0;
	this->editorBegin = this->editorEnd = 0;
}
bool file::openFile(char *fileName) {
	std::ifstream fin(fileName, std::ios::binary|std::ios::in);
	int length, i;
	vecTwo tmp = { 0, 0 };
	if (fin) {
		Editor->clear();
		fin.seekg(0, std::ios::end);
		length = (int)fin.tellg();
		fileLength = length;
		fin.seekg(0, std::ios::beg);
		fin.read(fileContent, length);
		fin.close();
		strcpy(this->fileName, fileName);
		_getcwd(this->fileFolder, 256);
	}
	else return false;
	for (i = 0; i < length; i++) {
		Editor->operate(fileContent[i], false);
		if (Editor->cursorPos.y == Editor->height - 1)break;
	}
	editorEnd = Editor->putRow(i, Editor->height-1);
	editorBegin = 0;
	Editor->cursorPos.x = Editor->cursorPos.y = 0;
	return true;
}
void file::closeFile() {
	Editor->clear();
	fileLength = 0;
	this->editorBegin = this->editorEnd = 0;
}
char file::posContent(int pos) {
	return fileContent[pos];
}
