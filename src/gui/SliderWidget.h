#ifndef SLIDER_WIDGET_H
#define SLIDER_WIDGET_H

#include "Label.h"

namespace Arboria {
	class SliderWidget : public Widget {
		public:
			SliderWidget(const String& labelName, Font* font = NULL, int curr = 50, int min = 0, int max = 100, int tick = 1);
			~SliderWidget() override;
			void run() override;
			bool onEvent(AEvent* e) override;
			void onRender() override;

			void setValue(int _value);
			void setInterval(int _interval) { interval = _interval; }
			int getValue() const { return value; }
			int getNumSegments() const { return static_cast<int>((maxValue - minValue) / interval); }
			float getKnobSize() const { return drawRect.w / getNumSegments(); }
		private:
			Label* label;
			int minValue;
			int maxValue;
			int value;
			unsigned interval;

			Rectangle drawRect;
			Rectangle knobRect;
	};
}

#endif