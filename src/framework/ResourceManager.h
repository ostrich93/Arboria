#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../containers/HashTable.h"

namespace Arboria {
	class Font;
	class Palette;
	class PaletteImage;
	class Image;
	class ResourceManager {
		public:
			static ResourceManager* createResourceManager();
			virtual ~ResourceManager() = default;
			ResourceManager() {}

			virtual Image* loadTexture(const uint32_t resourceId, bool isPaletted = false) = 0;
			virtual Font* loadFont(const String& filename, const int ptSize) = 0;
			virtual Palette* loadPalette(const uint32_t resourceId) = 0;
			virtual PaletteImage* getFontStringCacheEntry(Font* font, const String& text) = 0;
			virtual void putFontStringCacheEntry(Font* font, const String& text, PaletteImage*& img) = 0;
	};
}

#endif