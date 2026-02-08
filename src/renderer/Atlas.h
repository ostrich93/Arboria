#ifndef ATLAS_H
#define ATALS_H

#include "Texture.h"
#include <array>

namespace Arboria {
	const int maxTextureDepth = 8;

	class AtlasImageInfo {
	public:
		GLuint index;
		GLuint slot;
		int32_t texId;
		uint32_t imageId; //resourceId
		int32_t imageSize; //size of the actual image
		int32_t pageSize; //size of the page this belongs to
		Vector2<unsigned int> renderSize;
		Vector4<int> bounds;

		~AtlasImageInfo() = default;
	};

	class AtlasPage {
	private:
		int32_t _rows = 0;
		int32_t _cols = 0;
		int32_t _pageSize; //used to derive rows/columns
		int32_t _imageSize; //the size of the sprite that can be stored in this page
		int32_t _pageNumber;
		List<GLuint> _freeSlots;
	public:
		AtlasPage(int32_t pageNo, int32_t imageSize, int32_t pageSize = 2048);

		void initialize();
		AtlasImageInfo allocate(Image* i);
		bool isImageValid(int32_t actualWidth, int32_t actualHeight) const;
		int32_t getFreeSlots() const;
		void free(const AtlasImageInfo& info);
		Vector4<int> getSlotCoordinates(GLuint slot, int32_t actualWidth, int32_t actualHeight) const;
	};

	class Atlas {
	public:
		Atlas(int32_t pageSize, GLenum format);
		~Atlas();

		GLuint tex_id;
		GLenum format;

		int32_t _atlasWidth = 0;
		int32_t _atlasHeight = 0;
		int32_t _pageSize = 0;
		int32_t _imageSize = 0; //the width and height of an image in the atlas;
		List<AtlasPage> _pages;
		List<AtlasImageInfo> _spriteCache;
		std::array<uint32_t, 0xFFF> _indexMap;

		GLuint _pageIndices = 0;
		GLuint _atlasCapacity = 0;

		//check if texture has right dimensions and/or format
		bool isTextureValid(int32_t actualWidth, int32_t actualHeight) const;
		AtlasImageInfo allocateSprite(RGBAImage* i);
		AtlasImageInfo allocateSprite(PaletteImage* i);
		static int32_t calculateImageSizeOrder(int32_t actualWidth, int32_t actualHeight);
		void upload(AtlasImageInfo& entry, RGBAImage* i);
		void upload(AtlasImageInfo& entry, PaletteImage* i);
		void createTextures();
		void enlargeAtlas(GLuint newEntries);
		AtlasImageInfo getOrLoadRgbSprite(RGBAImage* i);
		AtlasImageInfo getOrLoadPaletteSprite(PaletteImage* i);
		void invalidateSprite(uint32_t resourceId);

	};
}

#endif