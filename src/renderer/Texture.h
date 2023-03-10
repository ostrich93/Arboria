#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "../utils/Vector.h"
#include "../containers/List.h"
#include <SDL_opengl.h>

namespace Arboria {
	class Sprite {
	private:
		friend class Texture;
	protected:
		Texture* img;
		Vector2<unsigned int> offsetPosition;
		Vector2<unsigned int> dimensions;

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
		size_t rowAlignment;
		unsigned int refCount;
		//texture buffers have the pixel data, which gets called with generateTexture?
	};


}

#endif