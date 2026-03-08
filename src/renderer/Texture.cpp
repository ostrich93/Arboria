#include "Texture.h"
#include "Palette.h"
#include "../utils/math.h"

namespace Arboria {
	Image::Image(Vector2<unsigned int> size, size_t _rowAlign) : format(NULL), internalFormat(NULL), imageSize(size), rowAlignment(_rowAlign), refCount(0), dirty(false), renderData(nullptr) {}
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