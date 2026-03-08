#ifndef SPRITE_SHADER_H
#define SPRITE_SHADER_H

#include "Atlas.h"
#include "ShaderProgram.h"
#include "SpriteBuffer.h"

namespace Arboria {
	class SpriteShader final : public ShaderProgram {
		private:
			List<SpriteBuffer*> buffers;
			unsigned int currentBuffer;

			Atlas palette_spritesheet;
			Atlas rgba_spritesheet;

			GLint vPosition{ -1 };
			GLint vTexPosition{ -1 };

			GLint vSpriteSize{ -1 };
			GLint vScreenPosition{ -1 };
			GLint vDisplaySize{ -1 };
			GLint vPage{ -1 };
			GLint vUsesPalette{ -1 };
			GLint vTint{ -1 };

			//uniform locations
			GLint uViewportSize{ -1 };
			GLint uPalettedSpritesheets{ -1 };
			GLint uRgbaSpritesheets{ -1 };
			GLint uPalette{ -1 };

			void getLocations();
		public:
			unsigned int usedBuffers;
			SpriteShader(unsigned int bufferSize, unsigned int bufferCount, unsigned int spritesheetPageSize);
			~SpriteShader() override;

			void flush(glm::uvec2 viewport_size);
			void draw(AtlasImageInfo* spriteInfo, glm::vec2 screenPos, GLuint atlasesTextureId, glm::uvec2 viewport_size, Color tint = {255, 255, 255, 255});
			void draw(RGBAImage* i, glm::vec2 screenPos, glm::vec2 renderSize, glm::uvec2 viewport_size, Color tint = { 255, 255, 255, 255 });
			void draw(PaletteImage* i, glm::vec2 screenPos, glm::vec2 renderSize, glm::uvec2 viewport_size, Color tint = { 255, 255, 255, 255 });
			AtlasImageInfo getOrCreateSprite(Image* i);
			void invalidateSprite(uint32_t spriteId); //used for temp sprites like text strings

	};
}

#endif