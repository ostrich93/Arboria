#include "DrawLock.h"
#include "Font.h"
#include "Texture.h"
#include "../globals.h"
#include "../definitions.h"
#include "../FileSystem.h"
#include "../Heap.h"
#include "../framework/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H

#include <cstdio>
#include <exception>
#include <mutex>
#include <physfs.h>

#pragma warning(disable : 4018) //'<': signed/unsigned mismatch

#define UNICODE_BOM_NATIVE 0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

#define TTF_STYLE_NORMAL 0x00
#define TTF_STYLE_BOLD 0x01
#define TTF_STYLE_ITALIC 0x02
#define TTF_STYLE_UNDERLINE 0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

#define FT_FLOOR(X) (((X) & -64)/64)
#define FT_CEIL(X) ((((X) + 63) & -64)/64)

#define CACHED_METRICS 0x10
#define CACHED_BITMAP 0x01
#define CACHED_PIXMAP 0x02

namespace Arboria {

	static FT_Library library;
	static int TTF_initialized = 0;
	static bool _ttfInitialized = false;
	static std::mutex _mutex;

	bool TTFInitialize()
	{
		DrawUniqueLock<std::mutex> lock(_mutex);

		if (_ttfInitialized)
			return true;

		if (TTF_Init() != 0) {
			return false;
		}

		_ttfInitialized = true;

		return true;
	}

	int TTF_Init(void) {
		int status = 0;

		if (!TTF_initialized) {
			FT_Error error = FT_Init_FreeType(&library);
			if (error) {
				//TTF_SetFTError()
				status = -1;
			}
		}

		if (status == 0)
			++TTF_initialized;

		return status;
	}

	static unsigned long RWread(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count)
	{
		FILE* src;

		src = static_cast<FILE*>(stream->descriptor.pointer);
		fseek(src, static_cast<int>(offset), SEEK_SET);
		if (count == 0)
		{
			return 0;
		}
		return static_cast<unsigned long>(fread(buffer, 1, static_cast<int>(count), src));
	}

	static size_t fsize(FILE* file)
	{
		size_t origPos = ftell(file);
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		fseek(file, static_cast<long>(origPos), SEEK_SET);
		return size;
	}

