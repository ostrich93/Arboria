#include "Texture.h"

namespace Arboria {
	Texture::Texture(const char* _textureName, size_t _rowAlign) : textureName(_textureName), textureId(0), format(NULL), height(0), width(0), pitch(0), pixelData(NULL), rowAlignment(_rowAlign), refCount(0) {}
	Texture::~Texture() = default;

	void Texture::intializePixelData(GLuint imWidth, GLuint imHeight, GLuint imFormat)
	{
		if (imWidth > 0 && imHeight > 0) {
			//free any current texture data
			freeTexture();

			GLuint imSize = imWidth * imHeight;
			pixelData = (GLubyte*)Mem_ClearedAlloc(imSize); //8 bit

			height = imHeight;
			width = imWidth;
			format = imFormat;
		}

	}

	void Texture::freeTexture()
	{
		if (textureId != 0) {
			glDeleteTextures(1, &textureId);
			textureId = 0;
		}

		if (pixelData != NULL) {
			delete[] pixelData;
			pixelData = NULL;
		}

		width = 0;
		height = 0;
		format = NULL;
	}

	unsigned int Texture::getReferenceCount() const
	{
		return refCount;
	}
	void Texture::ref()
	{
		refCount++;
	}
	bool Texture::deref()
	{
		refCount--;
		if (refCount < 0) {
			delete this;
			return true;
		}
		return false;
	}
	bool Texture::generateTexture()
	{
		bool success = true;
		if (textureId == 0 && pixelData != NULL) {
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			if (textureId != 0) {
				Engine::printError("Texture::generateTexture: Texture already loaded\n");
			}
			else if (pixelData == NULL) {
				Engine::printError("Texture::generateTexture: No pixels to create the texture from.\n");
			}
			success = false;
		}
		return success;
	}

	TextureAtlas::TextureAtlas(const char* _textureName, size_t _rowAlign) : Texture(_textureName, _rowAlign) {}

	TextureAtlas::~TextureAtlas() {
		freeTexture();
	}

	int TextureAtlas::addClip(const Rectangle& rect) {
		return clips.append(rect);
	}

	Rectangle& TextureAtlas::getClip(unsigned int index) {
		return clips[index];
	}

	void TextureAtlas::freeAtlas() {
		clips.clearFree();
	}

	void TextureAtlas::freeTexture() {
		freeAtlas();
		Texture::freeTexture();
	}
	void TextureAtlas::blitPixels(Rectangle& clip, uint8_t* pixels)
	{
		if (pixels != NULL && pixelData != NULL) {
			uint8_t* destPixels = pixelData;
			for (int i = 0; i < clip.h; ++i) {
				memcpy(&destPixels[Math::ftoi((i + clip.y) * clip.w + clip.x)], &pixels[Math::ftoi(i * clip.w)], (int)clip.w);
			}
		}
	}

	bool TextureAtlas::generateTexture() {
		bool success = true;
		if (textureId == 0 && pixelData != NULL) {
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
		else {
			if (textureId != 0) {
				Engine::printError("TextureAtlas::generateTexture: Texture already loaded\n");
			}
			else if (pixelData == NULL) {
				Engine::printError("TextureAtlas::generateTexture: No pixels to create the texture from.\n");
			}
			success = false;
		}
		return success;
	}

	bool TextureAtlas::addSubtexture(int x, int y, int pitch, int h, uint8_t* pixels, GLuint internalFormat, GLuint _format)
	{
		bool success = true;
		if (textureId != 0 && pixelData != NULL && pixels != NULL) {
			if (pitch <= 0 || h <= 0) {
				success = false;
				Engine::printError("TextureAtlas::addSubtexture: Cannot add a subtexture where the width and height of the clip are LEQ 0.\npitch: %d, h: %d\n.", pitch, h);
			}
			if (x < 0 || y < 0) {
				success = false;
				Engine::printError("TextureAtlas::addSubtexture: Cannot add a subtexture when the texture coordinates have values less than 0.\nx: %d, y: %d", x, y);
			}
			else {
				Rectangle tempRect = { (float)x, (float)y, (float)pitch, (float)h };
				blitPixels(tempRect, pixels);
				bindAtlas(); //if already bound, then this function call does nothing, which is what we want
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexSubImage2D(GL_TEXTURE_2D_ARRAY, 0, x, y, pitch, h, format, GL_UNSIGNED_BYTE, pixels);
				//We want the user to unbind the texture manually when they are done putting in all the subtextures
			}
		}
		else {
			success = false;
		}
		return success;
	}

	bool TextureAtlas::bindAtlas()
	{
		if (textureId != 0 && !isBound) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
			isBound = true;
		}

		return isBound;
	}

	bool TextureAtlas::unbindAtlas() {
		if (textureId != 0 && isBound) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
			isBound = false;
		}
		return isBound;
	}
}