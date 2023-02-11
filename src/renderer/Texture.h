#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "../utils/Vector.h"

namespace Arboria {
	class Sprite {
	public:

	private:
		friend class Texture;
	protected:
		Texture* img;
		Vector2 offsetPosition;
		Vector2 dimensions;

	};

	class Texture {
	public:
		Texture(const char* _textureName, GLuint _texId = 0, size_t _rowAlign = 16);
		virtual ~Texture();

		GLuint getTextureId() const { return textureId; }
		GLuint setTextureId(const GLuint _textureId) { textureId = _textureId; }
		const char* getTextureName() const { return textureName; }
		GLenum getFormat() const { return format; }
		void setFormat(const GLenum _format) { format = _format; }
		GLuint getHeight() const { return height; }
		void setHeight(const GLuint _height) { height = _height; }
		GLuint getWidth() const { return width; }
		void setWidth(const GLuint _width) { width = _width; }
		GLuint getWrapS() const { return wrapS; }
		void setWrapS(const GLuint _wrapS) { wrapS = _wrapS; }
		GLuint getWrapT() const { return wrapT; }
		void setWrapT(const GLuint _wrapT) { wrapT = _wrapT; }
		GLuint getFilterMin() const { return filterMin; }
		void setFilterMin(const GLuint _filterMin) { filterMin = _filterMin; }
		GLuint getFilterMax() const { return filterMax; }
		void setFilterMax(const GLuint _filterMax) { wrapT = _filterMax; }
		size_t getRowAlignment() const { return rowAlignment; }
		void setRowAlignment(const size_t _rowAlign) { rowAlignment = _rowAlign; }
		unsigned int getReferenceCount() const;
		void ref();
		void deref();

		void generateTexture(unsigned int w, unsigned int h, unsigned char* pixelData);
	private:
		GLuint textureId;
		const char* textureName;
		GLenum format;
		GLuint height, width;
		GLuint wrapS, wrapT;
		GLuint filterMin, filterMax;
		size_t rowAlignment;
		unsigned int refCount;
	};
}

#endif