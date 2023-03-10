#include "Font.h"
#include "../globals.h"
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace Arboria {
	Font::Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontName{ name, "", ptSz }, format(_format), fontSize(ptSz), width(_width), height(_height), hbFont(NULL) {}

	Font::Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontName{ name, family, ptSz }, format(_format), fontSize(ptSz), width(_width), height(_height), hbFont(NULL) {}

	Font::~Font()
	{
		FT_Done_Face(hb_ft_font_get_face(hbFont));
	}

	int Font::initialize(unsigned int _capacity) {
		if (wasInitialized) return 1;

		FT_Face face;

		if (FT_New_Face(engine->getFontManager()->getFontLibrary(), engine->getFontManager()->getFontFilename(fontName.fontName, fontName.fontFamily), 0, &face)) {
			fprintf(stderr, "FreeType Error: Could not open font %s\n", fontName.fontName);
			return 0;
		}

		if (FT_Set_Pixel_Sizes(face, width, height)) {
			fprintf(stderr, "FreeType Error: Could not set sizes\n");
			return 0;
		}

		glyphCapacity = _capacity;
		hbFont = hb_ft_font_create_referenced(face);

		wasInitialized = true;
		return 1;
	}
}