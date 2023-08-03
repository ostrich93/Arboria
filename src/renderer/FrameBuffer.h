#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Texture.h"

//struct SDL_Window;

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
			void bind();
			void bindDraw();
			void bindRead();
			void blitPixels(FrameBuffer& src, GLenum filter);
			void swapColorBuffer(FrameBuffer& other);
			void copyBuffer(FrameBuffer& src, GLenum filter);
	};
}

#endif