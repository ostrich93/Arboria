#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"
#include "../framework/String.h"
#include "../renderer/Font.h"

namespace Arboria {
	class Label : public Widget {
		private:
			String text;
			Font* font;
		public:
			VerticalAlignment valign;
			HorizontalAlignment halign;
			bool wordWrap;
			Vector4<size_t> tint{ 255, 255, 255, 255 };
			//font stuff

			Label(const String& textContent = "", Font* _font = NULL);
			~Label() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void run() override;

			String getText() const;
			void setText(String& _text);

			Font* getFont() const;
			void setFont(Font* _font);
	};
}

#endif