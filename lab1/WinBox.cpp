#include "WinBox.h"

int WinBox::boxCount = 0;
int WinBox::boxCurrentId = 0;
COORD WinBox::currentBoxPosition = COORD(1, 1);

WinBox::WinBox(COORD pos)
{
	id_ = boxCurrentId;
	boxPos_ = currentBoxPosition;
	currentElementPos_ = currentBoxPosition;
	boxCount++;
	boxCurrentId++;
}

WinBox::~WinBox()
{
	boxCount--;
}

void WinBox::setBoxPos(COORD pos)
{
	boxPos_ = pos;
}

COORD WinBox::getBoxPos()
{
	return boxPos_;
}

void WinBox::addElementWidthHorizontalAlignment(WinElement* element)
{
	element->transformVerticalElement(currentElementPos_);
	elementsList.push_back(element);
	currentElementPos_ = COORD(element->getElementBox().Right + HRZ_SPACING, element->getElementBox().Top);
	width_ = element->getElementBox().Right;
}

void WinBox::addElementWidthVerticalAlignment(WinElement* element)
{
	element->transformHorizontalElement(currentElementPos_);
	elementsList.push_back(element);
	currentElementPos_ = COORD(element->getElementBox().Left, element->getElementBox().Bottom + VRT_SPACING);
	width_ = element->getElementBox().Bottom;
	currentBoxPosition = COORD(element->getElementBox().Left, element->getElementBox().Bottom + VRT_SPACING);
}

void WinBox::drawElements(HANDLE& hStdout)
{
	for (auto el : elementsList) {
		el->drawElement(hStdout);
	}
}

void WinBox::checkClick(MOUSE_EVENT_RECORD& mer, Window* Sender)
{
	for (auto el : elementsList) {
		el->onClick(mer, Sender);
	}
}

void WinBox::handleWinElement(std::string subType, WinElementType type, std::list<CHAR*> textLines, HANDLE& hStdout)
{
	for (auto el : elementsList) {
		if (el->getSubType() == subType && el->getWinElementType()) {
			el->addTextAttribute(textLines);
			el->writeOutText(hStdout);
		}
	}
}

int WinBox::getBoxId()
{
	return id_;
}