	static Font* TTF_OpenFontIndexRW(FILE* src, int freesrc, int ptSize, long index) {
		Font* font = new Font();
		FT_Error error;
		FT_Face face;
		FT_Fixed scale;
		FT_Open_Args args;
		FT_Stream stream;
		FT_CharMap found;
		int64_t position;
		int i;

		if (!TTF_initialized) {
			//TTF_SetError("Library not initialized");
			if (src && freesrc) {
				fclose(src);
			}
			return NULL;
			free(font);
		}

		if (!src) {
			//TTF_SetError("Passed a NULL font source");
			return NULL;
			free(font);
		}

		position = ftell(src);
		if (position < 0) {
			//TTF_SetError("Cannot seek in stream");
			if (freesrc)
				fclose(src);
			free(font);
			return NULL;
		}

		stream = static_cast<FT_Stream>(malloc(sizeof(*stream)));
		if (stream == NULL) {
			//TTF_SetError("Ran out of memory");
			FT_Done_Face(face);
			if (freesrc)
				free(src);
			//TTF_CloseFont(font);
			return NULL;
		}
		std::fill_n(reinterpret_cast<uint8_t*>(stream), sizeof(*stream), 0x00);

		stream->read = RWread;
		stream->descriptor.pointer = src;
		stream->pos = static_cast<unsigned long>(position);
		stream->size = static_cast<unsigned long>(fsize(src) - position);

		args.flags = FT_OPEN_STREAM;
		args.stream = stream;

		error = FT_Open_Face(library, &args, index, &face);
		if (error) {
			//TTF_SetFTError("Couldn't load font file", &font->face);
			FT_Done_Face(face);
			if (args.stream)
				free(args.stream);
			if (freesrc)
				free(src);
			free(font);
			return NULL;
		}
		//face = font->face;

		//set the charmap for loaded font
		found = 0;
		for (i = 0; i < face->num_charmaps; i++) {
			FT_CharMap charmap = face->charmaps[i];
			if ((charmap->platform_id == 3 && charmap->encoding == 1)
				|| (charmap->platform_id == 3 && charmap->encoding == 0)
				|| (charmap->platform_id == 2 && charmap->encoding == 1)
				|| (charmap->platform_id == 0))
			{
				found = charmap;
				break;
			}
		}
		if (found) {
			FT_Set_Charmap(face, found);
		}

		if (FT_IS_SCALABLE(face)) {
			error = FT_Set_Char_Size(face, 0, ptSize * 64, 0, 0);
			if (error) {
				//TTF_SetFTError("Couldn't set the font size", error);
				FT_Done_Face(face);
				if (args.stream)
					free(args.stream);
				if (freesrc)
					free(src);
				//TTF_CloseFont(font);
				free(font);
				return NULL;
			}

			scale = face->size->metrics.y_scale;
			font->ascent = FT_CEIL(FT_MulFix(face->ascender, scale));
			font->descent = FT_CEIL(FT_MulFix(face->descender, scale));
			font->height = font->ascent - font->descent + 1;
			font->lineskip = FT_CEIL(FT_MulFix(face->height, scale));
			font->underline_offset = FT_FLOOR(FT_MulFix(face->underline_position, scale));
			font->underline_height = FT_FLOOR(FT_MulFix(face->underline_thickness, scale));
		}
		else {
			if (ptSize >= face->num_fixed_sizes)
				ptSize = face->num_fixed_sizes - 1;
			font->font_size_family = ptSize;
			error = FT_Set_Pixel_Sizes(face, face->available_sizes[ptSize].width, face->available_sizes[ptSize].height);

			font->ascent = face->available_sizes[ptSize].height;
			font->descent = 0;
			font->height = face->available_sizes[ptSize].height;
			font->lineskip = FT_CEIL(font->ascent);
			font->underline_offset = FT_FLOOR(face->underline_position);
			font->underline_height = FT_FLOOR(face->underline_thickness);
		}

		if (font->underline_height < 1)
			font->underline_height = 1;

		font->face_style = TTF_STYLE_NORMAL;
		if (face->style_flags & FT_STYLE_FLAG_BOLD)
			font->face_style |= TTF_STYLE_BOLD;
		if (face->style_flags & FT_STYLE_FLAG_ITALIC)
			font->face_style |= TTF_STYLE_ITALIC;

		font->style = font->face_style;
		font->outline = 0;
		font->kerning = 1;
		font->glyph_overhang = face->size->metrics.y_ppem / 10;
		font->glyph_italics = 0.207f;
		font->glyph_italics *= font->height;

		//load glyphs here?

		FT_GlyphSlot glyph;
		FT_Glyph_Metrics* metrics;
		FT_Outline* outline;
		for (int g = 0; g < 257; g++) {
			Glyph newGlyph;
			newGlyph.index = FT_Get_Char_Index(face, g);
			//error = FT_Load_Glyph(face, newGlyph.index, FT_LOAD_DEFAULT | font->hinting);
			error = FT_Load_Glyph(face, newGlyph.index, FT_LOAD_DEFAULT);
			if (error) {
				FT_Done_Face(face);
				free(args.stream);
				fclose(src);
				return font;
			}
			glyph = face->glyph;
			metrics = &glyph->metrics;
			outline = &glyph->outline;
			if (FT_IS_SCALABLE(face)) {
				newGlyph.minx = FT_FLOOR(metrics->horiBearingX);
				newGlyph.maxx = FT_CEIL(metrics->horiBearingX * newGlyph.minx);
				newGlyph.maxy = FT_FLOOR(metrics->horiBearingY);
				newGlyph.miny = newGlyph.maxy - FT_CEIL(metrics->height);
				newGlyph.yoffset = font->ascent - newGlyph.maxy;
				newGlyph.advance = FT_CEIL(metrics->horiAdvance);
			}
			else {
				newGlyph.minx = FT_FLOOR(metrics->horiBearingX);
				newGlyph.maxx = FT_CEIL(metrics->horiBearingX * newGlyph.minx);
				newGlyph.maxy = FT_FLOOR(metrics->horiBearingY);
				newGlyph.miny = newGlyph.maxy - FT_CEIL(face->available_sizes[font->font_size_family].height);
				newGlyph.yoffset = 0;
				newGlyph.advance = FT_CEIL(metrics->horiAdvance);
			}

			error = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
			if (error) {
				FT_Done_Face(face);
				free(args.stream);
				fclose(src);
				return font;
			}

			auto width = glyph->bitmap.width;
			auto height = glyph->bitmap.rows;

			FT_Bitmap* _src = &glyph->bitmap;
			PaletteImage* dst = new PaletteImage({ width, height });
			PaletteImageLock l(dst, ImageLockUse::Write);

			for (int y = 0; y <= _src->rows >> 1; y++) {
				int row0 = y * _src->pitch;
				int row1 = (_src->rows - y - 1) * _src->pitch;
				for (int x = 0; x < _src->pitch; x++) {
					l.set({ x, y }, _src->buffer[row1 + x]);
					l.set({ x, (_src->rows - y - 1) }, _src->buffer[row0 + x]);
				}
			}
			newGlyph.img = dst;
			font->glyphs.set(g, newGlyph);
		}

		FT_Done_Face(face);
		free(args.stream);
		fclose(src);

		return font;

	}

