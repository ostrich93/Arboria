#include "WidgetContainer.h"

namespace Arboria {
	WidgetContainer::~WidgetContainer() = default;

	void WidgetContainer::onRender() {
		Widget::onRender();
	}
	
	void WidgetContainer::eventOccured(Event* e) {
		Widget::eventOccured(e);
	}

	void WidgetContainer::run() {
		Widget::run();
		resolveLocation();
	}
}