#ifndef __FONT_H__
#define __FONT_H__

#include "FontManager.h"

namespace Arboria {
	class GlyphAtlas;

	struct Glyph {
		unsigned short codepoint;
		unsigned short glyphIndex;
		unsigned char width; //width of the glyph in pixels
		unsigned char height; //height of the glyph in pixels
		char top; //distance in pixels from the baseline to the top of the glyph
		char left; //distance in pixels from the origin to the left edge of the glyph
		float advanceX; //the horizontal skip after rendering the glyph (1/64 pixels)
		float u0; //x-offset (in pixels) in the image where the glyph starts
		float v0; //y-offset (in pixels) in the image where the glyph starts
		float u1;
		float v1;
	};

	class Font {
		private:
			bool wasInitialized;
			FontName fontName;
			unsigned width; //texture width
			unsigned height; //texture height
			unsigned char fontSize;
			unsigned int format;
			int glyphCapacity;
			GlyphAtlas* glyphAtlas;
		public:
			Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width = 1024, unsigned _height = 1024);
			~Font();
			int initialize(unsigned int _capacity = 128);
			Glyph* getGlyph(unsigned short _codePoint);
			inline FontName& getFontName() { return fontName; }
			inline GlyphAtlas* getGlyphAtlas() { return glyphAtlas; }
			hb_font_t* hbFont;
	};
}

#endif