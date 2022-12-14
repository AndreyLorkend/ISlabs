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
#include <bitset>

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
	int seed;
	std::unordered_set< char> asciiTable;
	std::unordered_set< char> keySet;

	char asciiTableArr[16][16];
	char keyArr[16];

	gost_key_t gostKey;
	gost_iv_t gostIV;

	std::bitset<89> LRC1;
	std::bitset<91> LRC2;
	int kb;
	char buf[16];

	std::string filenameIn;
	std::string filenameOut;
private:
	int getIndex(std::unordered_set< char> S,  char K);
	int getAsciiTableIndex(char symbol);
	int getKeyIndex(char symbol);
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
	void initLRC();
	char generatorLRC();

	void setCursor(COORD position);
	void resetCursor();
public:
	HANDLE getHandleStdOut();
	MainWindow();
	void setExitFlag(bool flag);
	void printArray(COORD textCoord);
	void handleArrayData(COORD outCoord);
	// first lab
	void encodePolybius();
	void decodePolybius();
	// second lab
	void encodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out);
	void decodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out);
	//third lab
	void encodeGenerator();
	void decodeGenerator();

	gost_key_t getGostKey();
	gost_iv_t getGostIV();

	std::string getFilenameIn();
	std::string getFileNameOut();

	void Show();
};

