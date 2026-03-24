#ifndef COLORED_PRIMITIVE_SHADER_H
#define COLORED_PRIMITIVE_SHADER_H

#include "ShaderProgram.h"
#include "../utils/Color.h"

namespace Arboria {

	class ColoredPrimitiveShader final : public ShaderProgram {
		private:
			struct ColoredPrimitiveVertex {
				Vector2<float> position;
				Color color;
			};

			struct ColoredPrimitiveVertexDescription {
				ColoredPrimitiveVertex vertices[4];
			};

			struct ColoredPrimitiveBuffer {
				GLuint vertex_buffer;
				GLuint vao_id;
				ColoredPrimitiveVertexDescription data;
			};

			List<ColoredPrimitiveBuffer> buffers;
			unsigned int currentBuffer;

			GLint uViewportSize{ -1 };

			GLint vPosition{ -1 };
			GLint vColor{ -1 };
			void getLocations();
		public:
			ColoredPrimitiveShader(unsigned int bufferCount);
			~ColoredPrimitiveShader() override;
			void drawQuad(Vector2<float> positions[4], Color colors[4], Vector2<unsigned int> viewportSize);
			void drawLine(Vector2<float> positions[2], Color colors[2], Vector2<unsigned int> viewportSize, float thickness);
	};
}

#endif