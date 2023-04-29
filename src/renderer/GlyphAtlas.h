#ifndef GLYPH_ATLAS_H
#define GLYPH_ATLAS_H

#include "Font.h"
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace Arboria {

	int generateGlyphs(Font* font, GlyphAtlas* outAtlas, unsigned int length);

	class GlyphAtlas {
		public:
			GlyphAtlas(Font& font, unsigned int _width = 1024, unsigned int _height = 1024);
			GlyphAtlas(unsigned int _width);
			~GlyphAtlas();

			inline unsigned int getWidth() { return width; }
			inline const unsigned int getWidth() const { return width; }
			inline void setWidth(unsigned int w) { width = w; }

			inline unsigned int getHeight() { return height; }
			inline unsigned int getHeight() const { return height; }
			inline void setHeight(unsigned int h) { height = h; }

			inline Font* getParentFont() { return parentFont; }
			inline Font* getParentFont() const { return parentFont; }
			inline void setFont(Font& font) { parentFont = &font; }

			inline unsigned char* getDataBuffer() { return dataBuffer; }
			inline void setDataBuffer(unsigned char* db) { dataBuffer = db; }

			inline Glyph* getGlyphs(){ return glyphs; }
			inline int getTextureId() { return textureId; }

			void initialize();
			//void bind(int unit);
			Glyph* getGlyph(unsigned int codepoint);
			const Glyph* getGlyph(unsigned int codepoint) const;
			//GlyphInfo* getGlyph(unsigned short codepoint) const;
			Glyph* loadGlyphs(FT_Face& face, FontMetrics* size, int length);
			void setGlyphMetrics(Glyph* glyphMetrics, const FT_UInt& index, int xPos, FontMetrics* size, int yPos, FT_Face& face);
			unsigned char* loadGlyph(FT_Face& _face, unsigned short codepoint, Glyph& glyph);
			Font* getFont();
			void setFont(Font* _font);
			bool wasInitialized;
		private:
			friend class FontManager;
			unsigned int textureId;
			unsigned int width;
			unsigned int height;
			unsigned char* dataBuffer;
			Glyph* glyphs;
			Font* parentFont;
	};
}

#endif