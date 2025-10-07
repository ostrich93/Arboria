#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "./OpenGLApi.h"
#include "../Drawing.h"

struct SDL_Window;

namespace Arboria {
	class FrameBuffer {
		private:
			GLuint fboId;
			GLuint textureId;
			Vector2<unsigned int> size;

		public:
			explicit FrameBuffer(SDL_Window* window);
			FrameBuffer(int width, int height);
			FrameBuffer(Vector2<unsigned int> _size);
			~FrameBuffer();
			FrameBuffer(const FrameBuffer& other) = delete;
			FrameBuffer& operator=(const FrameBuffer&) = delete;
			GLuint getWidth() const {
				return size.x;
			}
			GLuint getHeight() const {
				return size.y;
			}
			Vector2<unsigned int> getSize() const {
				return size;
			}
			GLuint getTexture() const { return textureId; }
			GLuint getFBO() const { return fboId; }
			void bind() const;
			void bindDraw() const;
			void bindRead() const;
			void getPixels(RenderTarget& rt) const;

			void blitPixels(FrameBuffer& src, GLenum filter);
			void swapColorBuffer(FrameBuffer& other);
			void copyBuffer(FrameBuffer& src, GLenum filter);
			void setPixels(const RenderTarget& rt);
	};
}

#endif