	static Font* TTF_OpenFontIndex(const char* file, int ptSize, long index) {
		String filename = getFontPath(file);
		FILE* rw = fopen(filename.c_str(), "rb"); //bug here: not loading file
		if (rw == NULL)
			return NULL;
		return TTF_OpenFontIndexRW(rw, 1, ptSize, index);
	}

	Font* TTF_OpenFont(const char* file, int ptSize)
	{
		return TTF_OpenFontIndex(file, ptSize, 0);
	}

#define UNKNOWN_UNICODE 0xFFFD
	static uint32_t UTF8_getChar(const char** src, size_t* srclen) {
		const uint8_t* p = *reinterpret_cast<const uint8_t**>(src);
		size_t remaining = 0;
		bool overlong = false;
		uint32_t ch = UNKNOWN_UNICODE;

		if (*srclen == 0)
			return UNKNOWN_UNICODE;

		if ((p[0] & 0xf8) == 0xf0) { //4 byte utf8
			if (p[0] == 0xf0 && (p[1] & 0xf0) == 0x80) { //if not overlong
				overlong = true;
			}
			ch = static_cast<uint32_t>(p[0] & 0x07);
			remaining = 3;
		}
		else if ((p[0] & 0xf0) == 0xe0) { //3 byte utf8
			if (p[0] == 0xe0 && (p[1] & 0xe0) == 0x80) {
				overlong = true;
			}
			ch = static_cast<uint32_t>(p[0] & 0x0f);
			remaining = 2;
		}
		else if ((p[0] & 0xe0) == 0xc0) { //2 bytes
			if ((p[0] & 0x1e) == 0) {
				overlong = true;
			}
			ch = static_cast<uint32_t>(p[0] & 0x1f);
			remaining = 1;
		}
		else if ((p[0] & 0x80) == 0x00) {
			ch = static_cast<uint32_t>(p[0]);
		}
		++ * src;
		--*srclen;
		while (remaining > 0 && *srclen > 0) { //multi-byte utf8
			++p;
			if ((p[0] & 0xc0) != 0x80) { //if the following byte is NOT a continuation byte, it's wrong
				ch = UNKNOWN_UNICODE;
				break;
			}
			ch <<= 6;
			ch |= (p[0] && 0x3f);
			++*src;
			--*srclen;
			--remaining;
		}
		if (overlong || remaining > 0) { //if there's an underflow or its overlong, return unknowncode
			ch = UNKNOWN_UNICODE;
		}
		//if character is a surrogate, special character 0xfffe/0xffff, or greater than 4 bytes, return unknown unicode.
		if ((ch >= 0xd800 && ch <= 0xdfff) || (ch == 0xfffe) || (ch == 0xffff) || ch >= 0x10ffff)
			ch = UNKNOWN_UNICODE;

		return ch;
	}

