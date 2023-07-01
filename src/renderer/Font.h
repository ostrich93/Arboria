#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include "../utils/Vector.h"
#include "../framework/Engine.h"
#include "../containers/HashTable.h"
#include "../framework/String.h"
#include "../utils/combine_hash.h"

namespace Arboria {
	class Texture;
	class TextureAtlas;

	struct Glyph {
		Vector2<unsigned int> size;
		int top; //distance in pixels from the baseline to the top of the glyph
		int left; //distance in pixels from the origin to the left edge of the glyph
		Vector2<int> bearing;
		Vector2<int> advance;
		Vector2<int> kerning;
	};

	struct GlyphEntry {
		Glyph glyph;
		float u0;
		float v0;
		float u1;
		float v1;
	};

	struct FontMetrics {
		int ptSize;
		int max_width;
		int max_height;
		Vector2<int> max_advance;
	};

	struct FontHandle {
		String fontName;
		String fontStyle;
		unsigned char fontSize;
		FontHandle() : fontName(""), fontStyle(""), fontSize(24) {}
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
		return combineHash(hash2, (static_cast<size_t>(fontHandle.fontSize) + 119));
	}

	class Font {
		private:
			friend class FontManager;
			bool wasInitialized;
			FontHandle fontHandle;
			unsigned width; //texture width
			unsigned height; //texture height
			unsigned char fontSize;
			unsigned int format;
			TextureAtlas* glyphAtlas;
			GlyphEntry glyphInfo[256];
			FontMetrics size;
			int length;
		public:
			Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			Font(FontHandle& fname, unsigned int _format, unsigned _width = 1024, unsigned _height = 1024);
			~Font();
			int initialize();
			GlyphEntry* getGlyphEntry(unsigned int codePoint);
			List<int> getGlyphs(const String& string);
			List<int> getGlyphs(const char* string);
			Glyph* getGlyph(unsigned int _codePoint);
			inline FontHandle& getFontName() { return fontHandle; }
			inline TextureAtlas* getGlyphAtlas() { return glyphAtlas; }
			inline void setGlyphAtlas(TextureAtlas* _glyphAtlas) { glyphAtlas = _glyphAtlas; }
			inline FontMetrics& getSize() { return size; }
	};

	class FontManager {
		public:
			FontManager();
			~FontManager();
			FT_Library getFontLibrary() { return fontLibrary; }
			const String getFontFilename(const char* family, const char* style);
			int loadFont(const char* family, const char* style, unsigned int size);
			int loadFont(FontHandle& fontHandle);
			Font* getFont(const char* family, const char* style, unsigned int size);
			Font* getFont(FontHandle& fontHandle);
			inline FT_Face getCurrentFace() { return currentFace; }
			inline void setCurrentFace(FT_Face& _face) { currentFace = _face; }
		private:
			FT_Library fontLibrary;
			FT_Face currentFace;
			HashTable<size_t, Font*> fontCache;
	};
}

#endif