#include "ListBoxWidget.h"
#include "../framework/ActionManager.h"
#include "../renderer/Renderer.h"

namespace Arboria {

	ListBoxWidget::ListBoxWidget() : ScrollAreaWidget(1, 1), cursor(0) {}

	ListBoxWidget::ListBoxWidget(ScrollbarWidget* widget) : ScrollAreaWidget(widget), cursor(0) {}

	ListBoxWidget::ListBoxWidget(int rowNum, int colNum) : ScrollAreaWidget(colNum, rowNum), cursor(0) {}

	ListBoxWidget::~ListBoxWidget() = default;

	/*void ListBoxWidget::eventOccured(SystemEvent* e) {

		Widget::eventOccured(e);

		if (e->type == EventType::EVENT_UI_INTERACTION) {
			auto widgetEventData = static_cast<GuiEventData*>(e->data);
			Widget* origin = widgetEventData->raisedBy;
			Widget* descendant = origin != NULL ? origin->getPointOfAncestry(this) : NULL;
			if (widgetEventData->guiEventType == WidgetEventType::KEY_DOWN || widgetEventData->guiEventType == WidgetEventType::KEY_UP) {
				KeyboardEventData keyData = widgetEventData->keyboardData;
				InputActionType actionType;
				if (scrollbar && (descendant != NULL || origin == this)) {
					actionType = inputManager->getActionTranslation(keyData);
					if (actionType != InputActionType::INVALID && widgetEventData->guiEventType == WidgetEventType::KEY_DOWN) {
						int newCursor;
						switch (viewOrientation) {
							case Orientation::HORIZONTAL:
								if (actionType == InputActionType::RIGHT) {
									newCursor = Math::iMin(cursor + 1, children.getLength() - 1);
									if (newCursor != cursor && newCursor >= indexOffset.x + elementDisplayCount - 1) {
										indexOffset.x++;
									}
									cursor = newCursor;
								}
								else if (actionType == InputActionType::LEFT) {
									newCursor = Math::iMax(cursor - 1, 0);
									if (newCursor != cursor && newCursor < indexOffset.y) {
										indexOffset.x = Math::iMax(indexOffset.x - 1, 0);
									}
									cursor = newCursor;
								}
								if (hovered != &children[cursor]) {
									hovered = &children[cursor];
									submitGuiEvent(WidgetEventType::LIST_BOX_CHANGE_HOVER, e);
								}
								break;
							case Orientation::VERTICAL:
							default:
								if (actionType == InputActionType::DOWN) {
									newCursor = Math::iMin(cursor + 1, children.getLength() - 1);
									if (newCursor != cursor && newCursor >= indexOffset.y + elementDisplayCount - 1) {
										indexOffset.y++;
									}
									cursor = newCursor;
								}
								else if (actionType == InputActionType::UP) {
									newCursor = Math::iMax(cursor - 1, 0);
									if (newCursor != cursor && newCursor < indexOffset.y) {
										indexOffset.y = Math::iMax(indexOffset.y - 1, 0);
									}
									cursor = newCursor;
								}
								if (hovered != &children[cursor]) {
									hovered = &children[cursor];
									submitGuiEvent(WidgetEventType::LIST_BOX_CHANGE_HOVER, e);
								}
								break;
						}
					}
					else if (actionType != InputActionType::INVALID && widgetEventData->guiEventType == WidgetEventType::KEY_UP) {
						switch (actionType) {
							case InputActionType::CONFIRM:
								selected = &children[cursor];
								submitGuiEvent(WidgetEventType::LIST_BOX_CHANGE_SELECT, e);
								break;
							case InputActionType::CANCEL:
								submitGuiEvent(WidgetEventType::LIST_BOX_CHANGE_CANCEL, e);
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}*/

	bool ListBoxWidget::onEvent(AEvent* e)
	{
		if (children.getLength() > 0)
			children[cursor]->onEvent(e);

		if (!e->isHandled && flags & WidgetStateFlags::WIDGET_FOCUSED) {
			int action = actionManager->getAction(*e);
			int newCursor;
			switch (action) {
				case 6: //UP
					if (elementDisplayCount == 1) //HORIZONTAL LIST
						break;
					newCursor = Math::iMax(cursor - columns, 0);
					if (cursor != newCursor && newCursor < indexOffset.y) {
						indexOffset.y = Math::iMax(indexOffset.y - 1, 0);
					}
					cursor = newCursor;
					e->isHandled = true;
					break;
				case 7: //DOWN
					if (elementDisplayCount == 1) //HORIZONTAL LIST
						break;
					newCursor = Math::iMin(cursor + columns, itemCount - 1);
					if (cursor != newCursor && cursor <= (indexOffset.y + elementDisplayCount - 1)) {
						indexOffset.y ++;
					}
					cursor = newCursor;
					e->isHandled = true;
					break;
				case 8: //LEFT
					newCursor = Math::iMax(cursor - 1, 0);
					if (elementDisplayCount == 1) {
						if (cursor != newCursor && cursor <= indexOffset.x) {
							indexOffset.x = Math::iMax(indexOffset.x - 1, 0);
						}
					}
					else {
						int newRowNum = Math::floor(newCursor / itemCount);
						int oldRowNum = Math::floor(cursor / itemCount);
						if (newCursor != cursor && newRowNum != oldRowNum && newRowNum < indexOffset.y) {
							indexOffset.y = Math::iMax(indexOffset.y - 1, 0);
						}
					}
					cursor = newCursor;
					e->isHandled = true;
					break;
				case 9: //RIGHT
					newCursor = Math::iMin(cursor + 1, itemCount - 1);
					if (elementDisplayCount == 1) {
						if (cursor != newCursor && cursor >= indexOffset.x + elementDisplayCount - 1) {
							indexOffset.x++;
						}
					}
					else {
						int newRowNum = Math::floor(newCursor / itemCount);
						int oldRowNum = Math::floor(cursor / itemCount);
						if (newCursor != cursor && oldRowNum != newRowNum && newRowNum >= indexOffset.y + elementDisplayCount - 1) {
							indexOffset.y++;
						}
					}
					cursor = newCursor;
					e->isHandled = true;
					break;
				default:
					if (action > 0 && callbacks[action - 1] != NULL) {
						callbacks[action - 1](this);
						e->isHandled = true;
					}
					break;
			}
		}
		return e->isHandled;
	}

