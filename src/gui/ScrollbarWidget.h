#ifndef __SCROLLBAR_WIDGET_H__
#define __SCROLLBAR_WIDGET_H__

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
			Vector4<float> color;
		public:
			ScrollbarWidget(Texture* scrollerImage = NULL, bool _isMenu = false, Orientation orientation = Orientation::VERTICAL);
			~ScrollbarWidget() override;
			void setMinimum(int newValue = 0);
			void setMaximum(int newValue = 0);
			int getScrollValue() const { return scrollValue; }
			int getMinimum() const { return minimum; }
			int getMaximum() const { return maximum; }
			bool getMenuFlag() const { return menuFlag; }
			Vector4<float> getColor() { return color; }
			void setColor(Vector4<float> _color) { color = _color; }
			void setMenuFlag(bool flag) { menuFlag = flag; }
			void setScrollValue(int value);
			void scrollMin() { setScrollValue(minimum); }
			void scrollMax() { setScrollValue(maximum); }

			void eventOccured(Event* e) override;
			void onRender() override;
			void run() override;

			int scrollStep;
			Orientation barOrientation;
	};
}

#endif