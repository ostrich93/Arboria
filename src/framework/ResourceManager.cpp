#include "ResourceManager.h"
#include "../FileSystem.h"
#include "../renderer/Font.h"
#include "../renderer/Texture.h"
#include "ResourceObject.h"
#include "../utils/Numerics.h"
#include "../definitions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../renderer/stb_image.h"
#include <mutex>

namespace Arboria {

	constexpr int32_t fontStringCacheSize = 256;

	HashTable<uint32_t, String> indexToSpriteFilenames = {
		{0, "background/mainBackground.png"},
		{1, "gui/ui.png"},
		{2, "gui/left_arr.png"},
		{3, "gui/right_arr.png"},
		{4, "gui/keys/keyboard_a.png"},
		{5, "gui/keys/keyboard_b.png"},
		{6, "gui/keys/keyboard_c.png"},
		{7, "gui/keys/keyboard_d.png"},
		{8, "gui/keys/keyboard_e.png"},
		{9, "gui/keys/keyboard_f.png"},
		{10, "gui/keys/keyboard_g.png"},
		{11, "gui/keys/keyboard_h.png"},
		{12, "gui/keys/keyboard_i.png"},
		{13, "gui/keys/keyboard_j.png"},
		{14, "gui/keys/keyboard_k.png"},
		{15, "gui/keys/keyboard_l.png"},
		{16, "gui/keys/keyboard_m.png"},
		{17, "gui/keys/keyboard_n.png"},
		{18, "gui/keys/keyboard_o.png"},
		{19, "gui/keys/keyboard_p.png"},
		{20, "gui/keys/keyboard_q.png"},
		{21, "gui/keys/keyboard_r.png"},
		{22, "gui/keys/keyboard_s.png"},
		{23, "gui/keys/keyboard_t.png"},
		{24, "gui/keys/keyboard_u.png"},
		{25, "gui/keys/keyboard_v.png"},
		{26, "gui/keys/keyboard_w.png"},
		{27, "gui/keys/keyboard_x.png"},
		{28, "gui/keys/keyboard_y.png"},
		{29, "gui/keys/keyboard_z.png"},
		{30, "gui/keys/keyboard_1.png"},
		{31, "gui/keys/keyboard_2.png"},
		{32, "gui/keys/keyboard_3.png"},
		{33, "gui/keys/keyboard_4.png"},
		{34, "gui/keys/keyboard_5.png"},
		{35, "gui/keys/keyboard_6.png"},
		{36, "gui/keys/keyboard_7.png"},
		{37, "gui/keys/keyboard_8.png"},
		{38, "gui/keys/keyboard_9.png"},
		{39, "gui/keys/keyboard_0.png"},
		{40, "gui/keys/keyboard_enter.png"},
		{41, "gui/keys/keyboard_escape.png"},
		{42, "gui/keys/keyboard_backspace.png"},
		{43, "gui/keys/keyboard_tab.png"},
		{44, "gui/keys/keyboard_space.png"},
		{45, "gui/keys/keyboard_minus.png"},
		{46, "gui/keys/keyboard_equals.png"},
		{47, "gui/keys/keyboard_bracket_open.png"},
		{48, "gui/keys/keyboard_bracket_close.png"},
		{49, "gui/keys/keyboard_slash_back.png"},
		{51, "gui/keys/keyboard_semicolon.png"},
		{52, "gui/keys/keyboard_apostrophe.png"},
		{53, "gui/keys/keyboard_tilde.png"},
		{54, "gui/keys/keyboard_comma.png"},
		{55, "gui/keys/keyboard_period.png"},
		{56, "gui/keys/keyboard_slash_forward.png"},
		{57, "gui/keys/keyboard_capslock.png"},
		{58, "gui/keys/keyboard_f1.png"},
		{59, "gui/keys/keyboard_f2.png"},
		{60, "gui/keys/keyboard_f3.png"},
		{61, "gui/keys/keyboard_f4.png"},
		{62, "gui/keys/keyboard_f5.png"},
		{63, "gui/keys/keyboard_f6.png"},
		{64, "gui/keys/keyboard_f7.png"},
		{65, "gui/keys/keyboard_f8.png"},
		{66, "gui/keys/keyboard_f9.png"},
		{67, "gui/keys/keyboard_f10.png"},
		{68, "gui/keys/keyboard_f11.png"},
		{69, "gui/keys/keyboard_f12.png"},
		{70, "gui/keys/keyboard_printscreen.png"},
		{74, "gui/keys/keyboard_home.png"},
		{75, "gui/keys/keyboard_page_up.png"},
		{76, "gui/keys/keyboard_delete.png"},
		{77, "gui/keys/keyboard_end.png"},
		{78, "gui/keys/keyboard_page_down.png"},
		{79, "gui/keys/keyboard_arrow_right.png"},
		{80, "gui/keys/keyboard_arrow_left.png"},
		{81, "gui/keys/keyboard_arrow_down.png"},
		{82, "gui/keys/keyboard_arrow_up.png"},
		{83, "gui/keys/keyboard_numlock.png"},
		{197, "gui/keys/keyboard_bracket_less.png"},
		{198, "gui/keys/keyboard_bracket_greater.png"},
		{224, "gui/keys/keyboard_ctrl.png"},
		{225, "gui/keys/keyboard_shift.png"},
		{226, "gui/keys/keyboard_alt.png"},
		{228, "gui/keys/keyboard_ctrl.png"},
		{229, "gui/keys/keyboard_shift.png"},
		{230, "gui/keys/keyboard_alt.png"},
		{231, "gui/scroller.png"},
	};

