#include "Texture.h"

namespace Arboria {
	Texture::Texture(const char* _textureName, GLuint _texId, size_t _rowAlign) : textureName(_textureName), textureId(_texId), format(GL_RGB), height(0), width(0), rowAlignment(_rowAlign), refCount(0)
	{
		glGenTextures(1, &this->textureId);
	}
	Texture::~Texture() = default;
	unsigned int Texture::getReferenceCount() const
	{
		return refCount;
	}
	void Texture::ref()
	{
		refCount++;
	}
	void Texture::deref()
	{
		refCount--;
	}
	void Texture::generateTexture(unsigned int w, unsigned int h, unsigned char* pixelData)
	{
		width = w;
		height = h;
		glBindTexture(GL_TEXTURE_2D, textureId);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}