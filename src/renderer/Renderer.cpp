#include "Renderer.h"
#include "Texture.h"
#include "Palette.h"
#include "RenderData.h"
#include "CodepointView.h"
#include "Font.h"
#include "../framework/Camera.h"
#include "../framework/ResourceManager.h"
#include <optional>
#include "../gui/Widget.h"

namespace Arboria {
	RendererFrameBufferBinding::RendererFrameBufferBinding(Surface* surface) :
		prevBinding(renderer->getCurrentSurface())
	{
		renderer->setSurface(surface);
	}

	RendererFrameBufferBinding::~RendererFrameBufferBinding() {
		renderer->setSurface(prevBinding);
	}

	Renderer::Renderer() : state(RendererState::Idle)
	{
		spriteShader = new SpriteShader(spriteBufferSize, spriteBufferCount, spritesheetPageSize);
		texturedShader = new TexturedShader(100);
		coloredPrimitiveShader = new ColoredPrimitiveShader(100);

		default_surface = new Surface(Vector2<int>{renderDevice->getWindowWidth(), renderDevice->getWindowHeight()});
		default_surface->renderData = new Framebuffer(0, Vector2<int>(renderDevice->getWindowWidth(), renderDevice->getWindowHeight()));
		current_surface = default_surface;
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		spriteShader->use().setVector2f("uViewportSize", glm::vec2{ renderDevice->getWindowWidth(), renderDevice->getWindowHeight() });
		coloredPrimitiveShader->use().setVector2f("uViewportSize", glm::vec2{ renderDevice->getWindowWidth(), renderDevice->getWindowHeight() });
		
		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		if (spritesheetPageSize > (unsigned int)maxTextureSize) {
			spritesheetPageSize = std::min(spritesheetPageSize, (unsigned int)maxTextureSize);
		}
	}
	Renderer::~Renderer()
	{
		delete texturedShader;
		delete spriteShader;
		delete coloredPrimitiveShader;
	}

