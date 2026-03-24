#ifndef RENDERER_H
#define RENDERER_H

#include "TexturedShader.h"
#include "SpriteShader.h"
#include "ColoredPrimitiveShader.h"

namespace Arboria {

	class RGBAImage;
	class PaletteImage;
	class TextureCache;
	class Font;
	class Palette;
	class Surface;

	class RendererFrameBufferBinding {
	private:
		RendererFrameBufferBinding(const RendererFrameBufferBinding&) = delete;
		Surface* prevBinding;
	public:
		RendererFrameBufferBinding(Surface* surface);
		~RendererFrameBufferBinding();
	};

	class Renderer {
		private:
			unsigned int spriteBufferSize = 16384;
			unsigned int spriteBufferCount = 40;
			unsigned int spritesheetPageSize = 2048;
			Vector2<int> maxSpritePackSize = { 256, 256 };
			TexturedShader* texturedShader;
			SpriteShader* spriteShader;
			ColoredPrimitiveShader* coloredPrimitiveShader;
			Palette* current_palette = nullptr;

			enum RendererState {
				Idle,
				BatchingSprites
			};

			Surface* default_surface;
			Surface* current_surface;

			RendererState state;

			uint32_t _drawCount = 0;
			uint32_t _ttfGlId = 0;

		public:
			Renderer();
			~Renderer();
			void initialize();
			void newFrame();

			void clear(Color c = { 0, 0, 0, 0 });
			void setPalette(Palette* pal);
			Palette* getPalette();
			void bind(Surface* s);
			void setSurface(Surface* s);
			Surface* getCurrentSurface();
			void draw(Image* i, const Vector2<float>& position, bool renderDefaultSurface = false);
			void drawScaled(Image* i, const Vector2<float>& position, const Vector2<float>& renderSize, bool linearScale = false, bool renderDefaultSurface = false);
			void drawTinted(Image* i, Vector2<float> position, Color tint, bool renderDefaultSurface = false);
			void drawText(Font* font, String& text, int32_t x, int32_t y);
			void drawLine(Vector2<float> _p1, Vector2<float> _p2, Color _color, float thickness);
			void drawRectangle(Vector2<float> _position, Vector2<float> _size, Color c);
			void drawBorders(Vector2<float> _position, Vector2<float> _size, Color c, float thickness = 1.0f);
			void flush();

			int maxSpriteBuffers = 0;
			int maxTextureBuffers = 0;
			int maxColoredPrimitiveBuffers = 0;
	};
}

#endif