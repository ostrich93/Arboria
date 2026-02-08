#include "Atlas.h"
#include <algorithm>

namespace Arboria {
	constexpr uint32_t unusedIndex = 0xFFFFFFFF;


	Atlas::Atlas(int32_t pageSize, GLenum format) : _pageSize(pageSize), format(format), tex_id(0)
	{
		std::fill(_indexMap.begin(), _indexMap.end(), unusedIndex);
	}

	Atlas::~Atlas()
	{
		if (tex_id)
			glDeleteTextures(1, &tex_id);
	}

	bool Atlas::isTextureValid(int32_t actualWidth, int32_t actualHeight) const
	{
		int32_t imageOrder = calculateImageSizeOrder(actualWidth, actualHeight);
		int32_t atlasOrder = log2(_imageSize);

		return imageOrder == atlasOrder;
	}

	AtlasImageInfo Atlas::allocateSprite(RGBAImage* i)
	{
		for (AtlasPage& page : _pages) {
			if (page.getFreeSlots() > 0 && page.isImageValid(i->getWidth(), i->getHeight())) {
				auto pInfo = page.allocate(i);
				upload(pInfo, i);
				return pInfo;
			}
		}

		auto pageIdx = static_cast<int32_t>(_pages.size());
		int32_t imageSize = powf(2, Atlas::calculateImageSizeOrder(i->getWidth(), i->getHeight()));

		_pages.append(AtlasPage(pageIdx, imageSize));
		_pages.end()->initialize();

		enlargeAtlas(1);

		auto info = _pages.end()->allocate(i);

		//for auto& page : _pages, go through entries and call upload on each one?
		upload(info, i);

		return info;
	}

	AtlasImageInfo Atlas::allocateSprite(PaletteImage* i)
	{
		for (AtlasPage& page : _pages) {
			if (page.getFreeSlots() > 0 && page.isImageValid(i->getWidth(), i->getWidth())) {
				auto pInfo = page.allocate(i);
				upload(pInfo, i);
				return pInfo;
			}
		}

		auto pageIdx = static_cast<int32_t>(_pages.size());
		int32_t imageSize = powf(2, Atlas::calculateImageSizeOrder(i->getWidth(), i->getHeight()));

		_pages.append(AtlasPage(pageIdx, imageSize));
		_pages.end()->initialize();

		enlargeAtlas(1);

		auto info = _pages.end()->allocate(i);

		//for auto& page : _pages, go through entries and call upload on each one?
		upload(info, i);

		return info;
	}

	AtlasPage::AtlasPage(int32_t pageNo, int32_t imageSize, int32_t pageSize) :
		_pageNumber(pageNo), _imageSize(imageSize), _pageSize(pageSize)
	{
	}

	void AtlasPage::initialize()
	{
		_cols = std::max(1, _pageSize / _imageSize);
		_rows = std::max(1, _pageSize / _imageSize);

		_freeSlots.resize(_cols * _rows);
		for (size_t i = 0; i < _freeSlots.size(); i++) {
			_freeSlots[i] = static_cast<GLuint>(i);
		}
	}

	AtlasImageInfo AtlasPage::allocate(Image* i)
	{
		assert(!_freeSlots.size() == 0);

		GLuint slot = _freeSlots.back();
		_freeSlots.removeLast();

		int32_t row = slot / _cols;
		int32_t col = slot % _cols;

		auto bounds = getSlotCoordinates(slot, i->getWidth(), i->getHeight());

		AtlasImageInfo info{};
		info.index = _pageNumber;
		info.slot = slot;
		info.bounds = bounds;
		info.renderSize = i->getSize();
		info.imageSize = _imageSize;
		info.pageSize = _pageSize;

		return info;
	}

	bool AtlasPage::isImageValid(int32_t actualWidth, int32_t actualHeight) const
	{
		int32_t imageOrder = Atlas::calculateImageSizeOrder(actualWidth, actualHeight);
		int32_t atlasOrder = log2(_imageSize);

		return imageOrder == atlasOrder;
		return false;
	}

	int32_t AtlasPage::getFreeSlots() const
	{
		return static_cast<int32_t>(_freeSlots.size());
	}

