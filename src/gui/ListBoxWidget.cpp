#include "ListBoxWidget.h"
#include "Window.h"
#include "../renderer/Renderer.h"
#include "../framework/InputManager.h"
#include "../framework/Lexer.h"

namespace Arboria
{

	ListBoxWidget::ListBoxWidget(Window* _gui, Orientation _orientation, uint32_t elemCount) : Widget(_gui), hovered(nullptr), selected(nullptr), displayCount(elemCount), cursor(0), itemSize(0), itemSpacing(0), viewOrientation(_orientation), viewPolicy(ScrollbarViewPolicy::AsNeeded), scrollbar(nullptr)
	{
	}

	ListBoxWidget::~ListBoxWidget() = default;

	bool ListBoxWidget::onEvent(AEvent* e)
	{
		Widget::onEvent(e);

		if (e->eventType >= EventType::EVENT_KEY && e->eventType <= EventType::EVENT_CONTROLLER_BUTTON && e->eventValue2 == 1) {
			int action = inputManager->getBinding(e->eventValue);
			int newCursor;
			switch (action) {
				case ACTION_UPARROW: //UP
					if (viewOrientation == Orientation::HORIZONTAL) //HORIZONTAL LIST
						break;
					newCursor = Math::iMax(cursor - 1, 0);
					if (cursor != newCursor && newCursor < indexOffset) {
						children[(displayCount - 1) + indexOffset]->setVisibility(false);
						scrollbar->scrollPrev();
						indexOffset = Math::iMax(indexOffset - 1, 0);
						children[indexOffset]->setVisibility(true);
					}
					if (cursor != newCursor) {
						gui->setCursor((int)(children[newCursor]->getActualPosition().x + (children[newCursor]->size.x) / 2),
							(int)(children[newCursor]->getActualPosition().y + (children[newCursor]->size.y) / 2));
					}
					cursor = newCursor;
					hovered = children[cursor];
					break;
				case ACTION_DOWNARROW: //DOWN
					if (viewOrientation == Orientation::HORIZONTAL) //HORIZONTAL LIST
						break;
					newCursor = Math::iMin(cursor + 1, itemCount - 1);
					if (cursor != newCursor && newCursor > (displayCount - 1) + indexOffset) {
						children[indexOffset]->setVisibility(false);
						scrollbar->scrollNext();
						indexOffset = Math::iMin(indexOffset + 1, itemCount - displayCount);
						children[(displayCount - 1) + indexOffset]->setVisibility(true);
					}
					if (cursor != newCursor) {
						gui->setCursor((int)(children[newCursor]->getActualPosition().x + (children[newCursor]->size.x) / 2),
							(int)(children[newCursor]->getActualPosition().y + (children[newCursor]->size.y) / 2));
					}
					cursor = newCursor;
					hovered = children[cursor];
					break;
				case ACTION_LEFTARROW: //LEFT
					if (viewOrientation == Orientation::VERTICAL)
						break;
					newCursor = Math::iMax(cursor - 1, 0);
					if (cursor != newCursor && cursor <= indexOffset) {
						children[(displayCount - 1) + indexOffset]->setVisibility(false);
						scrollbar->scrollPrev();
						indexOffset = Math::iMax(indexOffset - 1, 0);
						children[indexOffset]->setVisibility(true);
					}
					if (cursor != newCursor) {
						gui->setCursor((int)(children[newCursor]->getActualPosition().x + (children[newCursor]->size.x) / 2),
							(int)(children[newCursor]->getActualPosition().y + (children[newCursor]->size.y) / 2));
					}
					cursor = newCursor;
					hovered = children[cursor];
					break;
				case ACTION_RIGHTARROW: //RIGHT
					if (viewOrientation == Orientation::HORIZONTAL)
						break;
					newCursor = Math::iMin(cursor + 1, itemCount - 1);
					if (cursor != newCursor && cursor > (displayCount - 1) + indexOffset) {
						children[indexOffset]->setVisibility(false);
						scrollbar->scrollNext();
						indexOffset = Math::iMin(indexOffset + 1, itemCount - displayCount);
						children[(displayCount - 1) + indexOffset]->setVisibility(true);
					}
					if (cursor != newCursor) {
						gui->setCursor((int)(children[newCursor]->getActualPosition().x + (children[newCursor]->size.x) / 2),
							(int)(children[newCursor]->getActualPosition().y + (children[newCursor]->size.y) / 2));
					}
					cursor = newCursor;
					hovered = children[cursor];
					break;
				case ACTION_CONFIRM: //CONFIRM
					selected = hovered != nullptr ? hovered : children[cursor];
					break;
				case ACTION_CANCEL: //CANCEL
					selected = nullptr;
					break;
				default:
					break;
			}

		}
		return e->isHandled;
	}