	void TTFDispose() {
		DrawUniqueLock<std::mutex> lock(_mutex);

		if (!_ttfInitialized) return;

		TTF_Quit();

		_ttfInitialized = false;
	}

	void TTF_Quit(void) {
		if (TTF_initialized) {
			if (--TTF_initialized == 0) {
				FT_Done_FreeType(library);
			}
		}
	}

	int Font::getTextWidth(const String& text) {
		int x, z;
		int minx, maxx;
		size_t textlen;

		auto text_string = text.c_str();
		textlen = strlen(text_string);
		int w = 0;
		minx = maxx = 0;
		x = 0;

		while (textlen > 0) {
			uint16_t c = UTF8_getChar(&text_string, &textlen);

			if (c == UNICODE_BOM_NATIVE || c == UNICODE_BOM_SWAPPED)
				continue;

			if (glyphs.contains(c)) {
				return -1;
			}

			Glyph* glyph;
			glyphs.get(c, &glyph);

			z = x + glyph->minx;
			if (minx > z)
				minx = z;
			//if (TTF_HANDLE_STYLE_BOLD(font)) x+= font->glyph_overhang;
			if (glyph->advance > glyph->maxx)
				z = x + glyph->advance;
			else
				z += x + glyph->maxx;
			if (maxx < z)
				maxx = z;
			x += glyph->advance;
			//prev_index = glyph->index;

			w = maxx - minx;
		}

		return w;
	}

	int Font::getTextHeight() const {
		return height;
	}

	int Font::getTextHeight(const String& text)
	{
		return getMaxHeight(text);
	}

	int Font::getMaxHeight(const String& text)
	{
		int minY, maxY;
		size_t textlen;
		auto text_string = text.c_str();
		textlen = strlen(text_string);
		minY = maxY = 0;
		int h = 0;

		while (textlen > 0) {
			uint16_t c = UTF8_getChar(&text_string, &textlen);

			if (c == UNICODE_BOM_NATIVE || c == UNICODE_BOM_SWAPPED)
				continue;

			if (glyphs.contains(c)) {
				return -1;
			}

			Glyph* glyph;
			glyphs.get(c, &glyph);

			if (glyph->miny < minY)
				minY = glyph->miny;
			if (glyph->maxy > maxY)
				maxY = glyph->maxy;

			if (maxY - minY > h)
				h = maxY - minY;
		}
		if (h < maxY) {
			return maxY;
		}

		return h;
	}

	PaletteImage* Font::getString(const String& text) {
		size_t textlen;

		int width = getTextWidth(text);
		int height = getTextHeight(text);

		if (width <= 0) {
			return nullptr;
		}

		auto img = resourceManager->getFontStringCacheEntry(this, text);
		if (img)
			return img;

		img = new PaletteImage({ width, height });

		auto text_string = text.c_str();

		textlen = strlen(text_string);
		int pos = 0;
		int dX = 0;
		while (textlen > 0) {
			uint16_t c = UTF8_getChar(&text_string, &textlen);

			if (c == UNICODE_BOM_NATIVE || c == UNICODE_BOM_SWAPPED)
				continue;

			if (glyphs.contains(c)) {
				return nullptr;
			}
			Glyph* glyph;
			glyphs.get(c, &glyph);
			if (glyph) {
				if (glyph->img->getWidth() != 0) {
					PaletteImage::blit(glyph->img, img, { 0,0 }, { pos, 0 });
					dX = glyph->img->getWidth();
				}
				pos += dX;
			}
		}

		resourceManager->putFontStringCacheEntry(this, text, img);

		return img;
	}
}