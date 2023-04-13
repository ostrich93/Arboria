#include "GlyphAtlas.h"
#include "../Heap.h"
#include "FontManager.h"

namespace Arboria {
	GlyphAtlas::GlyphAtlas(Font& font, unsigned int _width, unsigned int _height) : parentFont(&font), width(_width), height(_height), textureId(0) {
		glGenTextures(1, &textureId);
	}

	GlyphAtlas::GlyphAtlas(unsigned int _width) : parentFont(NULL), width(_width), height(_width), textureId(0), wasInitialized(false) {
		glGenTextures(1, &textureId);
	}

	GlyphAtlas::~GlyphAtlas() {
		if (textureId) glDeleteTextures(1, &textureId);
	}

	void GlyphAtlas::initialize() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);
		//glBindTexture(GL_TEXTURE_2D, 0);

		wasInitialized = true;
	}

	Glyph* GlyphAtlas::loadGlyphs(FT_Face& face, FontMetrics* size, int length) {
		Glyph* glyphMetrics = (Glyph*)Mem_Alloc(sizeof(*glyphs) * face->num_glyphs);
		if (!glyphMetrics) return NULL;

		FT_UInt index;
		int xPos = 0;
		int yPos = 0;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		try {
			for (FT_ULong charcode = FT_Get_First_Char(face, &index); index != 0; charcode = FT_Get_Next_Char(face, charcode, &index)) {
				if (xPos < (length - 1)) {
					xPos++;
				}
				else {
					xPos = 0;
					yPos++;
				}

				FT_Load_Char(face, charcode, FT_LOAD_RENDER);
				FT_Bitmap* bitmap = &face->glyph->bitmap;
				if (bitmap->pixel_mode != ft_pixel_mode_grays) {
					break;
				}

				setGlyphMetrics(glyphMetrics, index, xPos, size, yPos, face);

				int xReal = xPos * size->ptSize;
				int yReal = yPos * size->ptSize;
				for (int y = 0; y < bitmap->rows; y++) {
					for (int x = 0; x < bitmap->width; x++) {
						int index = (yReal + y) * width + xReal + x;
						uint32_t* pixel = &((uint32_t*)dataBuffer)[index];
						uint8_t alpha = bitmap->buffer[y * bitmap->pitch + x];
						*pixel = (255 << 24) + (255 << 16) + (255 << 8) + alpha;
					}
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, xReal, yReal, bitmap->width, bitmap->rows, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->buffer);

			}
		}
		catch (std::exception _exception) {
			printf("GlyphAtlas::loadGlyphs: An exception was caught when attempting to load the glyph data for the font %s.\n%s", parentFont->getFontName().fontName.c_str(), _exception.what());
			Mem_Free(glyphMetrics);
			glyphMetrics = NULL;
		}

		return glyphMetrics;
	}

	void GlyphAtlas::setGlyphMetrics(Glyph* glyphMetrics, const FT_UInt& index, int xPos, FontMetrics* size, int yPos, FT_Face& face)
	{
		glyphMetrics[index].codepoint = face->glyph->glyph_index; //maybe glyphIndex?
		glyphMetrics[index].left = xPos * size->ptSize;
		glyphMetrics[index].top = yPos * size->ptSize;
		glyphMetrics[index].height = face->glyph->metrics.height >> 6;
		glyphMetrics[index].width = face->glyph->metrics.width >> 6;
		glyphMetrics[index].bearingX = face->glyph->metrics.horiBearingX >> 6;
		glyphMetrics[index].bearingY = face->glyph->metrics.horiBearingY >> 6;
		glyphMetrics[index].advanceX = face->glyph->metrics.horiAdvance >> 6;
		if (size->max_width < glyphMetrics[index].width) {
			size->max_width = glyphMetrics[index].width;
		}
		if (size->max_height < glyphMetrics[index].height) {
			size->max_height = glyphMetrics[index].height;
		}
		if (size->max_advance < glyphMetrics[index].advanceX) {
			size->max_advance = glyphMetrics[index].advanceX;
		}
	}

	Font* GlyphAtlas::getFont()
	{
		return parentFont;
	}

	void GlyphAtlas::setFont(Font* _font) {
		parentFont = _font;
	}

	Glyph* GlyphAtlas::getGlyph(unsigned int codepoint) {
		FT_UInt glyphIndex = FT_Get_Char_Index(parentFont->face, codepoint);
		if (glyphIndex != 0) return &glyphs[glyphIndex];

		return NULL;
	}

	const Glyph* GlyphAtlas::getGlyph(unsigned int codepoint) const {
		FT_UInt glyphIndex = FT_Get_Char_Index(parentFont->face, codepoint);
		if (glyphIndex != 0) return &glyphs[glyphIndex];

		return NULL;
	}

	int generateGlyphs(Font* font, GlyphAtlas* outAtlas, unsigned int length)
	{
		unsigned char* dBuffer = (unsigned char*)Mem_ClearedAlloc(outAtlas->getWidth() * outAtlas->getHeight() * sizeof(uint32_t));
		if (!dBuffer) {
			return 1;
		}
		outAtlas->setDataBuffer(dBuffer);
		FT_Face face = font->face;
		FontMetrics size = font->getSize();

		Glyph* glyphData = outAtlas->loadGlyphs(face, &size, length);
		if (!glyphData) {
			Mem_Free(outAtlas->getDataBuffer());
			return 1;
		}

		return 0;
	}
}