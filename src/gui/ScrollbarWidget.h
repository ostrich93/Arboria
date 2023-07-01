#ifndef SCROLLBAR_WIDGET_H
#define SCROLLBAR_WIDGET_H

#include "Widget.h"

namespace Arboria {
	const float DEFAULT_SCROLLBAR_SEGMENT_SIZE = 16.f;

	class ScrollbarWidget : public Widget {
		private:
			int scrollValue;
			float segmentSize;
			Texture* scrollbarImage;
		protected:
			int minimum, maximum;
			bool menuFlag;
			Color color;
		public:
			ScrollbarWidget(Texture* scrollerImage = NULL, bool _isMenu = false, Orientation orientation = Orientation::VERTICAL);
			~ScrollbarWidget() override;
			void setMinimum(int newValue = 0);
			void setMaximum(int newValue = 0);
			int getScrollValue() const { return scrollValue; }
			int getMinimum() const { return minimum; }
			int getMaximum() const { return maximum; }
			bool getMenuFlag() const { return menuFlag; }
			Color getColor() { return color; }
			void setColor(Color _color) { color = _color; }
			void setMenuFlag(bool flag) { menuFlag = flag; }
			void setScrollValue(int value);
			void scrollMin() { setScrollValue(minimum); }
			void scrollMax() { setScrollValue(maximum); }

			bool onEvent(AEvent* e) override;
			void onRender() override;
			void run() override;

			int scrollStep;
			Orientation barOrientation;
	};
}

#endif