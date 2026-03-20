#include "ListBoxWidget.h"
#include "../framework/ActionManager.h"
#include "../renderer/Renderer.h"

namespace Arboria {

	ListBoxWidget::ListBoxWidget() : ScrollAreaWidget(1, 1), cursor(0), itemSize(0), itemSpacing(0), hovered(nullptr), selected(nullptr), hoveredImage(nullptr), selectedImage(nullptr) {}

	ListBoxWidget::ListBoxWidget(ScrollbarWidget* widget) : ScrollAreaWidget(widget), cursor(0), itemSize(0), itemSpacing(0), hovered(nullptr), selected(nullptr), hoveredImage(nullptr), selectedImage(nullptr) {}

	ListBoxWidget::ListBoxWidget(int rowNum, int colNum) : ScrollAreaWidget(colNum, rowNum), cursor(0), itemSize(0), itemSpacing(0), hovered(nullptr), selected(nullptr), hoveredImage(nullptr), selectedImage(nullptr) {}

	ListBoxWidget::~ListBoxWidget() = default;

	bool ListBoxWidget::onEvent(AEvent* e)
	{
		Widget::onEvent(e);

		if (e->eventType == EventType::EVENT_UI_INTERACTION) {
			Widget* ctrl = e->guiEvent.raisedBy; //usually a button
			Widget* interactionPoint = ctrl->getPointOfAncestry(this); //the entry in the list containing the button
			if (e->guiEvent.eventType == GUIEventType::KEY_DOWN) {
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
						if (ctrl == this || ctrl == scrollbar) {
							interactionPoint = nullptr;
						}
						if (hovered != interactionPoint) {
							hovered = interactionPoint;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_HOVER, e);
						}
						break;
					case 7: //DOWN
						if (elementDisplayCount == 1) //HORIZONTAL LIST
							break;
						newCursor = Math::iMin(cursor + columns, itemCount - 1);
						if (cursor != newCursor && cursor <= (indexOffset.y + elementDisplayCount - 1)) {
							indexOffset.y++;
						}
						cursor = newCursor;
						if (ctrl == this || ctrl == scrollbar) {
							interactionPoint = nullptr;
						}
						if (hovered != interactionPoint) {
							hovered = interactionPoint;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_HOVER, e);
						}
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
						if (ctrl == this || ctrl == scrollbar) {
							interactionPoint = nullptr;
						}
						if (hovered != interactionPoint) {
							hovered = interactionPoint;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_HOVER, e);
						}
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
						if (ctrl == this || ctrl == scrollbar) {
							interactionPoint = nullptr;
						}
						if (hovered != interactionPoint) {
							hovered = interactionPoint;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_HOVER, e);
						}
						break;
					case 1: //CONFIRM
						if (ctrl == this || ctrl == scrollbar || ctrl != interactionPoint) {
							interactionPoint = nullptr;
						}
						if (selected != interactionPoint && interactionPoint != nullptr) {
							selected = interactionPoint;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_SELECTED, e);
						}
						break;
					case 2: //CANCEL
						if (ctrl == this || ctrl == scrollbar || ctrl != interactionPoint) {
							interactionPoint = nullptr;
						}
						if (selected != interactionPoint && interactionPoint != nullptr) {
							selected = nullptr;
							submitGuiEvent(GUIEventType::LISTBOX_CHANGE_CANCEL, e);
						}
						break;
					default:
						break;
				}
			}
		}
		return e->isHandled;
	}

	void ListBoxWidget::onRender() {
		int start = 1;
		if (scrollbar == NULL) {
			configureScrollbar(Orientation::VERTICAL);
		}
		if (horizontalScrollbar == NULL) {
			configureScrollbar(Orientation::HORIZONTAL);
		}
		
		Vector2<int> controlOffset = { 0, 0 };
		int currRow = 0;
		int currCol = 0;
		int itemIdx = 0;
		for (int i = 0; i <= itemCount; i++) { //first element is ALWAYS the scrollbar
			auto child = children[i];
			int indOff = elementDisplayCount > 1 ? indexOffset.y : indexOffset.x;
			if (child != scrollbar && child != horizontalScrollbar) {
				itemIdx = i - start;
				currRow = Math::ceil(itemIdx / columns);
				currCol = Math::ceil(itemIdx % columns);
				if (i >= indOff) {
					if (elementDisplayCount == 1 && itemIdx <= currCol + indOff + columns) {
						child->size.y = size.y;
						child->size.x = itemSize;
						child->position.x = controlOffset.x - itemSpacing;
						controlOffset.x += itemSize + itemSpacing;
					}
					else if (elementDisplayCount > 1 && itemIdx <= currRow + (indOff + elementDisplayCount - 1)) {
						child->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
						child->size.x = Math::ceil(size.x/columns);
						child->size.y = itemSize;
						controlOffset.x = currCol * (child->size.x);
						controlOffset.y = currRow * (itemSize + itemSpacing);
						child->position.x = controlOffset.x - itemSpacing;
						child->position.y = controlOffset.y - itemSpacing;
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
					if (selectedImage) {
						renderer->draw(selectedImage, Vector2<float>{child->position.x, child->position.y});
					}
					else {
						renderer->drawBorders(Vector2<float>{child->position.x, child->position.y}, Vector2<float>{child->size.x, child->size.y}, selectColor);
					}
				}
				if (child == hovered) {
					if (hoveredImage) {
						renderer->draw(hoveredImage, Vector2<float>{child->position.x, child->position.y});
					}
					else {
						renderer->drawBorders(Vector2<float>{child->position.x, child->position.y}, Vector2<float>{child->size.x, child->size.y}, hoverColor);
					}
					
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

	void ListBoxWidget::clear() {
		for (auto& c : children) {
			c->setParent(nullptr);
		}
		children.clear();
		selected = nullptr;
		hovered = nullptr;
		resolveLocation();
		setDirty();
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