#ifndef SCROLLAREA_WIDGET_H
#define SCROLLAREA_WIDGET_H

#include "ScrollbarWidget.h"

namespace Arboria {
	enum ScrollbarViewPolicy {
		AsNeeded,
		AlwaysOff,
		AlwaysOn
	};
	class ScrollAreaWidget : public Widget {
		protected:
			virtual void configureScrollbar(Orientation _orientation);
			Vector2<int> indexOffset = { 0, 0 };
			int elementDisplayCount = 1;
		public:
			ScrollAreaWidget(int displayCount = 1, Orientation _scrollOrientation = Orientation::VERTICAL, ScrollbarViewPolicy _viewP = ScrollbarViewPolicy::AlwaysOff);
			ScrollAreaWidget(ScrollbarWidget* widget);
			~ScrollAreaWidget() override;

			ScrollbarWidget* getScrollbar() const { return scrollbar; }
			int getXOffset() { return indexOffset.x; }
			void setXOffset(int x) { indexOffset.x = x; }
			int getYOffset() { return indexOffset.y; }
			void setYOffset(int y) { indexOffset.y = y; }

			/*virtual void run();
			virtual void eventOccured(Event* e) override;
			virtual void onRender();
			virtual void preRender();
			virtual void postRender();*/

			ScrollbarWidget* scrollbar;
			ScrollbarViewPolicy viewPolicy;
			Orientation viewOrientation;
	};
}

#endif