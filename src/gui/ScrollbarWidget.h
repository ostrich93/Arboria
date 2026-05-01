#ifndef SCROLLBAR_WIDGET_H
#define SCROLLBAR_WIDGET_H

#include "Widget.h"

namespace Arboria {
	const float DEFAULT_SCROLLBAR_SEGMENT_SIZE = 16.f;
	enum ScrollbarViewPolicy {
		AsNeeded,
		AlwaysOff,
		AlwaysOn
	};


	class ScrollbarWidget : public Widget {
		private:
			int scrollValue;
			float segmentSize;
		protected:
			int minimum, maximum;
			Color color, gripperColor;
		public:
			ScrollbarWidget(Window* gui, Orientation orientation = Orientation::VERTICAL, Color _gripperColor = {128, 128, 128, 128});
			~ScrollbarWidget() override;
			void setMinimum(int newValue = 0);
			void setMaximum(int newValue = 0);
			int getScrollValue() const { return scrollValue; }
			int getMinimum() const { return minimum; }
			int getMaximum() const { return maximum; }
			float getSegmentSize() const { return segmentSize; }
			Color getColor() { return color; }
			void setColor(Color _color) { color = _color; }
			void setSegmentSize(float _segSize) { segmentSize = _segSize; }
			void setScrollValue(int value);
			void scrollNext();
			void scrollPrev();
			void scrollMin() { setScrollValue(minimum); }
			void scrollMax() { setScrollValue(maximum); }

			bool onEvent(AEvent* e) override;
			void onRender() override;
			void run() override;
			bool parseInternalValue(const char* _name, Lexer* src) override;

			int scrollStep;
			Orientation barOrientation;
	};
}

#endif