	int32_t Atlas::calculateImageSizeOrder(int32_t actualWidth, int32_t actualHeight)
	{
		int32_t actualSize = std::max(actualWidth, actualHeight);

		if (actualSize < 8) {
			actualSize = 8;
		}
		return static_cast<int32_t>(ceil(log2f(static_cast<float>(actualSize))));
	}

	Vector4<int> AtlasPage::getSlotCoordinates(GLuint slot, int32_t actualWidth, int32_t actualHeight) const
	{
		int32_t row = slot / _cols;
		int32_t col = slot % _cols;

		return Vector4<int>{
			_imageSize* col, _imageSize* row, _imageSize* col + actualWidth, _imageSize* row + actualHeight
		};
	}

	void AtlasPage::free(const AtlasImageInfo& info)
	{
		assert(_pageNumber == info.index);

		_freeSlots.append(info.slot);
	}

	void Atlas::upload(AtlasImageInfo& entry, RGBAImage* i)
	{
		if (!i) {
			//log error
			return;
		}

		assert(format == GL_RGBA8);
		RGBAImageLock l(i);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, entry.bounds.x, entry.bounds.y, entry.index, entry.renderSize.x, entry.renderSize.y, 1, GL_RGBA, GL_UNSIGNED_BYTE, l.getData());
		return;
	}

	void Atlas::upload(AtlasImageInfo& entry, PaletteImage* i)
	{
		if (!i) {
			//log error
			return;
		}

		assert(format == GL_R8UI);
		PaletteImageLock l(i);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, entry.bounds.x, entry.bounds.y, entry.index, entry.renderSize.x, entry.renderSize.y, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE, l.getData());
		return;
	}

	void Atlas::createTextures()
	{
		if (!tex_id) {
			glGenTextures(1, &tex_id);
			glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
	}

	void Atlas::enlargeAtlas(GLuint newEntries)
	{
		createTextures();

		GLuint newIndices = _pageIndices + newEntries;

		List<char> oldPixels;

		GLenum data_format;
		if (format == GL_RGBA8)
			data_format = GL_RGBA;
		else if (format == GL_R8UI)
			data_format = GL_RED_INTEGER;

		if (newIndices > _atlasCapacity) {
			oldPixels.resize(_pageSize * _pageSize * _atlasCapacity);

			_atlasCapacity = (_atlasCapacity + 6) << 1uL;

			glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, _pageSize, _pageSize, _atlasCapacity, 0, data_format, GL_UNSIGNED_BYTE, nullptr);

			if (!oldPixels.getLength()) {
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, _pageSize, _pageSize, _pageIndices, data_format, GL_UNSIGNED_BYTE, oldPixels.getPtr());
			}
		}

		_pageIndices = newIndices;

	}

	AtlasImageInfo Atlas::getOrLoadRgbSprite(RGBAImage* i)
	{
		uint32_t idx;
		{
			idx = _indexMap[i->resId];
			if (idx != unusedIndex) { 
				const auto& info = _spriteCache[idx];
				return info;
			}
		}

		idx = uint32_t(_spriteCache.size());

		auto info = allocateSprite(i);
		info.imageId = i->resId;
		info.texId = tex_id;

		_spriteCache.append(info);
		_indexMap[i->resId] = idx;

		return info;
	}

	AtlasImageInfo Atlas::getOrLoadPaletteSprite(PaletteImage* i)
	{
		uint32_t idx;
		{
			idx = _indexMap[i->resId];
			if (idx != unusedIndex) {
				const auto& info = _spriteCache[idx];
				return info;
			}
		}

		idx = uint32_t(_spriteCache.size());

		auto info = allocateSprite(i);
		info.imageId = i->resId;
		info.texId = tex_id;

		_spriteCache.append(info);
		_indexMap[i->resId] = idx;

		return info;
	}

	void Atlas::invalidateSprite(uint32_t resourceId)
	{
		uint32_t index = _indexMap[resourceId];
		if (index == unusedIndex) return;

		AtlasImageInfo& elem = _spriteCache[index];
		_pages[elem.index].free(elem);
		_indexMap[resourceId] = unusedIndex;

		if (index == _spriteCache.size() - 1) { //if it's the last image in the cache
			_spriteCache.removeLast();
		}
		else {
			//move the last element to the current map, then change the index for the moved element
			AtlasImageInfo& last = _spriteCache.back();
			elem = last;
			_indexMap[last.imageId] = index;

			_spriteCache.removeLast();
		}
	}

}