#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../containers/HashTable.h"

struct hb_font_t;

namespace Arboria {
	class Font;

	struct FontName {
		const char* fontName;
		const char* fontFamily;
		unsigned char fontSize;
		FontName(const char* _fontName, const char* _fontFamily = "", unsigned char _fontSize = 24);
		FontName(const char* _fontName, unsigned char _fontSize);
	};

	class FontManager {
		public:
			FontManager();
			~FontManager();
			FT_Library getFontLibrary() { return fontLibrary; }
			const char* getFontFilename(const char* family, const char* style) const;
			int loadFont(const char* family, const char* style, unsigned int size);
			Font* getFont(const char* family, const char* style, unsigned int size);
			Font* getFont(const char* font_file, unsigned int size);
			Font* getFont(const FontName& fontName);
		private:
			FT_Library fontLibrary;
			HashTable<FontName, Font*> fontCache;
	};
}

#endif