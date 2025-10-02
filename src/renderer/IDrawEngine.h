#ifndef IDRAWENGINE_H
#define IDRAWENGINE_H

#include <cstdint>

namespace Arboria {
	struct RenderTarget;
	struct IDrawContext;

	enum DrawEngineType : int32_t {
		NONE = -1,
		SOFTWARE_WITH_HARDWARE_DISPLAY,
		OPENGL,
		COUNT
	};

	struct IDrawEngine {
		virtual ~IDrawEngine() {}

		virtual void initialize() = 0;
		virtual void resize(uint32_t width, uint32_t height) = 0;
		virtual void setVSync(bool vSync) = 0;

		virtual void invalidate(int32_t left, int32_t top, int32_t right, int32_t bottom) = 0;
		virtual void beginDraw() = 0;
		virtual void endDraw() = 0;
		virtual void copyRect(int32_t x, int32_t y, int32_t width, int32_t height, int32_t dx, int32_t dy) = 0;

		virtual IDrawContext* getDrawContext() = 0;
		virtual RenderTarget* getRenderTarget() = 0;

		virtual void invalidateImage(uint32_t image) = 0;
	};
}

#endif