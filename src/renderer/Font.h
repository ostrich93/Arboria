#ifndef FONT_H
#define FONT_H

#include "../utils/Vector.h"
#include "../framework/Engine.h"
#include "../containers/HashTable.h"
#include "../framework/String.h"
#include "../utils/combine_hash.h"

namespace Arboria {
	class PaletteImage;
	class Palette;

	bool TTFInitialize();
	void TTFDispose();
	struct TTFSurface;

	struct TTFSurface {
		const void* pixels;
		int32_t w;
		int32_t h;
	};

	struct Glyph {
		uint16_t codepoint;
		unsigned int index;
		int minx;
		int maxx;
		int miny;
		int maxy;
		int yoffset;
		int advance;
		PaletteImage* img;
	};

	class Font {
	public:
		int height = 0;
		int ascent = 0;
		int descent = 0;
		int lineskip = 0;

		int face_style = 0;
		int style = 0;
		int outline = 0;

		int kerning = 0;

		int glyph_overhang = 0;
		float glyph_italics = 0;

		int underline_offset = 0;
		int underline_height = 0;

		int font_size_family = 0;
		int hinting = 0;

		HashTable<uint32_t, Glyph> glyphs;
		Palette* palette = nullptr;

		//Glyph* getGlyph(char32_t codepoint);
		PaletteImage* getString(const String& text);
		int getTextWidth(const String& text);
		int getTextHeight() const;
		int getTextHeight(const String& text);
		int getMaxHeight(const String& text);
		//int getTextHeight(const std::string& text, int maxWidth);
		//std::string getName() const;
		//Palette* getPalette() const;

		//static Font* loadFont(const std::string& fontPath, int32_t ptSize, Palette* defaultPalette);
	};

	struct FontStringCacheEntry {
		PaletteImage* image;
		Font* font;
		String text;
		uint32_t lastUseTick;
	};

	//TTF_SDLPORT
	int TTF_Init(void);
	Font* TTF_OpenFont(const char* file, int ptSize);
	void TTF_Quit(void);
}

#endif