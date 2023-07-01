#ifndef DRAW_TEXT_SHADER_H
#define DRAW_TEXT_SHADER_H

#include "ShaderProgram.h"
#include "Font.h"
#include "../utils/Color.h"

namespace Arboria {
	struct DrawTextPass {
		float x;
		float y;
		String text;
		float scale;

		DrawTextPass() : DrawTextPass(0.0f, 0.0f, "") {}
		DrawTextPass(float _x, float _y, const String& _text, float _scale = 1.0f) : x(_x), y(_y), text(_text), scale(_scale) {}
		DrawTextPass(float _x, float _y, const char* _text, float _scale = 1.0f) : x(_x), y(_y), text(_text), scale(_scale) {}
	};

	struct DrawTextBatchCommand {
		Font* font;
		Color color;
		List<DrawTextPass> renderBatches;

		DrawTextBatchCommand() : font(NULL), color(0, 0, 0) {}
		DrawTextBatchCommand(Font* _font, const Color& _color) : font(_font), color(_color) {}
	};

	class DrawTextShader final : public ShaderProgram {
		private:
			GLuint _vbo;
			GLuint _ibo;
			GLuint _vao;

			GLuint uView;
			GLuint uModel;
			GLuint uProjection;
			GLuint uTexAtlas;

			GLuint vPosition;
			GLuint vTexCoord;
			GLuint vColor;

			Font* currentFont;
			Color currentColor;

			GLsizei numInstances = 0;
			GLsizei maxBufferSize;
		public:
			DrawTextShader();
			~DrawTextShader() override;

			void drawInstances(const List<DrawTextBatchCommand>& instances);
		private:
			void getLocations();

	};
}

#endif