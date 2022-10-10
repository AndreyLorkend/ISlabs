#pragma once
#include <windows.h>
#include "WinElement.h"
#include <list>
#include "Window.h"

#define VRT_SPACING 1
#define HRZ_SPACING 1

class WinBox
{
private:
	COORD boxPos_;
	COORD currentElementPos_;
	int height_;
	int width_;
	int id_;
	static int boxCount;
	static int boxCurrentId;
	static COORD currentBoxPosition;
	std::list<WinElement*> elementsList;
public:
	WinBox(COORD pos);
	~WinBox();
	void setBoxPos(COORD pos);
	COORD getBoxPos();
	void addElementWidthHorizontalAlignment(WinElement* element);
	void addElementWidthVerticalAlignment(WinElement* element);
	void drawElements(HANDLE& hStdout);
	void checkClick(MOUSE_EVENT_RECORD& mer, Window* Sender);
	//WinElement& getWinElement(std::string subType, WinElementType type);
	void handleWinElement(std::string subType, WinElementType type, std::list<CHAR*> textLines, HANDLE& hStdout);
	int getBoxId();
};

