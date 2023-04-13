#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../framework/Engine.h"
#include "../containers/HashTable.h"
#include "../framework/String.h"
#include "../utils/combine_hash.h"

namespace Arboria {
	class Font;
	struct FontHandle {
		String fontName;
		String fontStyle;
		unsigned char fontSize;
		FontHandle(const char* _fontName, const char* _fontStyle = "", unsigned char _fontSize = 24);
		FontHandle(const char* _fontName, unsigned char _fontSize);
		bool operator==(const FontHandle& other) const {
			return (fontName == other.fontName) && (fontStyle == other.fontStyle) && (fontSize == other.fontSize);
		}
		static size_t getHash(const FontHandle& fontHandle);
	};

	inline size_t FontHandle::getHash(const FontHandle& fontHandle) {
		size_t hash1 = String::getHash(fontHandle.fontName);
		size_t hash2 = combineHash(hash1, String::getHash(fontHandle.fontStyle));
		return combineHash(hash2, (fontHandle.fontSize + 119));
	}

	class FontManager {
		public:
			FontManager();
			~FontManager();
			FT_Library getFontLibrary() { return fontLibrary; }
			const char* getFontFilename(const char* family, const char* style) const;
			int loadFont(const char* family, const char* style, unsigned int size);
			int loadFont(FontHandle& fontHandle);
			Font* getFont(const char* family, const char* style, unsigned int size);
			Font* getFont(FontHandle& fontHandle);
		private:
			FT_Library fontLibrary;
			HashTable<size_t, Font*> fontCache;
	};
}

#endif