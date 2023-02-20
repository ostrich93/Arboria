#ifndef __LABEL_H__
#define __LABEL_H__

#include "Widget.h"

namespace Arboria {
	class Label : public Widget {
		private:
			const char* text;
		public:
			VerticalAlignment valign;
			HorizontalAlignment halign;
			bool wordWrap;
			//font stuff

			Label(const char* textContent = "");
			~Label() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void run() override;

	};
}

#endif