#include "TextRenderer.h"
#include "GlyphAtlas.h"
#include "../containers/List.h"
#include "Font.h"

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

	void TextRenderer::draw(Font* ft, const Vector2<int>* pos, Vector4<float> color, String& text) {
		GlyphAtlas ga = *ft->getGlyphAtlas();
		float u0, v0, u1, v1;
		float minX, minY, maxX, maxY;
		Vector2<int> cursor = *pos;
		List<TextVertexDesc> vertices;
		List<unsigned int> indices;
		int j = 0;

		for (int x = 0; x < text.length(); x++) {
			int advancer = 0;
			//int utf decoding
			char ch = text[x];
			if (ch == '\n') {
				cursor.x = pos->x;
				cursor.y += ft->getSize().height;
				continue;
			}
			else {
				const Glyph* glyph = ga.getGlyph(ch);
				if (glyph) {
					int destX = pos->x;
					int destY = pos->y - glyph->bearingY + ft->getSize().height;
					int destW = glyph->width;
					int destH = glyph->height;

					u0 = glyph->left / ga.getWidth();
					v0 = glyph->top / ga.getHeight();
					u1 = (glyph->left + glyph->width) / ga.getWidth();
					v1 = (glyph->top + glyph->height) / ga.getHeight();

					minX = destX;
					minY = destY;
					maxX = destX + destW;
					maxY = destY + destH;

					vertices[j * 4 + 0] = { Vector2<float>{minX, minY}, Vector2<float>{u0, v0}, Vector2<float>{destW, destH}, color };
					vertices[j * 4 + 1] = { Vector2<float>{minX, maxY}, Vector2<float>{u0, v1}, Vector2<float>{destW, destH}, color };
					vertices[j * 4 + 2] = { Vector2<float>{maxX, maxY}, Vector2<float>{u1, v1}, Vector2<float>{destW, destH}, color };
					vertices[j * 4 + 3] = { Vector2<float>{maxX, minY}, Vector2<float>{u1, v0}, Vector2<float>{destW, destH}, color };

					indices[j * 6 + 0] = 0;
					indices[j * 6 + 1] = 1;
					indices[j * 6 + 2] = 2;
					indices[j * 6 + 3] = 2;
					indices[j * 6 + 4] = 3;
					indices[j * 6 + 5] = 0;

					advancer += glyph->advanceX;
					j++;
				}
				cursor.x += advancer;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);

		glBufferData(GL_ARRAY_BUFFER, vertices.getLength() * sizeof(TextVertexDesc), &vertices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.getLength() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
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
		
		glDrawElements(GL_TRIANGLES, vertices.getLength(), GL_UNSIGNED_BYTE, NULL);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}