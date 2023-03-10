#include "GlyphAtlas.h"
#include "../Heap.h"
#include <SDL_opengl.h>
#include "FontManager.h"
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include "../utils/math.h"

namespace Arboria {
	GlyphAtlas::GlyphAtlas(Font& font, unsigned int _width, unsigned int _height, int capacity) : parentFont(&font), width(_width), height(_height), glyphCapacity(capacity), isDirty(false), dirtyArea{ _width, _height, 0, 0 }, textureId(0) {
		dataBuffer = (unsigned char*)Mem_ClearedAlloc(_width * _height * sizeof(unsigned char));
		pack_nodes = (stbrp_node*)Mem_ClearedAlloc(sizeof(stbrp_node) * _width);
		stbrp_init_target(&pack_context, _width, _height, pack_nodes, _width);
		glGenTextures(1, &textureId);
	}

	GlyphAtlas::~GlyphAtlas() {
		if (textureId) glDeleteTextures(1, &textureId);
		Mem_Free(pack_nodes);
	}

	void GlyphAtlas::initialize() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool GlyphAtlas::loadGlyphs()
	{
		FT_Face face = hb_ft_font_get_face(parentFont->hbFont);

		for (int c = 32; c < 128; c++) {
			Glyph glyph;
			unsigned char* glyph_data = loadGlyph(face, c, glyph);
			addEntry(c, glyph, glyph_data);
		}
		return false;
	}

	unsigned char* GlyphAtlas::loadGlyph(FT_Face& _face, unsigned short codepoint, Glyph& glyph)
	{
		if (FT_Load_Glyph(_face, codepoint, FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING | FT_LOAD_RENDER)) {
			return NULL;
		}
		glyph.codepoint = codepoint;
		glyph.left = _face->glyph->bitmap_left;
		glyph.top = _face->glyph->bitmap_top;
		glyph.width = _face->glyph->bitmap.width;
		glyph.height = _face->glyph->bitmap.rows;
		glyph.advanceX = _face->glyph->advance.x >> 6;

		unsigned char* glyph_data = (unsigned char*)Mem_Alloc(glyph.width * glyph.height * sizeof(unsigned char));
		for (size_t i = 0; i < glyph.height; i++) {
			memcpy(&glyph_data[i * glyph.width],
				_face->glyph->bitmap.buffer + (glyph.height - i - 1) * glyph.width,
				glyph.width * sizeof(unsigned char));
		}
		return glyph_data;
	}

	bool GlyphAtlas::addEntry(unsigned short codepoint, Glyph& glyph, const unsigned char* img)
	{
		stbrp_rect rect;
		rect.w = glyph.width + 1;
		rect.h = glyph.height + 1;

		bool ok = stbrp_pack_rects(&pack_context, &rect, 1);
		if (!ok) {
			fprintf(stderr, "STBRP Error: Could not pack the rectangle.\n");
			return ok;
		}
		if (rect.was_packed) {
			glyph.u0 = rect.x / width;
			glyph.v0 = rect.y / height;
			glyph.u1 = (rect.x + glyph.width) / width;
			glyph.v1 = (rect.y + glyph.height) / height;

			for (unsigned int i = 0; i < glyph.height; i++) {
				memcpy(this->dataBuffer + ((rect.y + i) * glyph.width + rect.x),
					img + (i * glyph.width),
					glyph.width * sizeof(unsigned char));
			}
			updateDirtyArea(rect.x, rect.y, glyph.width, glyph.height);
			glyphs.set(codepoint - 32, glyph);
			return true;
		}
		return false;
	}

	void GlyphAtlas::updateDirtyArea(int x, int y, int width, int height) {
		isDirty = true;
		dirtyArea.x1 = Math::iMin(dirtyArea.x1, x);
		dirtyArea.y1 = Math::iMin(dirtyArea.y1, y);
		dirtyArea.x2 = Math::iMin(dirtyArea.x2, x + width);
		dirtyArea.y2 = Math::iMin(dirtyArea.y2, y + height);
	}

	void GlyphAtlas::bind(int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (isDirty) {
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, dirtyArea.y1, width, dirtyArea.y2 - dirtyArea.y1, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer + width * dirtyArea.y1);
			isDirty = false;
			dirtyArea = { width, height, 0, 0 };
		}
	}

	Glyph* GlyphAtlas::getGlyph(unsigned short codepoint) {
		if (codepoint < 32) return NULL;

		if (!glyphs.contains(codepoint - 32)) {
			loadGlyphs();
		}

		return &(glyphs.get(codepoint - 32));
	}

	Glyph* Font::getGlyph(unsigned short _codePoint)
	{
		if (glyphAtlas == NULL) {
			glyphAtlas = &GlyphAtlas{ *this };
			glyphAtlas->initialize();
		}
		if (glyphAtlas != NULL) {
			return glyphAtlas->getGlyph(_codePoint);
		}
		return NULL;
	}
}