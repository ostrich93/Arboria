#include "SliderWidget.h"
#include "WidgetEnums.h"
#include "../renderer/Font.h"
#include "../framework/ActionManager.h"

namespace Arboria {
	SliderWidget::SliderWidget(const String& labelName, Font* font, int curr, int min, int max, int tick) : Widget(), value(curr), minValue(min), maxValue(max), interval(tick) {
		if (font == NULL) {
			font == fontManager->getFont("logotypegothicregular", "", 24);
		}
		label = new Label(labelName, font);
		
	}

	SliderWidget::~SliderWidget() = default;

	void SliderWidget::run()
	{
		Widget::run();
	}

	bool SliderWidget::onEvent(AEvent* e)
	{
		Widget::onEvent(e);
		if (!e->isHandled && flags & WidgetStateFlags::WIDGET_FOCUSED) {
			int action = actionManager->getAction(*e);
			switch (action) {
				case 8:
					setValue(value - interval);
					if (isDirty()) {
						e->isHandled = true;
					}
					break;
				case 9:
					setValue(value + interval);
					if (isDirty()) {
						e->isHandled = true;
					}
				default:
					break;
			}
		}
		return e->isHandled;
	}
	void SliderWidget::onRender()
	{
		Widget::onRender();
		knobRect.x = drawRect.x + static_cast<int>(value * getKnobSize());
		//drawRectangles;
		//draw value text next to the slider
	}
	void SliderWidget::setValue(int _value)
	{
		if (_value == value) return;

		int nValue;
		if (_value < value) {
			nValue = Math::iMax(_value, minValue);
			if (nValue != value) {
				value = nValue;
				setDirty();
			}
		}
		else if (_value > value) {
			nValue = Math::iMin(_value, maxValue);
			if (nValue != value) {
				value = nValue;
				setDirty();
			}
		}
	}
}