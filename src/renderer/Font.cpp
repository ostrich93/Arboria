#include "Font.h"
#include "GlyphAtlas.h"
#include "../globals.h"
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace Arboria {
	Font::Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle{ name, "", ptSz }, format(_format), fontSize(ptSz), width(_width), height(_height), face(NULL) {
		size.ptSize = ptSz;
	}

	Font::Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle{ name, family, ptSz }, format(_format), fontSize(ptSz), width(_width), height(_height), face(NULL) {
		size.ptSize = ptSz;
	}

	Font::Font(FontHandle& fname, unsigned int _format, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle(fname), format(_format), fontSize(fname.fontSize), width(_width), height(_height), face(NULL) {
		size.ptSize = fname.fontSize;
	}

	Font::~Font()
	{
		FT_Done_Face(face);
	}

	int Font::initialize() {
		if (wasInitialized) return 1;

		if (FT_New_Face(fontManager->getFontLibrary(),fontManager->getFontFilename(fontHandle.fontName, fontHandle.fontStyle), 0, &face)) {
			fprintf(stderr, "FreeType Error: Could not open font %s\n", fontHandle.fontName);
			return 0;
		}

		if (FT_Set_Pixel_Sizes(face, size.ptSize, size.ptSize)) {
			FT_Done_Face(face);
			fprintf(stderr, "FreeType Error: Could not set sizes\n");
			return 0;
		}

		unsigned int _length = (unsigned int)Math::ceil(Math::sqrt(face->num_glyphs));
		size.height = face->size->metrics.height >> 6;
		length = _length;
		unsigned int fwidth = length * size.ptSize;
		//create font glyph atlas
		glyphAtlas = &GlyphAtlas{ fwidth };
		glyphAtlas->initialize();
		int result = generateGlyphs(this, glyphAtlas, length);
		if (result) {
			glyphAtlas->~GlyphAtlas();
			FT_Done_Face(face);
			return 0;
		}
		glyphAtlas->setFont(this);

		wasInitialized = true;
		return 1;
	}
}