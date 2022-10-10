#pragma once
#include <windows.h>

class Window
{
public:
	virtual void printArray(COORD outCoord) = 0;
	virtual void fillArray() = 0;
	virtual HANDLE getHandleStdOut() = 0;
	virtual void setExitFlag(bool flag) = 0;
	virtual void handleArrayData(COORD outCoord) = 0;
	virtual void encode() = 0;
	virtual void decode() = 0;
	virtual ~Window();
};