	const HashTable<uint32_t, String> indexToPaletteFilenames = {
		{0, "palettes/defaultgray.png"}
	};

	const HashTable<uint32_t, String> indexToFontFilenames = {
		{0, "logotyperegulargothic.ttf"}
	};

	class ResourceManagerImpl : public ResourceManager {
	private:
		HashTable<String, Image> textureCache;
		std::recursive_mutex textureCacheLock;

		HashTable<String, Font> fontCache;
		std::recursive_mutex fontCacheLock;

		FontStringCacheEntry _fontStringCache[fontStringCacheSize] = {};
		int32_t _fontStringCacheCount = 0;
		int32_t _fontStringCacheHitCount = 0;
		int32_t _fontStringCacheMissCount = 0;
		std::recursive_mutex fontStringCacheLock;

		uint32_t getFontStringCacheHash(Font* font, const String& text);
		void disposeFontStringCacheEntry(FontStringCacheEntry* entry);
		void freeFontStringCacheEntry(PaletteImage* surface);
	public:
		ResourceManagerImpl();
		~ResourceManagerImpl() override = default;
		Image* loadTexture(const uint32_t resourceId, bool isPaletted) override;
		Font* loadFont(const String& filename, const int ptSize) override;

		PaletteImage* getFontStringCacheEntry(Font* font, const String& text) override;
		void putFontStringCacheEntry(Font* font, const String& text, PaletteImage*& img) override;
	};

	ResourceManager* ResourceManager::createResourceManager()
	{
		return new ResourceManagerImpl();
	}

	uint32_t ResourceManagerImpl::getFontStringCacheHash(Font* font, const String& text)
	{
		uint32_t hash = static_cast<uint32_t>(((reinterpret_cast<uint32_t>(font) * 23) ^ 0xAAAAAAAA) & 0xFFFFFFFF);

		for (int i = 0; i < text.length(); i++) {
			hash = Numerics::ror32(hash, 3) ^ (text[i] * 13);
		}

		return hash;
	}

	void ResourceManagerImpl::disposeFontStringCacheEntry(FontStringCacheEntry* entry)
	{
		if (entry->image != nullptr) {
			freeFontStringCacheEntry(entry->image);
			entry->text.clear();
			entry->image = nullptr;
			entry->font = nullptr;
		}
	}

	void ResourceManagerImpl::freeFontStringCacheEntry(PaletteImage* surface)
	{
		free(surface);
	}

	ResourceManagerImpl::ResourceManagerImpl()
	{
	}

