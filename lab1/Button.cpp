#include "Button.h"

bool Button::checkTouch(MOUSE_EVENT_RECORD& mer)
{
	SHORT mX = mer.dwMousePosition.X;
	SHORT mY = mer.dwMousePosition.Y;
	bool isTouched = false;
	if ( (mX >= getElementBox().Left && mX <= getElementBox().Right-1) &&
		(mY >= getElementBox().Top && mY <= getElementBox().Bottom-1) )
	{
		isTouched = true;
	}
	return isTouched;
}

void Button::onClick(MOUSE_EVENT_RECORD& mer, Window* sender)
{
	if (checkTouch(mer)) {
		switch (event_) {
		case SHOW_DATA:
			sender->printArray(COORD(2, 5));
			break;
		case CLOSE:
			sender->setExitFlag(true);
			break;
		case FILL_ARRAY:
			sender->fillArray();
			break;
		case HANDLE_DATA:
			sender->handleArrayData(COORD(2, 10));
			break;
		case ENCODE_POLYBIUS:
			sender->encodePolybius();
			break;
		case DECODE_POLYBIUS:
			sender->decodePolybius();
			break;
		case ENCODE_GOST:
		{
			sender->encodeGOST(sender->getGostKey(), sender->getGostIV(), sender->getFilenameIn(), sender->getFileNameOut());
			break;
		}
		case DECODE_GOST:
			sender->decodeGOST(sender->getGostKey(), sender->getGostIV(), sender->getFilenameIn(), sender->getFileNameOut());
			break;
		default:
			break;
		}
	}
}

void Button::writeOutText(HANDLE& hStdout)
{
}

void Button::addTextAttribute(std::list<CHAR*>& text)
{
}

void Button::handleType(WinElementType type)
{
}

void Button::drawElement(HANDLE& hStdout)
{
	DWORD cWritten;
	SMALL_RECT btnBox(getElementBox().Left, getElementBox().Top, getElementBox().Right, getElementBox().Bottom);
	COORD curCoord(btnBox.Left, btnBox.Top);
	WORD textColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	WORD bgColor = BACKGROUND_RED | BACKGROUND_INTENSITY;
	for (int i = btnBox.Top; i < btnBox.Bottom; i++) {
		FillConsoleOutputCharacterA(hStdout, ' ', btnBox.Right-btnBox.Left, curCoord, &cWritten);
		FillConsoleOutputAttribute(hStdout, textColor | bgColor, btnBox.Right-btnBox.Left, curCoord, &cWritten);
		curCoord.Y++;
	}
	COORD textCoord(btnBox.Left + getPadding(), (btnBox.Top + btnBox.Bottom) / 2);
	WriteConsoleOutputCharacterA(hStdout, getElementText(), lstrlenA(getElementText()), textCoord, &cWritten);
}

Button::Button(COORD btnPos, int padding, LPCSTR btnText, BtnOnClickEvent ev) : 
	WinElement(btnPos, padding, btnText, BUTTON_ELEMENT)
{
	event_ = ev;
}

Button::Button(int padding, LPCSTR btnText, BtnOnClickEvent ev) : WinElement(btnText, padding, BUTTON_ELEMENT)
{
	event_ = ev;
}
