#include "../renderer/Texture.h"
#include "GraphicButton.h"
#include "../framework/Lexer.h"
#include "../framework/ResourceManager.h"

namespace Arboria {
	GraphicButton::GraphicButton(Window* gui, Image* _image) : Widget(gui), image(_image)
	{
	}

	void GraphicButton::parseImage(Lexer* src, Image* img) {
		Token tok;
		if (!src->expectTokenType(TOKENTYPE_NUMBER, 0, &tok)) {
			Engine::printError("Could not read the expected image id value");
			return;
		}
		img = resourceManager->loadTexture(tok.getUnsignedIntegerValue(), getPalette() != NULL);
	}
	bool GraphicButton::parseInternalValue(const char* _name, Lexer* src)
	{
		if (String::iCompare(_name, "imageId") == 0) {
			parseImage(src, image);
			return true;
		}
		return Widget::parseInternalValue(_name, src);
	}
}