	void ListBoxWidget::onRender() {
		if (scrollbar == NULL) configureScrollbar(Orientation::VERTICAL);
		if (horizontalScrollbar == NULL) configureScrollbar(Orientation::HORIZONTAL);
		
		Vector2<int> controlOffset = { 0, 0 };
		for (int i = 0; i < itemCount; i++) {
			auto child = children[i];
			int currRow = Math::ceil(i / columns);
			int currCol = Math::ceil(i % columns);
			int indOff = elementDisplayCount > 1 ? indexOffset.y : indexOffset.x;
			if (child != scrollbar) {
				if (i >= indOff) {
					if (elementDisplayCount == 1 && i <= currCol + indOff + columns) {
						child->rect.h = rect.h;
						child->rect.w = itemSize;
						child->rect.x = rect.x + controlOffset.x;
						controlOffset.x += itemSize + itemSpacing;
					}
					else if (elementDisplayCount > 1 && i <= currRow + (indOff + elementDisplayCount - 1)) {
						child->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
						child->rect.w = Math::ceil(rect.w/columns);
						child->rect.h = itemSize;
						child->rect.x = rect.x + controlOffset.x;
						child->rect.y = rect.y + controlOffset.y;
						controlOffset.x = currCol * (child->rect.w);
						controlOffset.y = currRow * (itemSize + itemSpacing);
					}
				}
				else {
					child->clearFlag(WidgetStateFlags::WIDGET_VISIBLE);
				}
			}
		}
		scrollbar->setScrollValue(cursor);
	}

	void ListBoxWidget::postRender() {
		Widget::postRender();

		for (int i = 0; i < children.getLength(); i++) {
			auto child = children[i];
			if (child != scrollbar && child->isVisible()) {
				if (child == selected) {
					renderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { selectColor.r, selectColor.g, selectColor.b, selectColor.a });
					//spriteRenderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { child->rect.w, child->rect.h }, { selectColor.r, selectColor.g, selectColor.b, selectColor.a});
				}
				else if (child == hovered) {
					renderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a });
					//spriteRenderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { child->rect.w, child->rect.h }, { hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a });
				}
				else {
					renderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { regularColor.r, regularColor.g, regularColor.b, regularColor.a });
					//spriteRenderer->draw(child->getSurface(), { child->rect.x, child->rect.y }, { child->rect.w, child->rect.h }, { regularColor.r, regularColor.g, regularColor.b, regularColor.a });
				}
			}
		}
	}

	void ListBoxWidget::run() {
		Widget::run();

		if (scrollbar == NULL) {
			configureScrollbar(Orientation::VERTICAL);
			configureScrollbar(Orientation::HORIZONTAL);
		}
		if (scrollbar) {
			int oldMin = scrollbar->getMinimum();
			int oldMax = scrollbar->getMaximum();
			int off = viewOrientation == Orientation::VERTICAL ? indexOffset.y : indexOffset.x;
			scrollbar->setMinimum(Math::iMin(indexOffset.y, cursor));
			scrollbar->setMaximum(Math::iMax(indexOffset.y + elementDisplayCount - 1, Math::ceil(cursor/columns)));
			scrollbar->run();
			int oldHorMin = horizontalScrollbar->getMinimum();
			int oldHorMax = horizontalScrollbar->getMaximum();
			horizontalScrollbar->setMinimum(Math::iMin(indexOffset.x, columns));
			horizontalScrollbar->setMaximum(Math::iMax(indexOffset.x + columns - 1, Math::ceil(cursor % columns)));
			if (scrollbar->getMinimum() != oldMin || scrollbar->getMaximum() != oldMax || horizontalScrollbar->getMinimum() != oldHorMin || horizontalScrollbar->getMaximum() != oldHorMax) {
				this->setDirty();
			}
		}
	}

	void ListBoxWidget::addItem(Widget* item)
	{
		item->setParent(this);
		itemCount++;
		resolveLocation();
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
}