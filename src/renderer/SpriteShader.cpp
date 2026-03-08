#include "SpriteShader.h"

namespace Arboria {

	static const auto RGBA_IMAGE_TEX_SLOT = GL_TEXTURE0;
	static const auto PALETTE_IMAGE_TEX_SLOT = GL_TEXTURE1;
	static const auto PALETTE_TEX_SLOT = GL_TEXTURE2;
	static const auto SCRATCH_TEX_SLOT = GL_TEXTURE3;

	static const int RGBA_IMAGE_TEX_IDX = 0;
	static const int PALETTE_IMAGE_TEX_IDX = 1;
	static const int PALETTE_TEX_IDX = 2;

	SpriteShader::SpriteShader(unsigned int bufferSize, unsigned int bufferCount, unsigned int spritesheetPageSize) : ShaderProgram("spriteShader"),
	currentBuffer(0), usedBuffers(0), palette_spritesheet(spritesheetPageSize, GL_R8UI), rgba_spritesheet(spritesheetPageSize, GL_RGBA8) {
		assert(bufferSize > 0);
		assert(bufferCount > 0);

		compileShader(name);
		use();
		
		getLocations();

		for (int i = 0; i < bufferCount; i++) {
			buffers.append(new SpriteBuffer(bufferSize));
		}
	}

	void SpriteShader::getLocations() {
		uViewportSize = getUniformLocation("uViewportSize");
		uPalettedSpritesheets = getUniformLocation("uPalettedSpritesheets");
		assert(uPalettedSpritesheets >= 0);
		glUniform1i(uPalettedSpritesheets, PALETTE_IMAGE_TEX_IDX);
		uRgbaSpritesheets = getUniformLocation("uRgbaSpritesheets");
		assert(uRgbaSpritesheets >= 0);
		glUniform1i(uRgbaSpritesheets, RGBA_IMAGE_TEX_IDX);
		uPalette = getUniformLocation("uPalette");
		assert(uPalette >= 0);
		glUniform1i(uPalette, PALETTE_TEX_IDX);

		vPosition = getAttributeLocation("vPosition");
		vTexPosition = getAttributeLocation("vTexPosition");
		vSpriteSize = getAttributeLocation("vSpriteSize");
		vScreenPosition = getAttributeLocation("vScreenPosition");
		vDisplaySize = getAttributeLocation("vDisplaySize");
		vPage = getAttributeLocation("vPage");
		vUsesPalette = getAttributeLocation("vUsesPalette");
		vTint = getAttributeLocation("vTint");
	}

	SpriteShader::~SpriteShader()
	{
		if (programId) {
			glDeleteProgram(programId);
		}
	}

	void SpriteShader::flush(glm::uvec2 viewport_size) {
		if (buffers[currentBuffer]->isEmpty())
			return;
		use();

		glActiveTexture(PALETTE_IMAGE_TEX_SLOT);
		glBindTexture(GL_TEXTURE_2D_ARRAY, palette_spritesheet.tex_id); //how to pass in atlasesTexture from texture cache to here?
		glActiveTexture(RGBA_IMAGE_TEX_SLOT);
		glBindTexture(GL_TEXTURE_2D_ARRAY, rgba_spritesheet.tex_id);

		glUniform2f(uViewportSize, (float)viewport_size.x, (float)viewport_size.y);

		buffers[currentBuffer]->draw();
		buffers[currentBuffer]->reset();
		currentBuffer = (currentBuffer + 1) % buffers.size();
		usedBuffers++;
	}
	void SpriteShader::draw(AtlasImageInfo* spriteInfo, glm::vec2 screenPos, GLuint atlasesTextureId, glm::uvec2 viewport_size, Color tint)
	{
		if (buffers[currentBuffer]->isFull()) {
			flush(viewport_size);
		}
		buffers[currentBuffer]->pushSpriteData(spriteInfo, screenPos, tint);
	}

	void SpriteShader::draw(RGBAImage* i, glm::vec2 screenPos, glm::vec2 renderSize, glm::uvec2 viewport_size, Color tint)
	{
		auto sprite = rgba_spritesheet.getOrLoadRgbSprite(i);
		if (buffers[currentBuffer]->isFull()) {
			flush(viewport_size);
		}
		buffers[currentBuffer]->pushRGBAImage(sprite, screenPos, renderSize, tint);
	}

	void SpriteShader::draw(PaletteImage* i, glm::vec2 screenPos, glm::vec2 renderSize, glm::uvec2 viewport_size, Color tint) {
		auto sprite = palette_spritesheet.getOrLoadPaletteSprite(i);
		if (buffers[currentBuffer]->isFull()) {
			flush(viewport_size);
		}
		buffers[currentBuffer]->pushPaletteImage(sprite, screenPos, renderSize, tint);
	}

	AtlasImageInfo SpriteShader::getOrCreateSprite(Image* i)
	{
		auto palImage = dynamic_cast<PaletteImage*>(i);
		if (palImage) {
			return palette_spritesheet.getOrLoadPaletteSprite(palImage);
		}

		auto rgbImage = dynamic_cast<RGBAImage*>(i);
		if (rgbImage)
			return rgba_spritesheet.getOrLoadRgbSprite(rgbImage);

		return AtlasImageInfo();
	}

	void SpriteShader::invalidateSprite(uint32_t spriteId) {
		palette_spritesheet.invalidateSprite(spriteId);
	}
}