#pragma once
#include "WinElement.h"
#include <list>
#define SPACING_FROM_HEADER_TOP 2
#define SPACING_FROM_HEADER_LEFT 1

enum TextBoxTypes {
	LEGEND,
	INITIAL_DATA,
	HANDLED_DATA,
	OUTPUT
};

class TextBox : public WinElement
{
private:
	LPCSTR headerText;
	int height_;
	int width_;
	COORD outputPos;
	TextBoxTypes tbType;
	std::list<CHAR*> textLines;
public:
	~TextBox();
	TextBox(int height, int width, LPCSTR text, TextBoxTypes type);
	void handleType(WinElementType type);
	void drawElement(HANDLE& hStdout);
	void onClick(MOUSE_EVENT_RECORD& mer, Window* Sender);
	void writeOutText(HANDLE& hStdout);
	void addTextAttribute(std::list<CHAR*>& text);
};

