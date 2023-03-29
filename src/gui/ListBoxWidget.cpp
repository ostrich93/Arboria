#include "ListBoxWidget.h"
#include "../events/Event.h"
#include "../globals.h"

namespace Arboria {

	ListBoxWidget::ListBoxWidget() : ListBoxWidget(NULL) {}

	ListBoxWidget::ListBoxWidget(ScrollbarWidget* widget) : ScrollAreaWidget(widget) {}

	ListBoxWidget::~ListBoxWidget() = default;

	void ListBoxWidget::eventOccured(Event* e) {

		Widget::eventOccured(e);

		if (e->getEventType() == EventType::EVENT_UI_INTERACTION) {
			auto widgetEvent = dynamic_cast<WidgetEvent*>(e);
			Widget* origin = widgetEvent->getData().raisedBy;
			Widget* descendant = origin != NULL ? origin->getPointOfAncestry(this) : NULL;
			if (widgetEvent->getData().guiEventType == WidgetEventType::KEY_DOWN || widgetEvent->getData().guiEventType == WidgetEventType::KEY_UP) {
				KeyboardEventData* keyData = &widgetEvent->getData().keyboardData;
				InputActionType actionType;
				if (scrollbar && (descendant != NULL || origin == this)) {
					actionType = engine->getInputManager()->getActionTranslation(keyData);
					if (actionType != InputActionType::INVALID && widgetEvent->getData().guiEventType == WidgetEventType::KEY_DOWN) {
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
					else if (actionType != InputActionType::INVALID && widgetEvent->getData().guiEventType == WidgetEventType::KEY_UP) {
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
	}

	void ListBoxWidget::onRender() {
		if (scrollbar == NULL) configureScrollbar(viewOrientation);
		
		Vector2<int> controlOffset = { 0, 0 };
		for (int i = 0; i < children.getLength(); i++) {
			auto child = &children[i];
			int indOff = viewOrientation == Orientation::VERTICAL ? indexOffset.y : indexOffset.x;
			if (child != scrollbar) {
				children[i].setVisible(i >= indOff && i <= indOff + indOff + (elementDisplayCount - 1));
				if (!children[i].isVisible()) {
					continue;
				}
				switch (viewOrientation) {
					case(Orientation::HORIZONTAL):
						children[i].size.y = size.y;
						children[i].size.x = itemSize;
						children[i].location.x = location.x + controlOffset.x;
						controlOffset.x += itemSize + itemSpacing;
						break;
					case(Orientation::VERTICAL):
					default:
						children[i].size.x = size.x;
						children[i].size.y = itemSize;
						children[i].location.y = location.y + controlOffset.y;
						controlOffset.y += itemSize + itemSpacing;
						break;
				}
			}
		}
		scrollbar->setScrollValue(cursor);
	}

	void ListBoxWidget::postRender() {
		Widget::postRender();

		for (int i = 0; i < children.getLength(); i++) {
			auto child = &children[i];
			if (child != scrollbar && child->isVisible()) {
				if (child == selected) {
					engine->getSpriteRenderer()->draw(child->getSurface(), child->location, child->size, selectColor);
				}
				else if (child == hovered) {
					engine->getSpriteRenderer()->draw(child->getSurface(), child->location, child->size, hoverColor);
				}
				else {
					engine->getSpriteRenderer()->draw(child->getSurface(), child->location, child->size, regularColor);
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
			int oldMin = scrollbar->getMinimum();
			int oldMax = scrollbar->getMaximum();
			int off = viewOrientation == Orientation::VERTICAL ? indexOffset.y : indexOffset.x;
			scrollbar->setMinimum(Math::iMin(off, cursor));
			scrollbar->setMaximum(Math::iMax(off + elementDisplayCount - 1, cursor));
			scrollbar->run();
			if (scrollbar->getMinimum() != oldMin || scrollbar->getMaximum() != oldMax) {
				this->setDirty();
			}
		}
	}

	void ListBoxWidget::addItem(Widget* item)
	{
		item->setParent(this);
		resolveLocation();
		setDirty();
	}

	void ListBoxWidget::setSelected(Widget* widget)
	{
		bool found = false;
		for (int i = 0; i < children.getLength(); i++) {
			if (&children[i] == widget) {
				found = true;
				break;
			}
		}
		if (found) {
			printf("Cannot set an item not in the listbox to be the selected item");
		}
		selected = widget;
		setDirty();
	}
}