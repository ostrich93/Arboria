#include "FrameBuffer.h"

namespace Arboria {
	
	const GLuint BACKBUFFER_ID = 0;

	FrameBuffer::FrameBuffer(SDL_Window* window)
	{
		fboId = BACKBUFFER_ID;
		textureId = 0;
		int width, height;
		SDL_GL_GetDrawableSize(window, &width, &height);
		size = { width, height };
	}
	FrameBuffer::FrameBuffer(int width, int height)
	{
		size.x = width;
		size.y = height;

		glGenTextures(1, &textureId);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Engine::printError("FrameBuffer constuctor error: FrameBuffer for surface could not be generated.\n");
	}

	FrameBuffer::FrameBuffer(Vector2<unsigned int> _size)
	{
		size = _size;

		glGenTextures(1, &textureId);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Engine::printError("FrameBuffer constuctor error: FrameBuffer for surface could not be generated.\n");
	}

	FrameBuffer::~FrameBuffer()
	{
		if (fboId != BACKBUFFER_ID) {
			glDeleteTextures(1, &textureId);
			glDeleteFramebuffers(1, &fboId);
		}
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	}

	void FrameBuffer::bindDraw() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
	}

	void FrameBuffer::bindRead() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
	}

	void FrameBuffer::blitPixels(FrameBuffer& src, GLenum filter) {
		bindDraw();
		src.bindRead();
		glBlitFramebuffer(0, 0, src.size.x, src.size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, filter);
		bind();
	}

	void FrameBuffer::swapColorBuffer(FrameBuffer& other) {
		std::swap(textureId, other.textureId);
		
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, other.fboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, other.textureId, 0);
	}

	void FrameBuffer::copyBuffer(FrameBuffer& src, GLenum filter)
	{
		bindDraw();
		src.bindRead();
		glBlitFramebuffer(0, 0, src.getWidth(), src.getHeight(), 0, 0, getWidth(), getHeight(), GL_COLOR_BUFFER_BIT, filter);
		bind();
	}
}