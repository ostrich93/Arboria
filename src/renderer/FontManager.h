#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../containers/HashTable.h"
#include "../framework/String.h"

namespace Arboria {
	class Font;

	struct FontHandle {
		String fontName;
		String fontStyle;
		unsigned char fontSize;
		FontHandle(const char* _fontName, const char* _fontStyle = "", unsigned char _fontSize = 24);
		FontHandle(const char* _fontName, unsigned char _fontSize);
	};

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
			HashTable<FontHandle, Font*> fontCache;
	};
}

#endif