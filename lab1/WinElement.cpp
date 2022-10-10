#include "WinElement.h"

int WinElement::elementCount = 0;
int WinElement::elementCurrentId = 0;

WinElement::WinElement(COORD pos, int padding, LPCSTR text, WinElementType type)
{
	elementType = type;

	elementBox.Left = pos.X;
	elementBox.Right = pos.X + lstrlenA(text) + 2*padding;
	elementBox.Top = pos.Y;
	elementBox.Bottom = pos.Y + 3*padding;

	elementText = text;

	id_ = elementCurrentId;
	padding_ = padding;
	elementCurrentId++;
	elementCount++;
}

WinElement::WinElement(LPCSTR text, int padding, WinElementType type)
{
	elementText = text;
	elementType = type;
	id_ = elementCurrentId;
	padding_ = padding;
	elementCurrentId++;
	elementCount++;
}

WinElement::WinElement(int height, int width, WinElementType type)
{
	elementType = type;
	height_ = height;
	width_ = width;
}

WinElement::~WinElement()
{
	elementCount--;
}

void WinElement::transformHorizontalElement(COORD pos)
{
	elementBox.Left = pos.X;
	elementBox.Right = pos.X + width_;
	elementBox.Top = pos.Y;
	elementBox.Bottom = pos.Y + height_;

	height_ = elementBox.Bottom - elementBox.Top;
	width_ = elementBox.Right - elementBox.Left;
}

void WinElement::transformVerticalElement(COORD pos)
{
	elementBox.Left = pos.X;
	elementBox.Right = pos.X + lstrlenA(elementText) + 2 * padding_;
	elementBox.Top = pos.Y;
	elementBox.Bottom = pos.Y + 3 * padding_;

	height_ = elementBox.Bottom - elementBox.Top;
	width_ = elementBox.Right - elementBox.Left;
}

void WinElement::setSubType(std::string subType)
{
	subType_ = subType;
}

int WinElement::getWinElementId()
{
	return id_;
}

int WinElement::getPadding()
{
	return padding_;
}

int WinElement::getElementWidth()
{
	return width_;
}

int WinElement::getElementHeight()
{
	return height_;
}

std::string WinElement::getSubType()
{
	return subType_;
}

LPCSTR WinElement::getElementText()
{
	return elementText;
}

SMALL_RECT WinElement::getElementBox()
{
	return elementBox;
}

WinElementType WinElement::getWinElementType()
{
	return elementType;
}
