#include "ScrollbarWidget.h"
#include "../framework/ResourceManager.h"
#include "../framework/InputManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "Window.h"

namespace Arboria {
	ScrollbarWidget::ScrollbarWidget(Window* gui, Orientation orientation, Color _gripperColor) : Widget(gui), scrollValue(0), scrollStep(1), barOrientation(orientation), maximum(1), minimum(0), color({ (uint8_t)255, (uint8_t)255, (uint8_t)255, (uint8_t)128 }), gripperColor(_gripperColor) {
		
	}

	ScrollbarWidget::~ScrollbarWidget() = default;

	void ScrollbarWidget::setScrollValue(int value) {
		value = Math::iMax(value, minimum);
		value = Math::iMin(value, maximum);

		if (value == scrollValue)
			return;

		scrollValue = value;
		setDirty();
	}

	void ScrollbarWidget::scrollNext()
	{
		setScrollValue(scrollValue + scrollStep);
	}

	void ScrollbarWidget::scrollPrev()
	{
		setScrollValue(scrollValue - scrollStep);
	}

	void ScrollbarWidget::setMinimum(int newValue) {
		if (minimum == newValue)
			return;
		minimum = newValue;
		setScrollValue(scrollValue);
		setDirty();
	}

	void ScrollbarWidget::setMaximum(int newValue) {
		if (maximum == newValue)
			return;
		maximum = newValue;
		setScrollValue(scrollValue);
		setDirty();
	}

	bool ScrollbarWidget::onEvent(AEvent* e) {
		Widget::onEvent(e);

		if (e->eventType == EventType::EVENT_KEY) {
			if (e->eventValue == ArboriaKey_MouseLeft) {
				Vector2<int> realPos = getActualPosition();
				int cursorX = getGui()->getCursorX();
				int cursorY = getGui()->getCursorY();
				switch (barOrientation) {
				case Orientation::VERTICAL:
					if (realPos.x >= cursorX && cursorX <= realPos.x + size.x
						&& realPos.y + scrollValue >= cursorY 
						&& cursorY <= realPos.y + scrollValue + maximum) {
						setFlag(WIDGET_CAPTURED);
						setDirty();
					}
					else if (realPos.x >= cursorX && cursorX <= realPos.x + size.x
						&& cursorY > realPos.y + scrollValue + maximum) {
						scrollNext();
					}
					else if (realPos.x >= cursorX && cursorX <= realPos.x + size.x
						&& cursorY < realPos.y + scrollValue) {
						scrollPrev();
					}
					break;
				case Orientation::HORIZONTAL:
					if (realPos.x + scrollValue >= cursorX
						&& cursorX <= realPos.x + scrollValue + maximum
						&& realPos.y >= cursorY && cursorY <= realPos.y + size.y) {
						setFlag(WIDGET_CAPTURED);
						setDirty();
					}
					else if (realPos.y >= cursorY && cursorY <= realPos.y + size.y
						&& cursorX > realPos.x + scrollValue + maximum) {
						scrollNext();
					}
					else if (realPos.y >= cursorY && cursorY <= realPos.y + size.y
						&& cursorX < realPos.x + scrollValue) {
						scrollPrev();
					}
					break;
				default:
					break;
				}
			}
		}
		else if (e->eventType == EventType::EVENT_KEY && !e->eventValue2) {
			if (e->eventValue == ArboriaKey_MouseLeft) {
				if (flags & WIDGET_CAPTURED) {
					clearFlag(WIDGET_CAPTURED);
					setDirty();
				}
			}
		}
		else if (e->eventType == EventType::EVENT_MOUSE_MOVE && flags & WIDGET_CAPTURED) {
			if (barOrientation == Orientation::VERTICAL) {
				setScrollValue(static_cast<int>(e->eventValue2 / (size.y - maximum)) + minimum);
			}
			else
				setScrollValue(static_cast<int>(e->eventValue / (size.x - maximum)) + minimum);
		}

		return e->isHandled;
	}

	void ScrollbarWidget::onRender() {
		Widget::onRender();
		if (maximum == minimum)
			return;

		int position = static_cast<int>(segmentSize * minimum);
		Vector2<int> newPos, newSize;
		switch (barOrientation) {
			case Orientation::VERTICAL:
				newPos = { 0, position };
				newSize = { size.x, static_cast<int>(segmentSize * (maximum - minimum)) };
				break;
			case Orientation::HORIZONTAL:
			default:
				newPos = { position, 0 };
				newSize = { static_cast<int>(segmentSize * (maximum - minimum)), size.y };
				break;
		}
		renderer->drawRectangle(this->position, size, color);
		renderer->drawRectangle(newPos, newSize, gripperColor);
	}

	void ScrollbarWidget::run() {
	}

	bool ScrollbarWidget::parseInternalValue(const char* _name, Lexer* src)
	{
		return false;
	}
}