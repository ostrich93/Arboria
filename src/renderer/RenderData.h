#ifndef RENDERDATA_H
#define RENDERDATA_H

#include "../utils/Vector.h"
#include <GL/glew.h>

//struct SDL_Window;

namespace Arboria {
	class Palette;
	class PaletteImage;
	class RGBAImage;

	class RenderData {
	public:
		virtual ~RenderData();
	};

	class GLPalette final : public RenderData {
	public:
		GLuint tex_id;
		GLPalette(Palette* parent);
		~GLPalette() override;
	};

	class GLPaletteTexture final : public RenderData {
	public:
		GLuint tex_id = 0;
		Vector2<unsigned int> size;

		GLPaletteTexture(PaletteImage* parent);
		~GLPaletteTexture() override;
	};

	class GLRGBATexture final : public RenderData {
	public:
		GLuint tex_id = 0;
		Vector2<unsigned int> size;
		GLRGBATexture(RGBAImage* parent);
		~GLRGBATexture() override;
	};

	class Framebuffer final : public RenderData {
	public:
		GLuint fbo_id;
		GLuint tex_id;
		Vector2<unsigned int> size;

		Framebuffer(Vector2<int> _size);
		Framebuffer(GLuint fbo, Vector2<unsigned int> _sz);
		~Framebuffer() override;
		void bind() const;
		void bindDraw() const;
		void bindRead() const;
	};
}

#endif