	void ListBoxWidget::onRender() {
		int start = 1;
		if (scrollbar == NULL) {
			configureScrollbar(viewOrientation);
		}
		
		int controlOffset = 0;
		int currRow = 0;
		int currCol = 0;
		int itemIdx = 0;
		for (auto c = children.begin(); c != children.end(); c++) { //first element is ALWAYS the scrollbar
			auto child = *c;
			if (child != scrollbar && child->isVisible()) {
				if (itemSize != 0) {
					switch (viewOrientation) {
					case Orientation::VERTICAL:
						child->size.x = size.x;
						break;
					case Orientation::HORIZONTAL:
						child->size.y = size.y;
						break;
					}
				}

				switch (viewOrientation) {
				case Orientation::VERTICAL:
					child->position.y = controlOffset - scrollOffset;
					controlOffset += child->size.y + itemSpacing;
					break;
				case Orientation::HORIZONTAL:
					child->position.x = controlOffset - scrollOffset;
					controlOffset += child->size.x + itemSpacing;
					break;
				default:
					break;
				}
			}

		}
		switch (viewOrientation) {
		case Orientation::VERTICAL:
			scrollbar->setMaximum(Math::iMax(controlOffset - size.y, scrollbar->getMinimum()));
			break;
		case Orientation::HORIZONTAL:
			scrollbar->setMaximum(Math::iMax(controlOffset - size.x, scrollbar->getMinimum()));
			break;
		}
	}

	void ListBoxWidget::postRender() {
		Widget::postRender();

		for (int i = 0; i < children.getLength(); i++) {
			auto child = children[i];
			if (child != scrollbar && child->isVisible()) {
				if (child == selected) {
					renderer->drawBorders(Vector2<float>{child->position.x, child->position.y}, Vector2<float>{child->size.x, child->size.y}, selectColor);	
				}
				if (child == hovered) {
					renderer->drawBorders(Vector2<float>{child->position.x, child->position.y}, Vector2<float>{child->size.x, child->size.y}, hoverColor);
				}
			}
		}
	}

	void ListBoxWidget::run() {
		Widget::run();

		if (scrollbar == NULL) {
			configureScrollbar(viewOrientation);
		}
		if (scrollbar) {
			int scrollbarLength = itemCount == 0 ? 0 : itemSpacing * itemCount;

			switch (viewOrientation) {
			case Orientation::VERTICAL:
				if (itemSize == 0) {
					for (const auto& c : children) {
						scrollbarLength += c->size.y;
					}
				}
				else {
					scrollbarLength += itemCount * itemSize;
				}
				scrollbarLength = scrollbarLength > size.y ? scrollbarLength - size.y : 0;
				break;
			case Orientation::HORIZONTAL:
				if (itemSize == 0) {
					for (const auto& c : children) {
						scrollbarLength += c->size.x;
					}
				}
				else {
					scrollbarLength += itemCount * itemSize;
				}
				scrollbarLength = scrollbarLength > size.x ? scrollbarLength - size.x : 0;
				break;
			}
			scrollbar->setMaximum(scrollbarLength);
			scrollbar->run();
			
			if (scrollbar->getScrollValue() != scrollOffset) {
				scrollOffset = scrollbar->getScrollValue();
				setDirty();
			}
		}
	}

