#ifndef GRAPHIC_BUTTON_H
#define GRAPHIC_BUTTON_H

#include "Widget.h"

namespace Arboria {
	class Image;

	class GraphicButton : public Widget {
	private:
		Image* image;

	public:
		GraphicButton(Window* gui, Image* _image = nullptr);
		~GraphicButton() override;
		bool onEvent(AEvent* e) override;
		void onRender() override;
		void run() override;
		void unloadResources() override;

		void setImage(Image* _image) { image = _image; }

		void parseImage(Lexer* src, Image* img) override;
		bool parseInternalValue(const char* _name, Lexer* src) override;
	};
}

#endif