#include "ScrollAreaWidget.h"

namespace Arboria {
	void ScrollAreaWidget::configureScrollbar(Orientation _orientation)
	{
		scrollbar = createChild<ScrollbarWidget>();
		scrollbar->barOrientation = _orientation;
		scrollbar->size.x = 16;
		scrollbar->size.y = 16;
		if (_orientation == Orientation::VERTICAL) {
			scrollbar->location.x = size.x - scrollbar->size.x;
			scrollbar->location.y = 0;
			scrollbar->size.y = size.y;
		}
		else {
			scrollbar->location.y = size.y - scrollbar->size.y;
			scrollbar->location.x = 0;
			scrollbar->size.x = size.x;
		}
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