#include "RenderData.h"
#include "Palette.h"
#include "Texture.h"
#include <iostream>

namespace Arboria {
	RenderData::~RenderData() = default;

	GLPalette::GLPalette(Palette* parent) : tex_id(0) {
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, parent->colors.getLength(), 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, parent->colors.getPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLPalette::~GLPalette() {
		if (tex_id) {
			glDeleteTextures(1, &tex_id);
		}
	}

	GLPaletteTexture::GLPaletteTexture(PaletteImage* parent) : size(parent->getSize()) {
		PaletteImageLock l(parent);
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, parent->getWidth(), parent->getHeight(), 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, l.getData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLPaletteTexture::~GLPaletteTexture() {
		if (tex_id) {
			glDeleteTextures(1, &tex_id);
		}
	}

	GLRGBATexture::GLRGBATexture(RGBAImage* parent) : size(parent->getSize()) {
		RGBAImageLock l(parent);
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, parent->getWidth(), parent->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, l.getData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLRGBATexture::~GLRGBATexture() {
		if (tex_id)
			glDeleteTextures(1, &tex_id);
	}

	Framebuffer::Framebuffer(Vector2<int> _size) : size(_size) {
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &fbo_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);

		const GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			printf("ERROR::DRAW_FRAMEBUFFER:: Framebuffer is not complete: %d", status);
		}

		const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::Framebuffer(GLuint fbo, Vector2<unsigned int> _sz) : fbo_id(fbo), tex_id(0), size(_sz) {

	}

	Framebuffer::~Framebuffer()
	{
		if (fbo_id != 0) {
			glDeleteTextures(1, &tex_id);
			glDeleteFramebuffers(1, &fbo_id);
		}
	}

	void Framebuffer::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
		glViewport(0, 0, size.x, size.y);
	}
	void Framebuffer::bindDraw() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
	}
	void Framebuffer::bindRead() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id);
	}
}