#ifndef DRAW_COLORED_PRIMITIVE_SHADER_H
#define DRAW_COLORED_PRIMITIVE_SHADER_H

#include "ShaderProgram.h"
#include "../utils/Color.h"

namespace Arboria {
	struct DrawColoredPrimitiveVertex {
		Vector2<float> position;
		Vector4<float> color;
	};

	struct DrawColoredPrimitiveCommand {
		DrawColoredPrimitiveVertex vertices[4];
	};

	class DrawColoredPrimitiveShader final : public ShaderProgram {
		private:
			GLuint _vao;
			GLuint _vbo;
			GLuint uModel;
			GLuint uViewport;
			GLuint uProjection;
			GLuint vPosition;
			GLuint vColor;
		public:
			DrawColoredPrimitiveShader();
			~DrawColoredPrimitiveShader() override;
			void drawLineInstances(const List<DrawColoredPrimitiveCommand>& instances);
			void drawRectInstances(const List<DrawColoredPrimitiveCommand>& instances);
		private:
			void getLocations();
	};
}

#endif