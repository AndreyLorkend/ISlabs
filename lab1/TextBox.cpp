#include "TextBox.h"

TextBox::~TextBox()
{

}

TextBox::TextBox(int height, int width, LPCSTR text, TextBoxTypes type) : WinElement(height, width, TEXT_BOX_ELEMENT)
{
	headerText = text;
	tbType = type;
	switch (type)
	{
	case LEGEND:
		setSubType("LEGEND");
		break;
	case INITIAL_DATA:
		setSubType("INITIAL_DATA");
		break;
	case HANDLED_DATA:
		setSubType("HANDLED_DATA");
		break;
	default:
		break;
	}
	
}

void TextBox::handleType(WinElementType type)
{
	if (type = getWinElementType()) {
		
	}
}

void TextBox::drawElement(HANDLE& hStdout)
{
	DWORD cWritten;
	SMALL_RECT textBox(getElementBox().Left, getElementBox().Top, getElementBox().Right, getElementBox().Bottom);
	COORD curCoord(textBox.Left, textBox.Top);
	WORD textColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	WORD bgColor = BACKGROUND_RED | BACKGROUND_INTENSITY;
	for (int i = textBox.Top; i < textBox.Bottom; i++) {
		FillConsoleOutputCharacterA(hStdout, ' ', textBox.Right, curCoord, &cWritten);
		FillConsoleOutputAttribute(hStdout, textColor | bgColor, textBox.Right, curCoord, &cWritten);
		curCoord.Y++;
	}
	COORD textCoord(textBox.Left + 1, textBox.Top + 1);
	WriteConsoleOutputCharacterA(hStdout, headerText, lstrlenA(headerText), textCoord, &cWritten);
}

void TextBox::onClick(MOUSE_EVENT_RECORD& mer, Window* Sender)
{
}

void TextBox::addTextAttribute(std::list<CHAR*>& text)
{
	for (auto line : text) {
		textLines.push_back(line);
	}
}

void TextBox::writeOutText(HANDLE& hStdout)
{
	DWORD cWritten = 0;
	drawElement(hStdout);
	outputPos = COORD(getElementBox().Left + SPACING_FROM_HEADER_LEFT, getElementBox().Top + SPACING_FROM_HEADER_TOP);
	for (auto text : textLines) {
		WriteConsoleOutputCharacter(hStdout, text, lstrlen(text), outputPos, &cWritten);
		outputPos.Y++;
	}
	textLines.clear();
}
