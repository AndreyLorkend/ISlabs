#pragma once
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include "Window.h"
#include "WinBox.h"
#include "Button.h"
#include "TextBox.h"
#include <time.h>
#include <map>
#include <string>
#include <set>
#include <unordered_set>

#define ARR_LENGTH 20
#define INPUT_RECORD_BUFFER 128
#define START_OF_ASCII_TABLE 0
#define END_OF_ASCII_TABLE 256

extern "C" {
	int findNegativeCount(int length, int* array);
	int findPositiveArithmeticMean(int length, int* array);
}

class MainWindow : public Window
{
private:
	std::map<std::string, WinBox*> winBoxes;
	int arr[ARR_LENGTH];
	HANDLE hStdin;
	HANDLE hStdout;
	DWORD fdwSaveOldMode;
	DWORD fdwMode;
	INPUT_RECORD irInBuf[INPUT_RECORD_BUFFER];
	bool isExitFlagSet;
	std::unordered_set<unsigned char> asciiTable;
	std::unordered_set<unsigned char> keySet;
private:
	int getIndex(std::unordered_set<unsigned char> S, unsigned char K);
	void fillArray();
	void fillAsciiTable();
	void generateKey();
	VOID MouseEventProc(MOUSE_EVENT_RECORD mer);
	VOID SetConsoleAppearance(WORD wAttributes);
	VOID DrawInterface();
	void handleConsole();
	bool SetConsoleParams();
	bool ErrorExit(LPSTR lpszMessage);
	void CreateElements();
public:
	HANDLE getHandleStdOut();
	MainWindow();
	void setExitFlag(bool flag);
	void printArray(COORD textCoord);
	void handleArrayData(COORD outCoord);
	void encode();
	void decode();
	void Show();
};

