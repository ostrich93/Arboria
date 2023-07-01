#include "Font.h"
#include "Texture.h"
#include "../globals.h"
#include "../definitions.h"
#include "../FileSystem.h"
#include "../Heap.h"
#include <cstdio>
#include <exception>
#include <physfs.h>

namespace Arboria {

	Font::Font(const char* name, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle{ name, "", ptSz }, format(_format), width(_width), height(_height), fontSize(ptSz), length(0), size{ ptSz, 0, 0, {0,0} }, glyphAtlas(NULL) {
		
	}

	Font::Font(const char* name, const char* family, unsigned int _format, unsigned char ptSz, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle{ name, family, ptSz }, format(_format), width(_width), height(_height), fontSize(ptSz), length(0), size{ ptSz, 0, 0, {0,0} }, glyphAtlas(NULL) {
		
	}

	Font::Font(FontHandle& fname, unsigned int _format, unsigned _width, unsigned _height) : wasInitialized(false), fontHandle(fname), format(_format), width(_width), height(_height), fontSize(fname.fontSize), length(0), size{ fname.fontSize, 0, 0, { 0, 0 } }, glyphAtlas(NULL) {}

	Font::~Font()
	{
	}

	int Font::initialize() {
		if (wasInitialized) return 1;

		String fontfilename = fontManager->getFontFilename(fontHandle.fontName, fontHandle.fontStyle);
		FT_Face face = fontManager->getCurrentFace();
		if (FT_New_Face(fontManager->getFontLibrary(), fontfilename.c_str(), 0, &face)) {
			fprintf(stderr, "FreeType Error: Could not open font %s\n", fontHandle.fontName.c_str());
			FT_Done_Face(fontManager->getCurrentFace());
			return 0;
		}

		if (FT_Set_Pixel_Sizes(face, size.ptSize, size.ptSize)) {
			FT_Done_Face(face);
			fprintf(stderr, "FreeType Error: Could not set sizes\n");
			return 0;
		}

		FT_Glyph_Metrics metrics[256];
		int maxBearing = 0;
		int minHang = 0;
		unsigned int maxWidth = 0;
		unsigned int cellH = 0;
		Vector2<unsigned int> maxAdvance = { 0, 0 };
		int glyphSize = 0;
		FT_Error error;
		for (unsigned int c = 0; c < 256; c++) {
			error = FT_Load_Char(face, c, FT_LOAD_RENDER);
			if (error) {
				Engine::printError("Freetype Error %d: Could not load character %c.\n", error, c);
			}
			else {
				metrics[c] = face->glyph->metrics;
				glyphInfo[c].glyph.advance.x = face->glyph->metrics.horiAdvance >> 6;
				glyphInfo[c].glyph.advance.y = face->glyph->metrics.vertAdvance >> 6;
				glyphInfo[c].glyph.bearing.x = metrics[c].horiBearingX >> 6;
				glyphInfo[c].glyph.bearing.y = metrics[c].horiBearingY >> 6;
				glyphInfo[c].glyph.size.x = face->glyph->bitmap.width;
				glyphInfo[c].glyph.size.y = face->glyph->bitmap.rows;
				glyphInfo[c].glyph.left = face->glyph->bitmap_left;
				glyphInfo[c].glyph.top = face->glyph->bitmap_top;
				
				if (c > 0) {
					FT_Vector kerning;
					error = FT_Get_Kerning(face, c - 1, c, FT_KERNING_DEFAULT, &kerning);
					if (error) {
						Engine::printError("Freetype Error %d: Could not load kerning for character %c.\n", error, c);
					}
					else {
						glyphInfo[c].glyph.kerning.x = kerning.x >> 6;
						glyphInfo[c].glyph.kerning.y = kerning.y >> 6;
					}
				}

				maxBearing = Math::iMax(maxBearing, glyphInfo[c].glyph.bearing.y);
				maxWidth = Math::iMax(maxWidth, metrics[c].width >> 6);
				maxAdvance.x = Math::iMax(maxAdvance.x, glyphInfo[c].glyph.advance.x);
				maxAdvance.y = Math::iMax(maxAdvance.y, glyphInfo[c].glyph.advance.y);
				int glyphHang = (metrics[c].horiBearingY - metrics[c].height) >> 6;
				minHang = Math::iMin(minHang, glyphHang);
			}
		}
		cellH = maxBearing - minHang;
		size.max_height = cellH;
		size.max_width = maxWidth;
		size.max_advance = maxAdvance;
		TextureAtlas* gAtlas = new TextureAtlas(fontHandle.fontName.c_str());
		//unsigned int _length = (unsigned int)Math::ceil(Math::sqrt(face->num_glyphs));
		
		length = 16;
		unsigned int fwidth = 16 * maxWidth;
		unsigned int fheight = 16 * cellH;
		gAtlas->intializePixelData(fwidth, fheight, GL_RED);
		gAtlas->generateTexture();

		int currRow = 0;
		int currCol = 0;
		int s = 0;
		int t = 0;
		Rectangle rect = { 0, 0, (float)maxWidth, (float)cellH };
		for (unsigned int ch = 0; ch < 256; ch++) {
			Glyph glyph = glyphInfo[ch].glyph;
			currRow = ch / 16;
			currCol = ch % 16;
			s = maxWidth * currCol + glyph.left;
			t = cellH * currRow + glyph.top;
			glyphInfo[ch].u0 = s / fwidth;
			glyphInfo[ch].v0 = t / fheight;	
			glyphInfo[ch].u1 = s + glyph.size.x / fwidth;
			glyphInfo[ch].v1 = t + glyph.size.y / fheight;

			rect.x = (float)s;
			rect.y = (float)t;
			rect.w = maxWidth;
			rect.h = cellH;
			gAtlas->addSubtexture(s, t, glyph.size.x, glyph.size.y, face->glyph->bitmap.buffer, GL_RED);
			gAtlas->addClip({rect.x, rect.y, (float)glyph.size.x, (float)glyph.size.y});
			gAtlas->unbindAtlas();
		}
		
		FT_Done_Face(face);
		wasInitialized = true;
		return 1;

	}

	GlyphEntry* Font::getGlyphEntry(unsigned int codePoint)
	{
		if (codePoint > 255 || codePoint < 0)
			return NULL;

		return &glyphInfo[codePoint];
	}

	List<int> Font::getGlyphs(const String& string)
	{
		List<int> glyphs;
		for (int i = 0; i < string.length(); i++) {
			glyphs.append(string[i]);
		}
		return glyphs;
	}

	List<int> Font::getGlyphs(const char* string) {
		List<int> glyphs;
		for (int i = 0; i < strlen(string); i++) {
			glyphs.append(string[i]);
		}
		return glyphs;
	}

	Glyph* Font::getGlyph(unsigned int _codePoint)
	{
		if (_codePoint > 255 || _codePoint < 0)
			return NULL;

		return &glyphInfo[_codePoint].glyph;
	}

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

	const String FontManager::getFontFilename(const char* family, const char* style)
	{
		List<String> pathTokens;
		if (!style || strlen(style) == 0) {
			pathTokens = {family, ".ttf"};
		}
		else {
			pathTokens = {family, "-", style, ".ttf" };
		}

		String pathName = String::join(pathTokens, "");
		return PHYSFS_getRealDir(pathName.c_str()) + pathName;
	}

	int FontManager::loadFont(const char* family, const char* style, unsigned int size)
	{
		Font font { family, style, 0, static_cast<unsigned char>(size) };
		
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

	FontHandle::FontHandle(const char* _fontName, const char* _fontFamily, unsigned char _fontSize): fontName(_fontName), fontStyle(_fontFamily), fontSize(_fontSize) {}

	FontHandle::FontHandle(const char* _fontName, unsigned char _fontSize) : FontHandle(_fontName, "", _fontSize) {}
}