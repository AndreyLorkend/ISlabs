#pragma once
#include <windows.h>
#include <list>
#include "Window.h"
#include <string>

enum WinElementType {
	BUTTON_ELEMENT,
	TEXT_BOX_ELEMENT
};

class WinElement
{
private:
	SMALL_RECT elementBox;
	LPCSTR elementText;
	WinElementType elementType;
	int width_;
	int height_;
	int id_;
	static int elementCount;
	static int elementCurrentId;
	int padding_;
	std::string subType_;
public:
	WinElement(COORD pos, int padding, LPCSTR text, WinElementType type); // default
	WinElement(LPCSTR text, int padding, WinElementType type); // for adding elements with horizontal alignment
	WinElement(int height, int width, WinElementType type); // for adding elements width vertical alignment
	virtual ~WinElement();
	void virtual drawElement(HANDLE& hStdout) = 0;
	void virtual onClick(MOUSE_EVENT_RECORD& mer, Window* Sender) = 0;
	void transformHorizontalElement(COORD pos);
	void transformVerticalElement(COORD pos);
	void virtual writeOutText(HANDLE& hStdout) = 0;
	void virtual handleType(WinElementType type) = 0;
	void virtual addTextAttribute(std::list<CHAR*>& text) = 0;
	void setSubType(std::string subType);
	//getters
	int getWinElementId();
	int getPadding();
	int getElementWidth();
	int getElementHeight();
	std::string getSubType();
	LPCSTR getElementText();
	SMALL_RECT getElementBox();
	WinElementType getWinElementType();
};

