#include "Renderer.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "../containers/List.h"

namespace Arboria {
	Renderer::~Renderer() = default;

	struct VertexDescription {
		Vector4<GLfloat> vertex;
		Vector2f screen_position;
		Vector2f screen_size;
	};

	struct SpriteBuffer {
		GLuint vao_id;
		GLuint vbo_id;
		GLuint position_buffer;
	};

	class SpriteProgram : public ShaderProgram {
		private:
			List<SpriteBuffer> buffers;
			GLint viewportSizeLocation;
			GLint spriteColorLocation;
	};

}