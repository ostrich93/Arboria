#include "TextRenderer.h"
#include "../containers/List.h"

namespace Arboria {
	TextRenderer::TextRenderer() : textShader("textRenderShader.vert", "textRenderShader.frag") {
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		glGenBuffers(1, &vbo_id);
		glGenBuffers(1, &ibo_id);
		textShader.use();
		textureAtlasLocation = textShader.getUniformLocation("texture_atlas");
		pColorLocation = textShader.getUniformLocation("pColor");
	}

	TextRenderer::~TextRenderer() {
		if (vao_id) glDeleteVertexArrays(1, &vao_id);
		if (vbo_id) glDeleteBuffers(1, &vbo_id);
		if (ibo_id) glDeleteBuffers(1, &ibo_id);
	}

	void TextRenderer::draw(Font* ft, Vector2<float> pos, Vector3<float> color, const char* text) {
		GlyphAtlas ga = *(ft->glyphAtlas);
		float x = pos.x;
		float y = pos.y;
		float x0, x1, y0, y1;
		List<TextVertexDesc> vertices;
		List<unsigned int> indices;

		int charCount = strlen(text);
		vertices.resize(vertices.getLength() + (4 * charCount + 1));
		indices.resize(indices.getLength() + (6 * charCount + 1));

		for (int i = 0; i < charCount; i++) {
			unsigned char c = text[i];
			Glyph* g = ga.getGlyph(c);

			x0 = x + g->left;
			y0 = y + g->top - g->height;
			x1 = x0 + g->width;
			y1 = y0 + g->height;

			vertices[i * 4 + 0] = { Vector2<float>{x0, y0}, Vector2<float>{g->u0, g->v0 }, Vector2<float>{g->width, g->height}, color };
			vertices[i * 4 + 1] = { Vector2<float>{x0, y1}, Vector2<float>{g->u0, g->v1 }, Vector2<float>{g->width, g->height}, color };
			vertices[i * 4 + 2] = { Vector2<float>{x1, y1}, Vector2<float>{g->u1, g->v1 }, Vector2<float>{g->width, g->height}, color };
			vertices[i * 4 + 3] = { Vector2<float>{x1, y0}, Vector2<float>{g->u1, g->v0 }, Vector2<float>{g->width, g->height}, color };

			indices[i * 6 + 0] = 0;
			indices[i * 6 + 1] = 1;
			indices[i * 6 + 2] = 2;
			indices[i * 6 + 3] = 2;
			indices[i * 6 + 4] = 3;
			indices[i * 6 + 5] = 0;

			x += g->advanceX;

			//ga.bind(*g, 0);

		}

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);

		glBufferData(GL_ARRAY_BUFFER, vertices.getLength() * sizeof(TextVertexDesc), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.getLength() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		//glVertexAttribDivisor(0, 1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexDesc), (GLvoid*)offsetof(TextVertexDesc, screenPosition));

		glEnableVertexAttribArray(1);
		//glVertexAttribDivisor(1, 1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexDesc), (GLvoid*)offsetof(TextVertexDesc, texCoord));

		glEnableVertexAttribArray(2);
		//glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexDesc), (GLvoid*)offsetof(TextVertexDesc, texSize));

		glEnableVertexAttribArray(3);
		//glVertexAttribDivisor(3, 1);
		glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TextVertexDesc), (GLvoid*)offsetof(TextVertexDesc, tint));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i = 0; i < charCount; i++) {
			ga.bind(*ga.getGlyph(text[i]), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(int) * i * 6));
		}

		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}