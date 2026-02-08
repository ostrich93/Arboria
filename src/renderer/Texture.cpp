#include "Texture.h"
#include "Palette.h"
#include "../utils/math.h"

namespace Arboria {
	Image::Image(Vector2<unsigned int> size, size_t _rowAlign) : format(NULL), internalFormat(NULL), imageSize(size), rowAlignment(_rowAlign), refCount(0), dirty(false) {}
	Image::~Image() = default;

	unsigned int Image::getReferenceCount() const
	{
		return refCount;
	}
	void Image::ref()
	{
		refCount++;
	}
	bool Image::deref()
	{
		refCount--;
		if (refCount < 0) {
			delete this;
			return true;
		}
		return false;
	}

	TextureAtlas::TextureAtlas(const char* _textureName, size_t _rowAlign) : Image(_textureName, _rowAlign) {}

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
		Image::freeTexture();
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
				Engine::printError("TextureAtlas::generateTexture: Image already loaded\n");
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

	PaletteImage::PaletteImage(Vector2<unsigned int> sz, uint8_t initialIndex) : Image(sz), indices(new uint8_t[sz.x * sz.y])
	{
		memset(indices, 0, sz.x * sz.y);
	}

	PaletteImage::~PaletteImage() = default;

	RGBAImage* PaletteImage::toRGBImage(Palette* p)
	{
		RGBAImage* i = new RGBAImage(imageSize);
		RGBAImageLock imgLock(i, ImageLockUse::Write);
		for (unsigned int y = 0; y < imageSize.y; y++) {
			for (unsigned x = 0; x < imageSize.x; x++) {
				uint8_t idx = indices[y * imageSize.x + x];
				imgLock.set({ x, y }, p->getColor(idx));
			}
		}
		return i;
	}

	void PaletteImage::blit(PaletteImage* src, PaletteImage* dst, Vector2<unsigned int> srcOffset, Vector2<unsigned int> dstOffset)
	{
		PaletteImageLock read(src, ImageLockUse::Read);
		PaletteImageLock write(dst, ImageLockUse::Write);

		Vector2<unsigned int> size = { Math::iMin(src->getWidth() - srcOffset.x, dst->getWidth() - dstOffset.x),
									Math::iMin(src->getHeight() - srcOffset.y, dst->getHeight() - dstOffset.y) };

		Vector2<unsigned int> pos;
		for (pos.y = 0; pos.y < size.y; pos.y++) {
			for (pos.x = 0; pos.x < size.x; pos.x++) {
				Vector2<unsigned int> readPos = srcOffset + pos;
				Vector2<unsigned int> writePos = dstOffset + pos;
				write.set(writePos, read.get(readPos));
			}
		}
	}

	RGBAImage::RGBAImage(Vector2<unsigned int> sz, Color initialColor) :
		Image(sz), pixels(reinterpret_cast<Color*>(operator new[](imageSize.x * imageSize.y * sizeof(Color))))
	{
		if (initialColor.r == initialColor.g && initialColor.r == initialColor.b && initialColor.r == initialColor.a)
		{
			memset(reinterpret_cast<void*>(pixels), initialColor.r, sizeof(Color) * imageSize.x * imageSize.y);
		}
		else {
			for (unsigned int i = 0; i < imageSize.x * imageSize.y; i++) {
				pixels[i] = initialColor;
			}
		}
	}

	RGBAImage::~RGBAImage() = default;

	void RGBAImage::blit(RGBAImage* src, RGBAImage* dst, Vector2<unsigned int> srcOffset, Vector2<unsigned int> dstOffset)
	{
		RGBAImageLock read{ src, ImageLockUse::Read };
		RGBAImageLock write{ dst, ImageLockUse::Write };

		Vector2<unsigned int> size = { Math::iMin(src->getWidth() - srcOffset.x, dst->getWidth() - dstOffset.x),
									 Math::iMin(src->getHeight() - srcOffset.y, dst->getHeight() - dstOffset.y)};

		Vector2<unsigned int> pos;
		for (pos.y = 0; pos.y < size.y; pos.y++) {
			for (pos.x = 0; pos.x < size.x; pos.x++) {
				Vector2<unsigned int> readPos = srcOffset + pos;
				Vector2<unsigned int> writePos = dstOffset + pos;
				write.set(writePos, read.get(readPos));
			}
		}
	}

	PaletteImageLock::PaletteImageLock(PaletteImage* image, ImageLockUse _use) :
		img(image), use(_use)
	{
	}

	PaletteImageLock::~PaletteImageLock() = default;

	void* PaletteImageLock::getData()
	{
		return img->indices;
	}

	RGBAImageLock::RGBAImageLock(RGBAImage* image, ImageLockUse _use) :
		img(image), use(_use)
	{
	}

	RGBAImageLock::~RGBAImageLock() = default;

	void* RGBAImageLock::getData() {
		return img->pixels;
	}

	Surface::Surface(Vector2<unsigned int> sz) : Image(sz)
	{
	}

	Surface::~Surface() = default;
}