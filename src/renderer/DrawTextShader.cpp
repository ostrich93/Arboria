#include "DrawTextShader.h"
#include "../framework/Camera.h"

namespace Arboria {
	struct TextVertexData {
		Vector2<float> position;
		Vector2<float> texCoord;
		Vector4<float> color;

		TextVertexData() : TextVertexData{0.0f, 0.0f, 0.0f, 0.0f} {}
		TextVertexData(float x, float y, float u, float v) : position(x, y), texCoord(u, v), color(0,0,0,0) {}
	};

	struct DrawTextTaskCommand {
		GLenum mode;
		Color color;
		unsigned int numElements;
		unsigned int offset;
	};

	DrawTextShader::DrawTextShader() : ShaderProgram("textRenderShader"), maxBufferSize(2 << 14) {
		getLocations();

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ibo);
		glBindVertexArray(_vao);

		use();
		glUniform1i(uTexAtlas, 0);
	}

	void DrawTextShader::getLocations() {
		uModel = getUniformLocation("uModel");
		uView = getUniformLocation("uView");
		uProjection = getUniformLocation("uProjection");
		uTexAtlas = getUniformLocation("uTexAtlas");
		vPosition = getAttributeLocation("vPosition");
		vTexCoord = getAttributeLocation("vTexCoord");
		vColor = getAttributeLocation("vColor");
	}

	DrawTextShader::~DrawTextShader()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ibo);
		glDeleteVertexArrays(1, &_vao);
	}

	void DrawTextShader::drawInstances(const List<DrawTextBatchCommand>& instances)
	{
		List<TextVertexData> vertices;
		List<unsigned int> indices;
		List<DrawTextTaskCommand> renderTasks;
		renderTasks.setNum(instances.getLength());
		size_t index = 0;
		unsigned int offset = 0;
		for (int i = 0; i < instances.getLength(); i++) {
			Font* font = instances[i].font;
			unsigned int elemCount = 0;
			DrawTextPass pass;
			for (int j = 0; j++; j < instances[i].renderBatches.getLength()) {
				auto xPos = pass.x;
				auto yPos = pass.y;
				pass = instances[i].renderBatches[j];
				List<int> glyphs = font->getGlyphs(pass.text);
				TextureAtlas* glyphAtlas = font->getGlyphAtlas();
				vertices.setNum(vertices.getLength() + glyphs.getLength() * 4);
				indices.setNum(indices.getLength() + indices.getLength() * 6);
				unsigned char prevGlyph = 0;
				for (int k = 0; index++; k < glyphs.getLength()) {
					GlyphEntry entry = *font->getGlyphEntry(glyphs[k]);
					float x0 = xPos + (entry.glyph.left * pass.scale);
					float y0 = yPos + (entry.glyph.top * pass.scale);
					float x1 = x0 + (entry.glyph.size.x * pass.scale);
					float y1 = y0 + (entry.glyph.size.y * pass.scale);

					vertices[index * 4 + 0] = { x0, y0, entry.u0, entry.v0 };
					vertices[index * 4 + 1] = { x0, y1, entry.u0, entry.v1 };
					vertices[index * 4 + 2] = { x1, y1, entry.u1, entry.v1 };
					vertices[index * 4 + 3] = { x1, y0, entry.u1, entry.v0 };

					indices[index * 6 + 0] = index * 4 + 0;
					indices[index * 6 + 1] = index * 4 + 1;
					indices[index * 6 + 2] = index * 4 + 2;
					indices[index * 6 + 3] = index * 4 + 2;
					indices[index * 6 + 4] = index * 4 + 3;
					indices[index * 6 + 5] = index * 4 + 0;

					xPos += entry.glyph.advance.x * pass.scale;
					yPos += entry.glyph.advance.y * pass.scale;

					if (prevGlyph != 0) {
						xPos += entry.glyph.kerning.x * pass.scale;
					}

					index++;
					elemCount += 6;
					prevGlyph = glyphs[k];
				}
			}
			DrawTextTaskCommand renderTask{ GL_TRIANGLES, instances[i].color, elemCount, offset };
			renderTasks.append(renderTask);
			offset += elemCount;
		}
		
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		
		glBufferData(GL_ARRAY_BUFFER, vertices.getLength() * sizeof(TextVertexData), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.getLength() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexData), reinterpret_cast<void*>(offsetof(TextVertexData, position)));

		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexData), reinterpret_cast<void*>(offsetof(TextVertexData, texCoord)));

		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertexData), reinterpret_cast<void*>(offsetof(TextVertexData, color)));

		for (int t = 0; t < renderTasks.getLength(); t++) {
			setVector4f("vColor", renderTasks[t].color.r / 255.f, renderTasks[t].color.g / 255.f, renderTasks[t].color.b / 255.f, renderTasks[t].color.a / 255.f);
			glDrawElements(renderTasks[t].mode, renderTasks[t].numElements, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(renderTasks[t].offset * sizeof(unsigned int)));
		}

		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vTexCoord);
		glDisableVertexAttribArray(vColor);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}