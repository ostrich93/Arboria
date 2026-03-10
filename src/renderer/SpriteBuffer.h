#ifndef SPRITE_BUFFER_H
#define SPRITE_BUFFER_H

#include "DrawSpriteCommand.h"
#include "Texture.h"

namespace Arboria {
	class AtlasImageInfo;

	class SpriteBuffer {
	private:
		GLuint sprite_buffer_id;
		GLuint vertex_buffer_id;
		GLuint vao_id;

		unsigned int bufferCount;
		List<DrawSpriteCommand> buffer;
	public:
		SpriteBuffer(int buffer_size);
		~SpriteBuffer();
		bool isFull() const { return bufferCount >= buffer.getGranularity(); }
		bool isEmpty() const { return buffer.getLength() == 0; }
		void reset() { bufferCount = 0; }
		void draw();
		void pushSpriteData(AtlasImageInfo* spriteInfo, glm::vec2 position, Color tint);
		void pushRGBAImage(const AtlasImageInfo& e, glm::vec2 position, glm::vec2 renderSize, Color tint);
		void pushPaletteImage(const AtlasImageInfo& e, glm::vec2 position, glm::vec2 renderSize, Color tint);
	};
}

#endif