	void Renderer::initialize()
	{
		texturedShader = new TexturedShader(100);
		spriteShader = new SpriteShader(spriteBufferSize, spriteBufferCount, spritesheetPageSize);
		coloredPrimitiveShader = new ColoredPrimitiveShader(100);
		glViewport(0, 0, renderDevice->getWindowWidth(), renderDevice->getWindowHeight());
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void Renderer::newFrame()
	{
		if (spriteShader->usedBuffers > maxSpriteBuffers) {
			maxSpriteBuffers = spriteShader->usedBuffers;
		}
		if (texturedShader->usedBuffers > maxTextureBuffers) {
			maxTextureBuffers = texturedShader->usedBuffers;
		}
		if (coloredPrimitiveShader->usedBuffers > maxColoredPrimitiveBuffers) {
			maxColoredPrimitiveBuffers = coloredPrimitiveShader->usedBuffers;
		}
		spriteShader->usedBuffers = 0;
		texturedShader->usedBuffers = 0;
		coloredPrimitiveShader->usedBuffers = 0;
	}

	void Renderer::clear(Color c)
	{
		flush();
		glClearColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::setPalette(Palette* pal)
	{
		flush();
		current_palette = pal;
		if (pal == nullptr) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);
			return;
		}

		auto gPal = dynamic_cast<GLPalette*>(pal->renderData);
		if (!gPal) {
			gPal = new GLPalette(pal);
			pal->renderData = gPal;
		}

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gPal->tex_id);
	}

	Palette* Renderer::getPalette()
	{
		return current_palette;
	}

	void Renderer::bind(Surface* s)
	{
		auto fbo = dynamic_cast<Framebuffer*>(s->renderData);
		if (!fbo) {
			fbo = new Framebuffer(s->getSize());
			s->renderData = fbo;
		}
		fbo->bind();
	}

	void Renderer::setSurface(Surface* s)
	{
		flush();
		current_surface = s;
		auto fbo = dynamic_cast<Framebuffer*>(s->renderData);
		if (!fbo) {
			fbo = new Framebuffer(s->getSize());
			s->renderData = fbo;
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fbo_id);
		glViewport(0, 0, s->getWidth(), s->getHeight());
	}

	Surface* Renderer::getCurrentSurface()
	{
		return current_surface;
	}

	void Renderer::draw(Image* i, const Vector2<float>& position)
	{
		drawScaled(i, position, i->getSize(), false);
	}

	void Renderer::drawScaled(Image* i, const Vector2<float>& position, const Vector2<float>& renderSize, bool linearScale)
	{
		auto viewport_size = current_surface->getSize();
		auto paletteImage = dynamic_cast<PaletteImage*>(i);
		if (paletteImage) {
			if (paletteImage->getWidth() <= maxSpritePackSize.x && paletteImage->getHeight() <= maxSpritePackSize.y) {
				if (state != RendererState::BatchingSprites) {
					flush();
					state = RendererState::BatchingSprites;
				}
				spriteShader->draw(paletteImage, position, paletteImage->getSize(), viewport_size);
			}
			else {
				flush();
				texturedShader->draw(paletteImage, position, paletteImage->getSize(), viewport_size);
			}
			return;
		}

		auto rgbImage = dynamic_cast<RGBAImage*>(i);
		if (rgbImage) {
			if (rgbImage->getWidth() <= maxSpritePackSize.x && rgbImage->getHeight() <= maxSpritePackSize.y && !linearScale) {
				if (state != RendererState::BatchingSprites) {
					flush();
					state = RendererState::BatchingSprites;
				}
				spriteShader->draw(rgbImage, position, rgbImage->getSize(), viewport_size);
			}
			else {
				flush();
				texturedShader->draw(rgbImage, position, rgbImage->getSize(), viewport_size, linearScale);
			}
			return;
		}

		auto surface = dynamic_cast<Surface*>(i);
		if (surface) {
			flush();
			texturedShader->draw(surface, position, surface->getSize(), viewport_size, linearScale);
			return;
		}
	}

	void Renderer::drawTinted(Image* i, Vector2<float> position, Color tint)
	{
		auto viewport_size = current_surface->getSize();
		auto paletteImage = dynamic_cast<PaletteImage*>(i);
		if (paletteImage) {
			if (paletteImage->getWidth() <= maxSpritePackSize.x && paletteImage->getHeight() <= maxSpritePackSize.y) {
				if (state != RendererState::BatchingSprites) {
					flush();
					state = RendererState::BatchingSprites;
				}
				spriteShader->draw(paletteImage, position, i->getSize(), viewport_size, tint);
			}
			else {
				flush();
				texturedShader->draw(paletteImage, position, i->getSize(), viewport_size, tint);
			}
			return;
		}

		auto rgbaImage = dynamic_cast<RGBAImage*>(i);
		if (rgbaImage) {
			if (rgbaImage->getWidth() <= maxSpritePackSize.x && rgbaImage->getHeight() <= maxSpritePackSize.y) {
				if (state != RendererState::BatchingSprites) {
					flush();
					state = RendererState::BatchingSprites;
				}
				spriteShader->draw(rgbaImage, position, i->getSize(), viewport_size, tint);
			}
			else {
				flush();
				texturedShader->draw(rgbaImage, position, i->getSize(), viewport_size, false, tint);
			}
			return;
		}

		auto surface = dynamic_cast<Surface*>(i);
	}

	void Renderer::drawText(Font* font, String& text, int32_t x, int32_t y)
	{
		CodepointView codepoints(text);
		std::optional<size_t> ttfRunIndex{};

		for (auto it = codepoints.begin(); it != codepoints.end(); it++) {
			auto codepoint = *it;

			if (!ttfRunIndex.has_value()) {
				ttfRunIndex = it.getIndex();
			}
		}

		if (ttfRunIndex.has_value()) {
			auto len = text.size() - *ttfRunIndex;


			auto baseId = static_cast<uint32_t>(0xFFF) - 1023;
			auto imageId = baseId + _ttfGlId;

			spriteShader->invalidateSprite(imageId);

			if (font == nullptr) {
				return;
			}

			PaletteImage* img = font->getString(text.substring(ttfRunIndex.value(), len));
			img->resId = imageId;
			auto viewport_size = current_surface->getSize();

			if (img->getWidth() <= maxSpritePackSize.x && img->getHeight() <= maxSpritePackSize.y) {
				if (state != RendererState::BatchingSprites) {
					flush();
					state = RendererState::BatchingSprites;
				}

				_ttfGlId++;
				if (_ttfGlId >= 1023)
					_ttfGlId = 0;

				spriteShader->draw(img, { x, y }, img->getSize(), viewport_size);
			}
			else {
				flush();

				_ttfGlId++;
				if (_ttfGlId >= 1023)
					_ttfGlId = 0;

				texturedShader->draw(img, { x, y }, img->getSize(), viewport_size);
			}
		}
	}

	void Renderer::drawLine(Vector2<float> _p1, Vector2<float> _p2, Color _color, float thickness)
	{
		flush();
		Vector2<float> positions[2];
		Color colors[2];

		auto viewport_size = current_surface->getSize();
		colors[0] = _color;
		colors[1] = _color;
		positions[0] = _p1;
		positions[1] = _p2;

		coloredPrimitiveShader->drawLine(positions, colors, viewport_size, thickness);
	}

	void Renderer::drawRectangle(Vector2<float> _position, Vector2<float> _size, Color c)
	{
		flush();
		Vector2<float> positions[4];
		Color colors[4];

		auto viewport_size = current_surface->getSize();

		for (int i = 0; i < 4; i++) {
			positions[i] = _position + _size * identity_quad[i];
			colors[i] = c;
		}
		coloredPrimitiveShader->drawQuad(positions, colors, viewport_size);
	}

	void Renderer::drawBorders(Vector2<float> _position, Vector2<float> _size, Color c, float thickness)
	{
		Vector2<float> p0 = _position;
		Vector2<float> p1 = _position + _size;

		Vector2<float> rectA = { p0 };
		Vector2<float> sizeA = { _size.x - thickness, thickness };

		Vector2<float> rectB = { p1.x - thickness, p0.y };
		Vector2<float> sizeB = { thickness, _size.y - thickness };

		Vector2<float> rectC = { p0.x + thickness, p1.y - thickness };
		Vector2<float> sizeC = { _size.x - thickness, thickness };

		Vector2<float> rectD = { p0.x, p0.y + thickness };
		Vector2<float> sizeD = { thickness, _size.y - thickness };

		drawRectangle(rectA, sizeA, c);
		drawRectangle(rectB, sizeB, c);
		drawRectangle(rectC, sizeC, c);
		drawRectangle(rectD, sizeD, c);
	}

	void Renderer::flush() {
		auto viewport_size = current_surface->getSize();
		if (state == RendererState::BatchingSprites) {
			spriteShader->flush(viewport_size);
			state = RendererState::Idle;
		}
	}
}