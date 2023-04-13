#include "FontManager.h"
#include "GlyphAtlas.h"
#include "../definitions.h"
#include "../FileSystem.h"
#include "../Heap.h"
#include <cstdio>
#include <exception>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include <physfs.h>

namespace Arboria {

	FontManager::FontManager() {
		if (FT_Init_FreeType(&fontLibrary) != 0) {
			const char* errMessage = "FreeType Error: Could not init FreeType Library\n";
			fprintf(stderr, errMessage);
			throw new std::exception(errMessage);
		}
	}

	FontManager::~FontManager() {
		FT_Done_FreeType(fontLibrary);
	}

	const char* FontManager::getFontFilename(const char* family, const char* style) const
	{
		List<String> pathTokens;
		if (!style || strlen(style) == 0) {
			pathTokens = { PHYSFS_getBaseDir(), "assets", DIR_SEPARATOR_STR, "fonts", DIR_SEPARATOR_STR, family, ".ttf"};
		}
		else {
			pathTokens = { PHYSFS_getBaseDir(), "assets", DIR_SEPARATOR_STR, "fonts", DIR_SEPARATOR_STR, family, "-", style, ".ttf" };
		}

		String pathName = String::join(pathTokens, "");
		return pathName.c_str();
	}

	int FontManager::loadFont(const char* family, const char* style, unsigned int size)
	{
		Font font { family, style, 0, static_cast<unsigned char>(size) };
		Font* fl = (Font*)Mem_ClearedAlloc(sizeof(*fl));
		
		int result = font.initialize();
		if (!result) {
			fprintf(stderr, "FontManager::loadFont failed to load font");
			font.~Font();
		}
		else {
			fontCache.set(FontHandle::getHash(font.getFontName()), &font);
			sizeof(font.getFontName());
			result = 1;
		}

		return result;
	}

	int FontManager::loadFont(FontHandle& fontHandle) {
		Font font = Font{ fontHandle, 0 };

		int result = font.initialize();
		if (!result) {
			fprintf(stderr, "FontManager::loadFont failed to load font");
			font.~Font();
		}
		else {
			fontCache.set(FontHandle::getHash(font.getFontName()), &font);
			result = 1;
		}

		return result;
	}

	Font* FontManager::getFont(const char* family, const char* style, unsigned int size)
	{
		FontHandle fnm{ family, style, static_cast<unsigned char>(size) };
		size_t fontHash = FontHandle::getHash(fnm);
		if (!fontCache.contains(fontHash)) {
			int rslt = loadFont(fnm);
			if (!rslt) {
				return NULL;
			}
		}
		return fontCache.get(fontHash);
	}

	Font* FontManager::getFont(FontHandle& fontHandle) {
		size_t fontHash = FontHandle::getHash(fontHandle);
		if (!fontCache.contains(fontHash)) {
			int rslt = loadFont(fontHandle);
			if (!rslt) {
				return NULL;
			}
		}
		return fontCache.get(fontHash);
	}

	//int FontManager::loadGlyphData(Font& font)
	//{
	//	FT_Face face = hb_ft_font_get_face(font.hbFont);
	//	FT_GlyphSlot glyph = face->glyph;
	//	int srcWidth = glyph->bitmap.width;
	//	int srcHeight = glyph->bitmap.rows;
	//	stbrp_rect rect;
	//	rect.w = srcWidth;
	//	rect.h = srcHeight;
	//	stbrp_pack_rects(&font.packContext, &rect, 1);
	//	if (!rect.was_packed) {
	//		fprintf(stderr, "STBRP Error: Could not pack the rectangle.\n");
	//		return 0;
	//	}
	//	Texture* tex = new Texture(font.fontHandle);
	//	GlyphInfo* glyphPointer = font.glyphData;
	//	List<SubTexture> subTextures;
	//	List<TextureBuffer> buffers;
	//	for (int c = 32; c < font.glyphCapacity; c++) {
	//		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
	//			fprintf(stderr, "FreeType Error: Failed to load glyph %c", c);
	//			//return 0;
	//			continue;
	//		}

	//		int idx = c - 32;
	//		glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, glyph->bitmap.width, glyph->bitmap.rows, GL_RGBA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

	//		font.glyphData[idx].codepoint = c;
	//		font.glyphData[idx].glyphIndex = glyph->glyph_index;
	//		font.glyphData[idx].top = glyph->bitmap_top;
	//		font.glyphData[idx].left = glyph->bitmap_left;
	//		font.glyphData[idx].width = glyph->bitmap.width;
	//		font.glyphData[idx].height = glyph->bitmap.rows;
	//		font.glyphData[idx].advanceX = glyph->advance.x >> 6;
	//		font.glyphData[idx].u0 = (float)rect.x / font.width;
	//		font.glyphData[idx].v0 = (float)rect.y / font.height;
	//		font.glyphData[idx].u1 = (float)(rect.x + srcWidth) / font.width;
	//		font.glyphData[idx].v1 = (float)(rect.y + srcHeight) / font.height;

	//		/*unsigned int x0 = rect.x + font.glyphData[idx].left;
	//		unsigned int y0 = rect.y + font.glyphData[idx].top - (int)font.glyphData[idx].height;
	//		unsigned int x1 = x0 + font.glyphData[idx].width;
	//		unsigned int y1 = y0 + font.glyphData[idx].height;*/

	//		TextureBuffer buf;
	//		buf.textureFormatIn = GL_RGBA8;
	//		buf.textureFormatOut = GL_RGBA;
	//		buf.data = glyph->bitmap.buffer;
	//		buffers.append(buf);
	//		SubTexture* st = new SubTexture(rect.x, rect.y, font.glyphData[idx].width, font.glyphData[idx].height, rect.x / 2, rect.y / 2);
	//		subTextures.append(*st);
	//		
	//		if (c < 128)
	//			font.ascii[c] = c;

	//		font.numGlyphs++;
	//	}
	//	tex->setBufferData(buffers);
	//	tex->setSubTextures(subTextures);
	//	engine->getResourceManager()->getImageCache().set(font.fontHandle, *tex);
	//	font.dataBuffer = glyph->bitmap.buffer;
	//	FontHandle fName{ font };
	//	fontCache.set(fName, &font);
	//	return 1;
	//}



	FontHandle::FontHandle(const char* _fontName, const char* _fontFamily, unsigned char _fontSize): fontName(_fontName), fontStyle(_fontFamily), fontSize(_fontSize) {}

	FontHandle::FontHandle(const char* _fontName, unsigned char _fontSize) : FontHandle(_fontName, "", _fontSize) {}
}