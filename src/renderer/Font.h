#ifndef __FONT_H__
#define __FONT_H__

#include "../utils/Vector.h"
#include "FontManager.h"

namespace Arboria {
	class GlyphAtlas;

	struct Glyph {
		unsigned int codepoint;
		unsigned int width; //width of the glyph in pixels
		unsigned int height; //height of the glyph in pixels
		int top; //distance in pixels from the baseline to the top of the glyph
		int left; //distance in pixels from the origin to the left edge of the glyph
		int advanceX; //the horizontal skip after rendering the glyph (1/64 pixels)
		int bearingX;
		int bearingY;
	};

	struct FontMetrics {
		int height;
		int ptSize;
		int max_width;
		int max_height;
		int max_advance;
	};

	class Font {
		private:
			bool wasInitialized;
			FontHandle fontHandle;
			unsigned width; //texture width
			unsigned height; //texture height
			unsigned char fontSize;
			unsigned int format;
			GlyphAtlas* glyphAtlas;
			FontMetrics size;
			int length;
		public:
			Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			Font(FontHandle& fname, unsigned int _format, unsigned _width = 1024, unsigned _height = 1024);
			~Font();
			int initialize();
			Glyph* getGlyph(unsigned int _codePoint);
			inline FontHandle& getFontName() { return fontHandle; }
			inline GlyphAtlas* getGlyphAtlas() { return glyphAtlas; }
			inline void setGlyphAtlas(GlyphAtlas* _glyphAtlas) { glyphAtlas = _glyphAtlas; }
			inline FontMetrics& getSize() { return size; }
			FT_Face face;
	};
}

#endif