	void ListBoxWidget::clear() {
		for (auto& c : children) {
			c->setParent(nullptr);
		}
		children.clear();
		selected = nullptr;
		hovered = nullptr;
		setDirty();
	}

	void ListBoxWidget::addItem(Widget* item)
	{
		if (scrollbar)
			item->setParent(this, children.findIndex(scrollbar));
		else
			item->setParent(this);
		
		itemCount++;
		setDirty();
	}

	void ListBoxWidget::setSelected(Widget* widget)
	{
		bool found = false;
		for (int i = 0; i < children.getLength(); i++) {
			if (children[i] == widget) {
				found = true;
				break;
			}
		}
		if (found) {
			Engine::printError("ListBoxWidget::setSelected: Cannot set an item not in the listbox to be the selected item");
		}
		selected = widget;
		setDirty();
	}

	bool ListBoxWidget::parseInternalValue(const char* _name, Lexer* src)
	{
		if (String::iCompare(_name, "itemSize") == 0) {
			itemSize = src->parseInt();
			return true;
		}
		if (String::iCompare(_name, "itemCount") == 0) {
			itemCount = src->parseInt();
			return true;
		}
		if (String::iCompare(_name, "itemSpacing") == 0) {
			itemSpacing = src->parseInt();
			return true;
		}
		if (String::iCompare(_name, "displayCount") == 0) {
			displayCount = src->parseInt();
			return true;
		}
		if (String::iCompare(_name, "viewOrientation") == 0) {
			viewOrientation = (Orientation)src->parseInt();
			if (scrollbar != NULL) {
				scrollbar->barOrientation = viewOrientation;
			}
			return true;
		}
		if (String::iCompare(_name, "hoverColor") == 0) {
			parseColor(src, hoverColor);
			return true;
		}
		if (String::iCompare(_name, "selectColor") == 0) {
			parseColor(src, selectColor);
			return true;
		}
		if (String::iCompare(_name, "regularColor") == 0) {
			parseColor(src, regularColor);
			return true;
		}
		if (String::iCompare(_name, "viewPolicy") == 0) {
			viewPolicy = (ScrollbarViewPolicy)src->parseInt();
			return true;
		}
		return Widget::parseInternalValue(_name, src);
	}

	void ListBoxWidget::updateScrollbarInfo()
	{
		if (!scrollbar)
			configureScrollbar(viewOrientation);

		if (itemCount > 0) {
			if (viewOrientation == Orientation::VERTICAL) {
				scrollbar->scrollStep = static_cast<int>(scrollbar->size.y / itemCount);
				scrollbar->setSegmentSize((displayCount / itemCount) * scrollbar->size.y);
			}
			else {
				scrollbar->scrollStep = static_cast<int>(scrollbar->size.x / itemCount);
				scrollbar->setSegmentSize((displayCount / itemCount) * scrollbar->size.x);
			}
		}
	}

	void ListBoxWidget::configureScrollbar(Orientation orientation) {
		if (orientation == Orientation::VERTICAL) {
			scrollbar = createChild<ScrollbarWidget>(orientation);
			scrollbar->size.x = 16;
			scrollbar->position.x = size.x - scrollbar->size.x;
			scrollbar->position.y = 0;
			scrollbar->size.y = size.y;
		}
		else {
			scrollbar = createChild<ScrollbarWidget>(orientation);
			scrollbar->size.y = 16;
			scrollbar->position.y = size.y - scrollbar->size.y;
			scrollbar->position.x = 0;
			scrollbar->size.x = size.x;
		}
		
		updateScrollbarInfo();
	}
}