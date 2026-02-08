#include "ResourceManager.h"
#include "../FileSystem.h"
#include "../renderer/Font.h"
#include "../renderer/Texture.h"
#include "../utils/Numerics.h"
#include "../definitions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../renderer/stb_image.h"
#include <physfs.h>
#include <mutex>

namespace Arboria {

	constexpr int32_t fontStringCacheSize = 256;

	const HashTable<uint32_t, String> indexToSpriteFilenames = {
		{0, "background/mainBackground.png"},
		{1, "gui/ui.png"}
	};

	const HashTable<uint32_t, String> indexToPaletteFilenames = {
		{0, "palettes/defaultgray.png"}
	};

	const HashTable<uint32_t, String> indexToFontFilenames = {
		{0, "logotyperegulargothic.ttf"}
	};

	class ResourceManagerImpl : public ResourceManager {
	private:
		HashTable<String, Image*> textureCache;
		std::recursive_mutex textureCacheLock;

		HashTable<String, Font*> fontCache;
		std::recursive_mutex fontCacheLock;

		FontStringCacheEntry _fontStringCache[fontStringCacheSize] = {};
		int32_t _fontStringCacheCount = 0;
		int32_t _fontStringCacheHitCount = 0;
		int32_t _fontStringCacheMissCount = 0;
		std::recursive_mutex fontStringCacheLock;

		uint32_t getFontStringCacheHash(Font* font, const String& text);
		void disposeFontStringCacheEntry(FontStringCacheEntry* entry);
		void freeFontStringCacheEntry(PaletteImage* surface);
	public:
		ResourceManagerImpl();
		~ResourceManagerImpl() override = default;
		Image* loadTexture(const uint32_t resourceId, bool isPaletted) override;
		Font* loadFont(const String& filename, const int ptSize) override;

		PaletteImage* getFontStringCacheEntry(Font* font, const String& text) override;
		void putFontStringCacheEntry(Font* font, const String& text, PaletteImage*& img) override;
	};

	ResourceManager* ResourceManager::createResourceManager()
	{
		return new ResourceManagerImpl();
	}

	uint32_t ResourceManagerImpl::getFontStringCacheHash(Font* font, const String& text)
	{
		uint32_t hash = static_cast<uint32_t>(((reinterpret_cast<uint32_t>(font) * 23) ^ 0xAAAAAAAA) & 0xFFFFFFFF);

		for (int i = 0; i < text.length(); i++) {
			hash = Numerics::ror32(hash, 3) ^ (text[i] * 13);
		}

		return hash;
	}

	void ResourceManagerImpl::disposeFontStringCacheEntry(FontStringCacheEntry* entry)
	{
		if (entry->image != nullptr) {
			freeFontStringCacheEntry(entry->image);
			entry->text.clear();
			entry->image = nullptr;
			entry->font = nullptr;
		}
	}

	void ResourceManagerImpl::freeFontStringCacheEntry(PaletteImage* surface)
	{
		free(surface);
	}

	ResourceManagerImpl::ResourceManagerImpl()
	{
	}

	Image* ResourceManagerImpl::loadTexture(const uint32_t resourceId, bool isPaletted)
	{
		std::lock_guard<std::recursive_mutex> l(textureCacheLock);
		auto imagePath = indexToSpriteFilenames[resourceId];

		if (imagePath.isEmpty()) {
			return nullptr;
		}

		Image* img = textureCache.get(imagePath);
		if (img) {
			return img;
		}

		unsigned int bytesRead;
		unsigned char* fileData = readBytesFromPhysFS(imagePath.c_str(), &bytesRead);
		if (bytesRead == 0)
			return nullptr;

		int imgBPP = 0;
		int width, height;

		stbi_set_flip_vertically_on_load(1);
		unsigned char* imgData = stbi_load_from_memory(fileData, bytesRead, &width, &height, &imgBPP, 0);

		if (imgData == NULL) {
			free(fileData);
			return nullptr;
		}

		if (width == 0 || height == 0) {
			stbi_image_free(imgData);
			free(fileData);
			return nullptr;
		}

		if (isPaletted) {
			PaletteImage* palImage = new PaletteImage({ width, height });
			PaletteImageLock pLock(palImage, ImageLockUse::Write);

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pLock.set({ x, y }, imgData[(y * width) + x]);
				}
			}

			img = palImage;
		}
		else {
			RGBAImage* rgbImage = new RGBAImage({ width, height });
			RGBAImageLock rgbLock(rgbImage, ImageLockUse::Write);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int baseIdx = imgBPP * width * y + imgBPP * x;
					Color c(imgData[baseIdx], imgData[imgBPP * width * y + imgBPP * x + 1],
						imgData[imgBPP * width * y + imgBPP * x + 2], imgBPP == 4 ? imgData[4 * width * y + 4 * x + 3] : 255);
					rgbLock.set({ x,y }, c);
				}
			}

			img = rgbImage;
		}
		img->resId = resourceId;
		img->filePath = imagePath;
		switch (imgBPP) {
		case 1:
			img->setFormat(GL_R8UI);
			img->setInternalFormat(GL_RED_INTEGER);
			break;
		case 2:
			img->setFormat(GL_RG8);
			img->setInternalFormat(GL_RG);
			break;
		case 3:
			img->setFormat(GL_RGB8);
			img->setInternalFormat(GL_RGB);
			break;
		default:
			img->setFormat(GL_RGBA8);
			img->setInternalFormat(GL_RGBA);
			break;
		}
		textureCache.set(imagePath, img);
		return img;
	}

	Font* ResourceManagerImpl::loadFont(const String& filename, const int ptSize)
	{
		std::lock_guard<std::recursive_mutex> l(fontCacheLock);

		Font* fnt = fontCache[filename];
		if (fnt) {
			return fnt;
		}

		bool isTTFInitialized = TTFInitialize();
		if (!isTTFInitialized)
			return nullptr;

		fnt = TTF_OpenFont(filename.c_str(), ptSize);
		if (fnt) {
			fontCache[filename] = fnt;
		}
		return fnt;
	}

	PaletteImage* ResourceManagerImpl::getFontStringCacheEntry(Font* font, const String& text) {
		uint32_t hash = getFontStringCacheHash(font, text);
		int32_t index = hash % fontStringCacheSize;

		FontStringCacheEntry* entry;

		std::lock_guard<std::recursive_mutex> l(fontStringCacheLock);

		for (int32_t i = 0; i < fontStringCacheSize; i++) {
			entry = &_fontStringCache[index];

			if (entry->image == nullptr)
				return nullptr;
			if (entry->font == font && entry->text.compare(text) == 0) {
				_fontStringCacheHitCount++;
				entry->lastUseTick = gCurrentDrawCount;
				return entry->image;
			}

			if (entry->lastUseTick < gCurrentDrawCount - 64) break;

			if (++index >= fontStringCacheSize) {
				index = 0;
			}
		}

		entry = &_fontStringCache[index];
		disposeFontStringCacheEntry(entry);
		_fontStringCacheMissCount++;

		return nullptr;
	}

	void ResourceManagerImpl::putFontStringCacheEntry(Font* font, const String& text, PaletteImage*& img) {
		uint32_t hash = getFontStringCacheHash(font, text);
		int32_t index = hash % fontStringCacheSize;

		std::lock_guard<std::recursive_mutex> l(fontStringCacheLock);
		_fontStringCache[index] = { img, font, text, gCurrentDrawCount }; //lastUseTick should be gCurrentDrawCount
		_fontStringCacheCount++;
	}
}