	Image* ResourceManagerImpl::loadTexture(const uint32_t resourceId, bool isPaletted)
	{
		std::lock_guard<std::recursive_mutex> l(textureCacheLock);
		String* imagePath;
		indexToSpriteFilenames.get(resourceId, &imagePath);

		if (!imagePath || imagePath->isEmpty()) {
			return nullptr;
		}

		Image* img;
		textureCache.get(*imagePath, &img);
		if (img) {
			return img;
		}

		unsigned int bytesRead;
		unsigned char* fileData = readBytesFromPhysFS(imagePath->c_str(), &bytesRead);
		if (bytesRead == 0)
			return nullptr;

		int imgBPP = 0;
		int width, height;

		stbi_set_flip_vertically_on_load(1);
		unsigned char* imgData = stbi_load_from_memory(fileData, bytesRead, &width, &height, &imgBPP, 0);

		if (imgData == NULL) {
			free(fileData);
			return nullptr;
		}

		if (width == 0 || height == 0) {
			stbi_image_free(imgData);
			free(fileData);
			return nullptr;
		}

		if (isPaletted) {
			PaletteImage* palImage = new PaletteImage({ width, height });
			PaletteImageLock pLock(palImage, ImageLockUse::Write);

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pLock.set({ x, y }, imgData[(y * width) + x]);
				}
			}

			img = palImage;
		}
		else {
			RGBAImage* rgbImage = new RGBAImage({ width, height });
			RGBAImageLock rgbLock(rgbImage, ImageLockUse::Write);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int baseIdx = imgBPP * width * y + imgBPP * x;
					Color c(imgData[baseIdx], imgData[imgBPP * width * y + imgBPP * x + 1],
						imgData[imgBPP * width * y + imgBPP * x + 2], imgBPP == 4 ? imgData[4 * width * y + 4 * x + 3] : 255);
					rgbLock.set({ x,y }, c);
				}
			}

			img = rgbImage;
		}
		
		img->resId = resourceId;
		img->filePath = *imagePath;
		switch (imgBPP) {
		case 1:
			img->setFormat(GL_R8UI);
			img->setInternalFormat(GL_RED_INTEGER);
			break;
		case 2:
			img->setFormat(GL_RG8);
			img->setInternalFormat(GL_RG);
			break;
		case 3:
			img->setFormat(GL_RGB8);
			img->setInternalFormat(GL_RGB);
			break;
		default:
			img->setFormat(GL_RGBA8);
			img->setInternalFormat(GL_RGBA);
			break;
		}
		textureCache.set(imagePath->c_str(), *img);
		return img;
	}

	Font* ResourceManagerImpl::loadFont(const String& filename, const int ptSize)
	{
		std::lock_guard<std::recursive_mutex> l(fontCacheLock);

		Font* fnt;
		fontCache.get(filename, &fnt);
		if (fnt) {
			return fnt;
		}

		bool isTTFInitialized = TTFInitialize();
		if (!isTTFInitialized)
			return nullptr;

		fnt = TTF_OpenFont(filename.c_str(), ptSize);
		if (fnt) {
			fontCache.set(filename, *fnt);
		}
		return fnt;
	}

	PaletteImage* ResourceManagerImpl::getFontStringCacheEntry(Font* font, const String& text) {
		uint32_t hash = getFontStringCacheHash(font, text);
		int32_t index = hash % fontStringCacheSize;

		FontStringCacheEntry* entry;

		std::lock_guard<std::recursive_mutex> l(fontStringCacheLock);

		for (int32_t i = 0; i < fontStringCacheSize; i++) {
			entry = &_fontStringCache[index];

			if (entry->image == nullptr)
				return nullptr;
			if (entry->font == font && entry->text.compare(text) == 0) {
				_fontStringCacheHitCount++;
				entry->lastUseTick = gCurrentDrawCount;
				return entry->image;
			}

			if (entry->lastUseTick < gCurrentDrawCount - 64) break;

			if (++index >= fontStringCacheSize) {
				index = 0;
			}
		}

		entry = &_fontStringCache[index];
		disposeFontStringCacheEntry(entry);
		_fontStringCacheMissCount++;

		return nullptr;
	}

	void ResourceManagerImpl::putFontStringCacheEntry(Font* font, const String& text, PaletteImage*& img) {
		uint32_t hash = getFontStringCacheHash(font, text);
		int32_t index = hash % fontStringCacheSize;

		std::lock_guard<std::recursive_mutex> l(fontStringCacheLock);
		_fontStringCache[index] = { img, font, text, gCurrentDrawCount }; //lastUseTick should be gCurrentDrawCount
		_fontStringCacheCount++;
	}
}