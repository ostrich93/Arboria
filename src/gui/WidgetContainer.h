#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAIENR_H__

#include "Widget.h"

namespace Arboria {

	class WidgetContainer : public Widget {
		public:
			~WidgetContainer() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void run() override;
			//void preRender() override;
			//void postRender() override;
	};
}

#endif