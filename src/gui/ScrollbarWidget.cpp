#include "ScrollbarWidget.h"
#include "../framework/ActionManager.h"
#include "../framework/ResourceManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"

namespace Arboria {
	ScrollbarWidget::ScrollbarWidget(Texture* scrollerImage, bool _isMenu, Orientation orientation) : Widget(), scrollValue(0), scrollStep(1), barOrientation(orientation), maximum(1), minimum(0), menuFlag(_isMenu), scrollbarImage(scrollerImage), color({ (uint8_t)255, (uint8_t)255, (uint8_t)255, (uint8_t)128 }) {
		if (!scrollbarImage) {
			scrollbarImage = resourceManager->loadTexture("gui/scroller.png");
		}
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

		int action = actionManager->getAction(*e);
		int newCursor;
		switch (action) {
		case 6: //UP
			if (barOrientation == Orientation::VERTICAL)
				setScrollValue(scrollValue - 1);
			break;
		case 7: //DOWN
			if (barOrientation == Orientation::VERTICAL)
				setScrollValue(scrollValue + 1);
			break;
		case 8: //LEFT
			if (barOrientation == Orientation::HORIZONTAL)
				setScrollValue(scrollValue - 1);
			break;
		case 9: //RIGHT
			if (barOrientation == Orientation::HORIZONTAL)
				setScrollValue(scrollValue + 1);
			break;
		default:
			break;
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
				newSize = { rect.w, static_cast<int>(segmentSize * (maximum - minimum)) };
				break;
			case Orientation::HORIZONTAL:
			default:
				newPos = { position, 0 };
				newSize = { static_cast<int>(segmentSize * (maximum - minimum)), rect.h };
				break;
		}
		renderer->draw(scrollbarImage, newPos, color, barOrientation == Orientation::VERTICAL ? newSize.y / segmentSize : newSize.x / segmentSize);
		//spriteRenderer->draw(scrollbarImage, newPos, newSize, color);
	}

	void ScrollbarWidget::run() {
		Widget::run();

		int numSegments = (maximum - minimum) + 1;
		int bsize = barOrientation == Orientation::VERTICAL ? rect.h : rect.w;
		segmentSize = bsize / static_cast<float>(numSegments);
	}
}