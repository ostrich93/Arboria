#include "ScrollAreaWidget.h"

namespace Arboria {
	void ScrollAreaWidget::configureScrollbar(Orientation _orientation)
	{

		if (_orientation == Orientation::VERTICAL) {
			scrollbar = createChild<ScrollbarWidget>();
			scrollbar->barOrientation = _orientation;
			scrollbar->rect.w = 16;
			scrollbar->rect.h = 16;
			scrollbar->rect.x = rect.w - scrollbar->rect.w;
			scrollbar->rect.y = 0;
			scrollbar->rect.h = rect.h;
		}
		else {
			horizontalScrollbar = createChild<ScrollbarWidget>();
			horizontalScrollbar->barOrientation = _orientation;
			horizontalScrollbar->rect.w = 16;
			horizontalScrollbar->rect.h = 16;
			horizontalScrollbar->rect.y = rect.h - horizontalScrollbar->rect.h;
			horizontalScrollbar->rect.x = 0;
			horizontalScrollbar->rect.w = rect.w;
		}
	}

	ScrollAreaWidget::ScrollAreaWidget(uint32_t columnNumber, uint32_t rowNum) : Widget(), columns(columnNumber), elementDisplayCount(rowNum), viewPolicy(ScrollbarViewPolicy::AsNeeded) {
		
	}

	ScrollAreaWidget::ScrollAreaWidget(int displayCount, Orientation _scrollOrientation, ScrollbarViewPolicy _viewP) : Widget(), elementDisplayCount(displayCount), viewPolicy(_viewP), viewOrientation(_scrollOrientation)
	{
	}

	ScrollAreaWidget::ScrollAreaWidget(ScrollbarWidget* widget) : Widget(), elementDisplayCount(1), scrollbar(widget)
	{
		if (widget != NULL) {
			viewOrientation = widget->barOrientation;
			viewPolicy = ScrollbarViewPolicy::AsNeeded;
		}
	}

	ScrollAreaWidget::~ScrollAreaWidget() = default;
}