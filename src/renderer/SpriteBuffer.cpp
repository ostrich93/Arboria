#include "Atlas.h"
#include "SpriteBuffer.h"
#include <assert.h>

namespace Arboria {
	SpriteBuffer::SpriteBuffer(int buffer_size) :
		sprite_buffer_id(0), vertex_buffer_id(0), vao_id(0), bufferCount(0), buffer(buffer_size)
	{
		assert(buffer_size > 0);
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		//The vertex description values are advanced FOR EACH INSTANCE, NOT VERTEX
		glGenBuffers(1, &sprite_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, sprite_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, buffer_size * sizeof(DrawSpriteCommand), nullptr, GL_STREAM_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, texPosition));

		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, spriteSize));

		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, screenPosition));

		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, displaySize));

		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);
		glVertexAttribIPointer(5, 1, GL_INT, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, page));

		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);
		glVertexAttribIPointer(6, 1, GL_INT, sizeof(DrawSpriteCommand), (GLvoid*)offsetof(DrawSpriteCommand, uses_palette));

		//The base vertices for the quad are progressed per index FOR EACH INSTANCE
		Vector2<float> position_values[4] = { {0,0}, {0,1}, {1,0}, {1,1} };
		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(position_values), &position_values[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2<float>), 0);

	}
	SpriteBuffer::~SpriteBuffer()
	{
		if (vao_id)
			glDeleteVertexArrays(1, &vao_id);
		if (sprite_buffer_id)
			glDeleteBuffers(1, &sprite_buffer_id);
		if (vertex_buffer_id)
			glDeleteBuffers(1, &vertex_buffer_id);
	}

	void SpriteBuffer::pushSpriteData(AtlasImageInfo* spriteInfo, glm::vec2 position, Color tint) {
		assert(!isFull());

		auto& d = buffer.allocateNewElement();
		bufferCount++;

		d.texPosition[0] = spriteInfo->bounds.x;
		d.texPosition[1] = spriteInfo->bounds.y;
		d.spriteSize[0] = spriteInfo->imageSize;
		d.spriteSize[1] = spriteInfo->imageSize;
		d.screenPosition[0] = position.x;
		d.screenPosition[1] = position.y;
		d.displaySize[0] = spriteInfo->imageSize;
		d.displaySize[1] = spriteInfo->imageSize;
		d.page = spriteInfo->index;
		d.tint = tint;
	}

	void SpriteBuffer::pushRGBAImage(const AtlasImageInfo& e, glm::vec2 position, glm::vec2 renderSize, Color tint)
	{
		assert(!isFull());

		auto& d = buffer.allocateNewElement();
		bufferCount++;

		d.texPosition[0] = e.bounds.x;
		d.texPosition[1] = e.bounds.y;
		d.spriteSize[0] = e.renderSize.x;
		d.spriteSize[1] = e.renderSize.y;
		d.screenPosition[0] = position.x;
		d.screenPosition[1] = position.y;
		d.displaySize[0] = renderSize.x;
		d.displaySize[1] = renderSize.y;
		d.page = e.index;
		d.uses_palette = 0;
		d.tint = tint;
	}

	void SpriteBuffer::pushPaletteImage(const AtlasImageInfo& e, glm::vec2 position, glm::vec2 renderSize, Color tint)
	{
		assert(!isFull());

		auto& d = buffer.allocateNewElement();
		bufferCount++;

		d.texPosition[0] = e.bounds.x;
		d.texPosition[1] = e.bounds.y;
		d.spriteSize[0] = e.renderSize.x;
		d.spriteSize[1] = e.renderSize.y;
		d.screenPosition[0] = position.x;
		d.screenPosition[1] = position.y;
		d.displaySize[0] = renderSize.x;
		d.displaySize[1] = renderSize.y;
		d.page = e.index;
		d.uses_palette = 1;
		d.tint = tint;
	}

	void SpriteBuffer::draw() {
		if (bufferCount == 0)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, sprite_buffer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DrawSpriteCommand) * bufferCount, buffer.getPtr());
		glBindVertexArray(vao_id);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, bufferCount);
	}
}