#ifndef TEXTURE_H
#define TEXTURE_H
#include "../utils/Vector.h"
#include "../framework/String.h"
#include "../framework/Engine.h"

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
		Texture(const char* _textureName, size_t _rowAlign = 16);
		virtual ~Texture();

		void intializePixelData(GLuint imWidth, GLuint imHeight, GLuint imFormat = GL_RGBA);
		virtual void freeTexture();
		GLuint getTextureId() const { return textureId; }
		GLuint setTextureId(const GLuint _textureId) { textureId = _textureId; }
		const char* getTextureName() const { return textureName.c_str(); }
		GLuint getFormat() const { return format; }
		void setFormat(const GLuint _format) { format = _format; }
		GLuint getHeight() const { return height; }
		void setHeight(const GLuint _height) { height = _height; }
		GLuint getWidth() const { return width; }
		void setWidth(const GLuint _width) { width = _width; }
		uint8_t* getPixelData() const { return pixelData; }

		size_t getRowAlignment() const { return rowAlignment; }
		void setRowAlignment(const size_t _rowAlign) { rowAlignment = _rowAlign; }
		unsigned int getReferenceCount() const;
		void ref();
		bool deref();

		virtual bool generateTexture();
		
	protected:
		GLuint textureId = 0;
		String textureName;
		GLuint format;
		GLuint height, width;
		Vector2<int> imageSize;
		size_t rowAlignment;
		unsigned int refCount;
		//texture buffers have the pixel data, which gets called with generateTexture?
		uint8_t* pixelData;
		GLint pitch;
	};

	class TextureAtlas : public Texture { //spritesheets/glyph atlases
		public:
			struct AtlasEntry { //stores texcoords.
				float u0;
				float v0;
				float u1;
				float v1;
			};
		public:
			TextureAtlas(const char* _textureName, size_t _rowAlign = 16);
			~TextureAtlas();
			int addClip(const Rectangle& rect);
			Rectangle& getClip(unsigned int index);
			void freeAtlas();
			void freeTexture() override;
			void blitPixels(Rectangle& clip, uint8_t* pixels);
			bool generateTexture() override;
			bool addSubtexture(int x, int y, int pitch, int h, uint8_t* pixels, GLuint internalFormat = GL_RGBA, GLuint _format = GL_RGBA);
			bool bindAtlas();
			bool unbindAtlas();
		protected:
			bool isBound = false;
			List<Rectangle> clips; //tex positions
			List<AtlasEntry> entries;
	};
}

#endif