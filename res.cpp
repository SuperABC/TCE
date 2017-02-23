#include "tce.h"

extern frame Frame;

enum editor::LANGUAGE {
		CL_C,
		CL_CPP,
		CL_CS,
		CL_JAVA,
		CL_ASM,
	};
char *editor::keyWord[5][100] = { {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union", "unsigned", "void",
														"for", "do", "while", "break", "continue", "if", "else", "goto", "switch", "case", "default", "return",
														"auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile"},
													   {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union", "unsigned", "void",
														"for", "do", "while", "break", "continue", "if", "else", "goto", "switch", "case", "default", "return",
														"auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile",
														"bool", "true", "false", "class", "public", "private", "protected", "friend", "mutable", "explicit", "this",
														"virtual", "template", "typename", "inline", "new", "delete", "try", "catch", "throw", "nullptr", "operator"} };

const char *frame::newFileAlert = "Funtion uncoded!\n"
"  The new file function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::saveCurrentAlert = "Function uncoded!\n"
"  The saving function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::saveAsAlert = "Function uncoded!\n"
"  The saving function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::closeFileAlert = "\nAre you sure to close this file?";
const char *frame::closeTCEAlert = "\nAre you sure to exit TCE?";
const char *frame::undoAlert = "History record uncoded!\n"
"  The history record is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::redoAlert = "History record uncoded!\n"
"  The history record is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::cutAlert = "Clipboard uncoded!\n"
"  The clipboard is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::copyAlert = "Clipboard uncoded!\n"
"  The clipboard is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::pasteAlert = "Clipboard uncoded!\n"
"  The clipboard is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::deleteAlert = "Clipboard uncoded!\n"
"  The clipboard is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::gotoAlert = "Function uncoded!\n"
"  The goto function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::clipboardAlert = "Clipboard uncoded!\n"
"  The clipboard is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::settingAlert = "Setting board uncoded!\n"
"  The setting board is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::searchAlert = "search&replace function uncoded!\n"
"  The search&replace function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::replaceAlert = "search&replace function uncoded!\n"
"  The search&replace function is now writing and has not been finished yet. Please wait patiently, thank you!";
const char *frame::compileAlert = "compiler not found!\n"
"  There isn't any compiler in this TCE.";
const char *frame::runAlert = "compiler not found!\n"
"  There isn't any compiler in this TCE.";
const char *frame::debugAlert = "compiler not found!\n"
"  There isn't any compiler in this TCE.";

void functions::closeTCE() {
	if (Frame.alertReturn == NO_BUTTON || Frame.alertReturn == CANCEL_BUTTON)return;
	if (Frame.alertReturn == OK_BUTTON)exit(0);
}
