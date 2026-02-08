#ifndef TEXTURE_H
#define TEXTURE_H
#include "../utils/Vector.h"
#include "../framework/ResourceObject.h"
#include "../framework/Engine.h"
#include "../utils/Color.h"
#include <assert.h>

namespace Arboria {
	class RenderData;
	class Palette;

	class Sprite {
	private:
		friend class Image;
	protected:
		Image* img;
		Vector2<unsigned int> offsetPosition;
		Vector2<unsigned int> dimensions;

	};

	enum class ImageLockUse {
		Read,
		Write,
		ReadWrite
	};

	class Image : public ResourceObject {
	public:
		Image(Vector2<unsigned int> size, size_t _rowAlign = 16);
		virtual ~Image();
;
		unsigned int getFormat() const { return format; }
		void setFormat(const GLuint _format) { format = _format; }
		unsigned int getInternalFormat() const { return internalFormat; }
		void setInternalFormat(const GLuint _InternalFormat) { internalFormat = _InternalFormat; }
		unsigned int getHeight() const { return imageSize.y; }
		void setHeight(const GLuint _height) { imageSize.y = _height; }
		unsigned int getWidth() const { return imageSize.x; }
		void setWidth(const GLuint _width) { imageSize.x = _width; }
		Vector2<unsigned int> getSize() { return imageSize; }
		void setSize(Vector2<unsigned int> size) { imageSize = size; }

		size_t getRowAlignment() const { return rowAlignment; }
		void setRowAlignment(const size_t _rowAlign) { rowAlignment = _rowAlign; }
		unsigned int getReferenceCount() const;
		void ref();
		bool deref();

		virtual bool generateTexture();
		RenderData* renderData; //can be GL, DX11, etc.
		
	protected:
		unsigned int format;
		unsigned int internalFormat;
		Vector2<unsigned int> imageSize;
		size_t rowAlignment;
		unsigned int refCount;
		bool dirty;
	};

	class Surface : public Image {
	public:
		Surface(Vector2<unsigned int> sz);
		~Surface() override;
	};

	class RGBAImage : public Image {
	private:
		friend class RGBAImageLock;
		Color* pixels;
	public:
		RGBAImage(Vector2<unsigned int> sz, Color initialColor = { 0, 0, 0, 0 });
		~RGBAImage() override;
		static void blit(RGBAImage* src, RGBAImage* dst, Vector2<unsigned int> srcOffset = { 0, 0 },
			Vector2<unsigned int> dstOffset = { 0, 0 });
	};

	class RGBAImageLock {
	private:
		RGBAImage* img;
		RGBAImageLock(const RGBAImageLock&) = delete;
		ImageLockUse use;
	public:
		RGBAImageLock(RGBAImage* image, ImageLockUse _use = ImageLockUse::Write);
		~RGBAImageLock();
		Color get(const Vector2<unsigned int>& pos) const {
			assert(use == ImageLockUse::Read || use == ImageLockUse::ReadWrite);
			unsigned offset = pos.y * this->img->getWidth() + pos.x;
			assert(offset < this->img->imageSize.x * this->img->imageSize.y);
			return this->img->pixels[offset];
		}
		void set(const Vector2<unsigned int>& pos, const Color& colour) {
			assert(use == ImageLockUse::Write || use == ImageLockUse::ReadWrite);
			unsigned offset = pos.y * this->img->getWidth() + pos.x;
			assert(offset < this->img->imageSize.x * this->img->imageSize.y);
			this->img->pixels[offset] = colour;
		}

		void* getData();
	};

	class PaletteImage : public Image {
	private:
		friend class PaletteImageLock;
		uint8_t* indices;
	public:
		PaletteImage(Vector2<unsigned int> sz, uint8_t initialIndex = 0);
		~PaletteImage() override;
		RGBAImage* toRGBImage(Palette* p);
		static void blit(PaletteImage* src, PaletteImage* dst, Vector2<unsigned int> srcOffset = { 0, 0 },
			Vector2<unsigned int> dstOffset = { 0, 0 });
	};

	class PaletteImageLock {
	private:
		PaletteImage* img;
		PaletteImageLock(const PaletteImageLock&) = delete; //disallow copy
		ImageLockUse use;
	public:
		PaletteImageLock(PaletteImage* image, ImageLockUse _use = ImageLockUse::Write);
		~PaletteImageLock();
		uint8_t get(const Vector2<unsigned int>& pos) const {
			assert(use == ImageLockUse::Read || use == ImageLockUse::ReadWrite);
			unsigned offset = pos.y * this->img->getWidth() + pos.x;
			assert(offset < this->img->imageSize.x * this->img->imageSize.y);
			return this->img->indices[offset];
		}
		void set(const Vector2<unsigned int>& pos, const uint8_t idx) {
			assert(use == ImageLockUse::Write || use == ImageLockUse::ReadWrite);
			unsigned offset = pos.y * this->img->getWidth() + pos.x;
			assert(offset < this->img->imageSize.x * this->img->imageSize.y);
			this->img->indices[offset] = idx;
		}

		void* getData();
	};

	class TextureAtlas : public Image { //spritesheets/glyph atlases
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