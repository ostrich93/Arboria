#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../containers/HashTable.h"

namespace Arboria {
	class Font;

	class FontManager {
	public:
		FontManager();
		~FontManager();
		Font* getFont(const char* family, const char* style, unsigned int size);
		Font* getFont(const char* font_file, unsigned int size);
	private:
		FT_Library fontLibrary;
		HashTable<size_t, Font*> fontCache;
	};
}

#endif