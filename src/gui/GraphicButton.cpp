#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "GraphicButton.h"
#include "../framework/Lexer.h"
#include "../framework/ResourceManager.h"

namespace Arboria {
	GraphicButton::GraphicButton(Window* gui, Image* _image) : Widget(gui), image(_image)
	{
	}

	GraphicButton::~GraphicButton() = default;

	bool GraphicButton::onEvent(AEvent* e)
	{ //Future implementations will have button click sounds, etc.
		return Widget::onEvent(e);
	}

	void GraphicButton::onRender()
	{
		Widget::onRender();
		if (!image) return;

		Vector2<float> pos = { 0,0 };
		if (Vector2<unsigned int>(size) == image->getSize())
			renderer->draw(image, pos);
		else
			renderer->drawScaled(image, pos, size);
		renderer->drawBorders(Vector2<float>{3, 3}, size - 6, { 100, 100, 100 });
	}

	void GraphicButton::run()
	{
		Widget::run();
	}

	void GraphicButton::unloadResources()
	{
		image = nullptr;
		Widget::unloadResources();
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