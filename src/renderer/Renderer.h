#ifndef RENDERER_H
#define RENDERER_H

#include "DrawSurfaceShader.h"
#include "DrawTextShader.h"
#include "DrawColoredPrimitiveShader.h"

namespace Arboria {
	struct VertexData {
		int x0;
		int y0;
		int x1;
		int y1;
		float u0;
		float v0;
		float u1;
		float v1;
	};

	
	class Font;
	class Renderer {
		private:
			DrawSurfaceShader* surfaceShader;
			DrawTextShader* textShader;
			DrawColoredPrimitiveShader* coloredPrimitiveShader;
			List<DrawSurfaceCommand> surfaceCommandBuffer;
			List<DrawTextBatchCommand> textCommandBuffers;
			List<DrawColoredPrimitiveCommand> lineCommandBuffers;
			List<DrawColoredPrimitiveCommand> rectCommandBuffers;
		public:
			Renderer(): surfaceShader(NULL), textShader(NULL), coloredPrimitiveShader(NULL){}
			~Renderer();
			void initialize();
			void draw(Texture* _texture, Vector2<GLfloat> _screenPosition, Color _color, GLfloat _scale = 1.0f);
			//void draw(TextureAtlas* _textureAtlas, Vector2<GLfloat> _screenPosition, Color _color, GLint _clipIndex = 0, GLfloat _scale = 1.0f);
			void drawText(const char* text, Font* font, Vector2<GLfloat> _screenPosition, Color _color, GLfloat _scale = 1.0f);
			void drawLine(Vector2<GLfloat> _positions[2], Color _colors[2]);
			void drawRectangle(Vector2<GLfloat> _positions[4], Color _colors[4]);
			void flush();
			void flushSurfaceCommands();
			void flushText();
			void flushLines();
			void flushRectangles();
	};

	class SpriteRenderer {
	private:
		unsigned int vao_id;
		unsigned int vbo_id;
		ShaderProgram spriteShader;
		GLint modelLocation;
		GLint spriteColorLocation;
	protected:
		void initRenderData();
	public:
		SpriteRenderer();

		~SpriteRenderer();

		void draw(Texture* texture, Vector2<int> position, Vector2<int> size, Vector4<float> tint, float rotate = 0);
	};
}

#endif