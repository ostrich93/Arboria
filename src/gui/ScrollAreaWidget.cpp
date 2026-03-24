#include "ScrollAreaWidget.h"

namespace Arboria {
	void ScrollAreaWidget::configureScrollbar(Orientation _orientation)
	{

		if (_orientation == Orientation::VERTICAL) {
			scrollbar = createChild<ScrollbarWidget>();
			scrollbar->barOrientation = _orientation;
			scrollbar->size.x = 16;
			scrollbar->size.y = 16;
			scrollbar->position.x = size.x - scrollbar->size.x;
			scrollbar->position.y = 0;
			scrollbar->size.y = size.y;
		}
		else {
			horizontalScrollbar = createChild<ScrollbarWidget>();
			horizontalScrollbar->barOrientation = _orientation;
			horizontalScrollbar->size.x = 16;
			horizontalScrollbar->size.y = 16;
			horizontalScrollbar->size.y = size.y - horizontalScrollbar->size.y;
			horizontalScrollbar->position.x = 0;
			horizontalScrollbar->size.x = size.x;
		}
	}

	ScrollAreaWidget::ScrollAreaWidget(uint32_t columnNumber, uint32_t rowNum) : Widget(), columns(columnNumber), elementDisplayCount(rowNum), viewPolicy(ScrollbarViewPolicy::AsNeeded) {
		if (columnNumber > 1) {
			horizontalScrollbar = createChild<ScrollbarWidget>();
			horizontalScrollbar->barOrientation = Orientation::HORIZONTAL;
			horizontalScrollbar->size.x = 16;
			horizontalScrollbar->size.y = 16;
			horizontalScrollbar->size.y = size.y - horizontalScrollbar->size.y;
			horizontalScrollbar->position.x = 0;
			horizontalScrollbar->size.x = size.x;
		}
		else {
			horizontalScrollbar = nullptr;
		}
		if (rowNum > 1) {
			scrollbar = createChild<ScrollbarWidget>();
			scrollbar->barOrientation = Orientation::VERTICAL;
			scrollbar->size.x = 16;
			scrollbar->size.y = 16;
			scrollbar->position.x = size.x - scrollbar->size.x;
			scrollbar->position.y = 0;
			scrollbar->size.y = size.y;
		}
		else {
			scrollbar = nullptr;
		}

		viewOrientation = columnNumber > rowNum ? Orientation::HORIZONTAL : Orientation::VERTICAL;
	}

	ScrollAreaWidget::ScrollAreaWidget(int displayCount, Orientation _scrollOrientation, ScrollbarViewPolicy _viewP) : Widget(), elementDisplayCount(displayCount), viewPolicy(_viewP), viewOrientation(_scrollOrientation)
	{
		configureScrollbar(viewOrientation);
	}

	ScrollAreaWidget::ScrollAreaWidget(ScrollbarWidget* widget) : Widget(), elementDisplayCount(1), scrollbar(widget)
	{
		if (widget != NULL) {
			viewOrientation = widget->barOrientation;
			viewPolicy = ScrollbarViewPolicy::AsNeeded;
			columns = 1;
			if (viewOrientation == Orientation::VERTICAL) {
				scrollbar = widget;
				horizontalScrollbar = nullptr;
			}
			else {
				horizontalScrollbar = widget;
				scrollbar = nullptr;
			}
		}
		else {
			columns = 1;
			viewOrientation = Orientation::VERTICAL;
			horizontalScrollbar = nullptr;
			viewPolicy = ScrollbarViewPolicy::AsNeeded;
		}
	}

	ScrollAreaWidget::~ScrollAreaWidget() = default;
}