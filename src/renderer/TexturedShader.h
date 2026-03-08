#ifndef DRAW_SURFACE_SHADER_H
#define DRAW_SURFACE_SHADER_H

#include "ShaderProgram.h"
#include "../utils/Color.h"

namespace Arboria {

	struct TextureQuadBuffer {
		GLuint position_buffer;
		GLuint vao_id;
	};

	struct TextureVDStruct {
		glm::vec2 position;
		glm::vec2 texCoord;
		Color tint;
	};

	struct TextureVertices {
		TextureVDStruct vertices[4];
	};

	static const glm::vec2 identity_quad[4] = { {0,0}, {0, 1}, {1, 0}, {1,1} };

	class PaletteImage;
	class RGBAImage;
	class Surface;

	class TexturedShader final : public ShaderProgram {
	private:
		List<TextureQuadBuffer> buffers;
		unsigned int currentBuffer;

		GLuint _vao{};
		GLuint _vbo{};

		//uniform locations
		GLint uFlipY{ -1 };
		GLint uTexSize{ -1 };
		GLint uPaletteTexture{ -1 };
		GLint uRgbTexture{ -1 };
		GLint uPalette{ -1 };
		GLint uIsPaletted{ -1 };
		//GLint uColor{ -1 };

		//attribute locations
		GLint vPosition{ -1 };
		GLint vTexCoord{ -1 };
		GLint vTint{ -1 };
		void getLocations();
	public:
		TexturedShader(unsigned int bufferCount);
		~TexturedShader() override;

		void draw(PaletteImage* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, Color tint = { 255, 255, 255, 255 });
		void draw(RGBAImage* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, bool isLinear = false, Color tint = { 255, 255, 255, 255 });
		void draw(Surface* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, bool isLinear = false, Color tint = { 255, 255, 255, 255 });
		void setUniforms(GLuint textureId, glm::vec2 position, glm::vec2 size);

		unsigned int usedBuffers;
	};
}

#endif