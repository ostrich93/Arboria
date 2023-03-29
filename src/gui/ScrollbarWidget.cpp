#include "ScrollbarWidget.h"
#include "../globals.h"
#include "../framework/Engine.h"
#include "../renderer/Texture.h"

namespace Arboria {
	ScrollbarWidget::ScrollbarWidget(Texture* scrollerImage, bool _isMenu, Orientation orientation) : Widget(), scrollValue(0), scrollStep(1), barOrientation(orientation), maximum(1), minimum(0), menuFlag(_isMenu), scrollbarImage(scrollerImage), color({ 255.f, 255.f, 255.f, 128.f }) {
		if (!scrollbarImage) {
			scrollbarImage = engine->getResourceManager()->loadTexture("gui/scroller.png");
		}
	}

	ScrollbarWidget::~ScrollbarWidget() = default;

	void ScrollbarWidget::setScrollValue(int value) {
		value = Math::iMax(value, minimum);
		value = Math::iMin(value, maximum);

		if (value == scrollValue)
			return;

		scrollValue = value;
		submitGuiEvent(WidgetEventType::SCROLLBAR_CHANGE, NULL);
		setDirty();
	}

	void ScrollbarWidget::setMinimum(int min) {
		if (minimum == min)
			return;
		minimum = min;
		setScrollValue(scrollValue);
		setDirty();
	}

	void ScrollbarWidget::setMaximum(int max) {
		if (maximum == max)
			return;
		maximum = max;
		setScrollValue(scrollValue);
		setDirty();
	}

	void ScrollbarWidget::eventOccured(Event* e) {
		Widget::eventOccured(e);

		if (e->getEventType() == EventType::EVENT_UI_INTERACTION) {
			int pos = static_cast<int>(segmentSize * (scrollValue - minimum));
			auto widgetEvent = dynamic_cast<WidgetEvent*>(e);
			if (widgetEvent->getData().raisedBy == this && widgetEvent->getData().guiEventType == WidgetEventType::KEY_DOWN) {
				InputActionType action = engine->getInputManager()->getActionTranslation(&widgetEvent->getData().keyboardData);
				switch (barOrientation) {
					case Orientation::HORIZONTAL:
						if (action == InputActionType::RIGHT) {
							setScrollValue(scrollValue + 1);
						}
						else if (action == InputActionType::LEFT) {
							setScrollValue(scrollValue - 1);
						}
						break;
					case Orientation::VERTICAL:
					default:
						if (action == InputActionType::DOWN) {
							setScrollValue(scrollValue + 1);
						}
						else if (action == InputActionType::UP) {
							setScrollValue(scrollValue - 1);
						}
						break;
				}
			}
		}
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
				newPos = { position, 0 };
				newSize = { static_cast<int>(segmentSize * (maximum - minimum)), size.y };
				break;
		}
		engine->getSpriteRenderer()->draw(scrollbarImage, newPos, newSize, color);
	}

	void ScrollbarWidget::run() {
		Widget::run();

		int numSegments = (maximum - minimum) + 1;
		int bsize = barOrientation == Orientation::VERTICAL ? size.y : size.x;
		segmentSize = bsize / static_cast<float>(numSegments);
	}
}