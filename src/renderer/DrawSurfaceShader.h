#ifndef DRAW_SURFACE_SHADER_H
#define DRAW_SURFACE_SHADER_H

#include "ShaderProgram.h"
#include "../utils/Color.h"

namespace Arboria {

	class Texture;

	struct SurfaceVertexDescription {
		Vector2<float> position;
		Vector2<float> texCoord;
		Color color;
	};

	struct DrawSurfaceCommand {
		Vector2<GLfloat> screenSize;
		Vector2<GLfloat> screenPosition;
		Vector2<GLfloat> texturePosition; //for normal textures, it's 0,0
		Vector2<GLfloat> textureSize;
		//Vector2<GLfloat> textureCoord;
		Vector4<GLfloat> boundaries;
		Vector4<GLfloat> color;
		GLfloat scaleValue;
	};

	struct SurfaceTextureBuffer {
		GLuint vbo;
		GLuint vao;
	};

	struct SurfaceVertexData {
		SurfaceVertexDescription vertices[4];
	};

	class DrawSurfaceShader final : public ShaderProgram {
		private:
			//uniforms
			GLuint uTexture;
			GLuint uTexSize;
			GLuint uModel;
			GLuint uView;
			GLuint uProjection;
			//vertex attributes
			GLuint vScreenPosition = 0;
			GLuint vTexCoord = 1;
			GLuint vColor = 2;

			List<SurfaceTextureBuffer> buffers;
			unsigned int currentBuffer = 0;

		public:
			DrawSurfaceShader(unsigned int bufferCount);
			~DrawSurfaceShader() override;
			void drawTexture(Texture* img, Vector2<float> screenPosition, Vector2<float> displayOnScreenSize, Vector2<float> texSize, Vector2<float> rotationCenter, float rotationRadians, Color _color);

	};
}

#endif