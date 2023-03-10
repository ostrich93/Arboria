#ifndef __LABEL_H__
#define __LABEL_H__

#include "Widget.h"
#include "../renderer/FontManager.h"

namespace Arboria {
	class Label : public Widget {
		private:
			const char* text;
			Font* font;
		public:
			VerticalAlignment valign;
			HorizontalAlignment halign;
			bool wordWrap;
			Vector3<size_t> tint{ 255, 255, 255 };
			//font stuff

			Label(const char* textContent = "", Font* _font = NULL);
			~Label() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void run() override;

			const char* getText() const;
			void setText(const char*& _text);

			Font* getFont() const;
			void setFont(Font* _font);
	};
}

#endif