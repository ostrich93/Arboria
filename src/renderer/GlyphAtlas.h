#ifndef __GLYPH_ATLAS_H__
#define __GLYPH_ATLAS_H__

#include "Font.h"
#include "stb_rect_pack.h"

namespace Arboria {


	class GlyphAtlas {
	public:
		struct GlyphInfo {
			Glyph glyph;
			float u0;
			float v0;
			float u1;
			float v1;
		};

	public:
		GlyphAtlas(Font& font, unsigned int _width = 1024, unsigned int _height = 1024, int capacity = 128);
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

		//void loadGlyphs();
		//void reloadGlyphs();
		//void repack();
		void initialize();
		void bind(int unit);
		Glyph* getGlyph(unsigned short codepoint);
		//GlyphInfo* getGlyph(unsigned short codepoint) const;
		bool loadGlyphs();
		unsigned char* loadGlyph(FT_Face& _face, unsigned short codepoint, Glyph& glyph);
		bool addEntry(unsigned short codepoint, Glyph& glyph, const unsigned char* img);
		void bind(int unit);

	private:
		friend class FontManager;
		struct dirty_rect {
			unsigned int x1;
			unsigned int y1;
			unsigned int x2;
			unsigned int y2;
		};
		bool isDirty;
		dirty_rect dirtyArea;
		unsigned int textureId;
		unsigned int width;
		unsigned int height;
		unsigned char* dataBuffer;
		stbrp_context pack_context;
		stbrp_node* pack_nodes;
		int glyphCapacity;
		HashTable<size_t, Glyph> glyphs;
		Font* parentFont;

		void updateDirtyArea(int x, int y, int width, int height);
	};
}

#endif