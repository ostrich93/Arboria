#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "Label.h"

namespace Arboria {
	class Font;

	class ButtonWidget : public Widget {
		private:
			Label* label;
		public:
			bool hover;
			VerticalAlignment textVAlign;
			HorizontalAlignment textHAlign;

			ButtonWidget(const String& labelName, Font* font = NULL);
			~ButtonWidget() override;
			void run() override;
			void eventOccured(Event* e) override;
			void onRender() override;
	};
}

#endif