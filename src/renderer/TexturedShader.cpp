#include "TexturedShader.h"
#include "Texture.h"
#include "RenderData.h"

namespace Arboria {

    TexturedShader::TexturedShader(unsigned int bufferCount) : ShaderProgram("baseSurfaceShader"), currentBuffer(0), usedBuffers(0) {
        assert(bufferCount);

        compileShader(name);
        use();
        getLocations();

        for (unsigned int i = 0; i < bufferCount; i++) {
            TextureQuadBuffer b;
            glGenVertexArrays(1, &b.vao_id);
            glBindVertexArray(b.vao_id);

            glGenBuffers(1, &b.position_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, b.position_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVertices), nullptr, GL_STREAM_DRAW);

            const GLuint floatsPerVert = 2;
            const GLuint floatsPerUV = 2;

            //GLint stride = sizeof(GLfloat) * (floatsPerVert + floatsPerUV); //sizeof(GLfloat) * floats per vertex (2) + floats per color (4) + floats per uv (2)

            glEnableVertexAttribArray(vPosition);
            glVertexAttribPointer(vPosition, floatsPerVert, GL_FLOAT, GL_FALSE, sizeof(TextureVDStruct), (GLvoid*)offsetof(TextureVDStruct, position));

            //glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * floatsPerVert));
            //glEnableVertexAttribArray(1);
                                                                            //floats per vertex
            glEnableVertexAttribArray(vTexCoord);
            glVertexAttribPointer(vTexCoord, floatsPerUV, GL_FLOAT, GL_FALSE, sizeof(TextureVDStruct), (GLvoid*)offsetof(TextureVDStruct, texCoord));

			glEnableVertexAttribArray(vTint);
			glVertexAttribPointer(vTint, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TextureVDStruct), (GLvoid*)offsetof(TextureVDStruct, tint));

            buffers.append(b);
        }
    }

    TexturedShader::~TexturedShader()
    {
        if (programId) {
            glDeleteProgram(programId);
        }
        destroy();
        for (auto buf: buffers) {
            glDeleteVertexArrays(1, &buf.vao_id);
            glDeleteBuffers(1, &buf.position_buffer);
        }
        buffers.clearFree();
    }

	void TexturedShader::draw(PaletteImage* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, Color tint)
	{
		auto tex = dynamic_cast<GLPaletteTexture*>(img->renderData);
		if (!tex) {
			tex = new GLPaletteTexture(img);
			img->renderData = tex;
		}
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex->tex_id);

		TextureVertices v;

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(viewport_size.x), 0.0f, static_cast<float>(viewport_size.y));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 1));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation for now is always 0 but when it becomes time, we'll replace it with a parameter value
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 1));
		model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

		for (unsigned int i = 0; i < 4; i++) {
			auto p = identity_quad[i];

			v.vertices[i].position = identity_quad[i];
			v.vertices[i].texCoord = identity_quad[i];
			v.vertices[i].tint = tint;
		}

		use();

		glUniform1i(uFlipY, 0);
		glUniform1i(uIsPaletted, 1);
		glUniform2f(uTexSize, (float)img->getWidth(), (float)img->getHeight());
		glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(projection));

		auto& buf = buffers[currentBuffer];
		currentBuffer = (currentBuffer + 1) % buffers.getLength();
		usedBuffers++;

		glBindBuffer(GL_ARRAY_BUFFER, buf.position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureVertices), &v);

		glBindVertexArray(buf.vao_id);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void TexturedShader::draw(RGBAImage* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, bool isLinear, Color tint)
	{
		auto tex = dynamic_cast<GLRGBATexture*>(img->renderData);
		if (!tex) {
			tex = new GLRGBATexture(img);
			img->renderData = tex;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->tex_id);
		if (isLinear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		TextureVertices v;

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(viewport_size.x), 0.0f, static_cast<float>(viewport_size.y));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation for now is always 0 but when it becomes time, we'll replace it with a parameter value
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

		for (unsigned int i = 0; i < 4; i++) {
			auto p = identity_quad[i];

			v.vertices[i].position = identity_quad[i];
			v.vertices[i].texCoord = identity_quad[i];
			v.vertices[i].tint = tint;
		}

		use();

		glUniform1i(uFlipY, 0);
		glUniform1i(uIsPaletted, 0);
		glUniform2f(uTexSize, (float)img->getWidth(), (float)img->getHeight());
		glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(projection));

		auto& buf = buffers[currentBuffer];
		currentBuffer = (currentBuffer + 1) % buffers.getLength();
		usedBuffers++;

		glBindBuffer(GL_ARRAY_BUFFER, buf.position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureVertices), &v);

		glBindVertexArray(buf.vao_id);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void TexturedShader::draw(Surface* img, glm::vec2 position, glm::vec2 size, glm::uvec2 viewport_size, bool isLinear, bool renderDefaultSurface, Color tint)
	{
		auto tex = dynamic_cast<Framebuffer*>(img->renderData);

		if (!tex) {
			tex = new Framebuffer(img->getSize());
			img->renderData = tex;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->tex_id);
		if (isLinear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		TextureVertices v;
		glm::mat4 projection = renderDefaultSurface ?
			glm::ortho(0.0f, static_cast<float>(viewport_size.x), static_cast<float>(viewport_size.y), 0.0f, -1.0f, 1.0f)
			: glm::ortho(0.0f, static_cast<float>(viewport_size.x), 0.0f, static_cast<float>(viewport_size.y));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation for now is always 0 but when it becomes time, we'll replace it with a parameter value
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0));
		model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

		for (unsigned int i = 0; i < 4; i++) {
			auto p = identity_quad[i];

			v.vertices[i].position = identity_quad[i];
			v.vertices[i].texCoord = identity_quad[i];
			v.vertices[i].tint = tint;
		}

		use();

		glUniform1i(uFlipY, 0);
		glUniform1i(uIsPaletted, 0);
		glUniform2f(uTexSize, (float)img->getWidth(), (float)img->getHeight());
		glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(projection));

		auto& buf = buffers[currentBuffer];
		currentBuffer = (currentBuffer + 1) % buffers.getLength();
		usedBuffers++;

		glBindBuffer(GL_ARRAY_BUFFER, buf.position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureVertices), &v);

		glBindVertexArray(buf.vao_id);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void TexturedShader::getLocations()
	{
		uModel = getUniformLocation("uModel");
		uProjection = getUniformLocation("uProjection");
		uFlipY = getUniformLocation("uFlipY");
		uTexSize = getUniformLocation("uTexSize");

		uPaletteTexture = getUniformLocation("uPaletteTexture");
		glUniform1i(uPaletteTexture, 1);

		uRgbTexture = getUniformLocation("uRgbTexture");
		glUniform1i(uRgbTexture, 0);

		uPalette = getUniformLocation("uPalette");
		glUniform1i(uPalette, 2);

		uIsPaletted = getUniformLocation("uIsPaletted");

		vPosition = getAttributeLocation("vPosition");
		vTexCoord = getAttributeLocation("vTexCoord");
		vTint = getAttributeLocation("vTint");
	}
	void TexturedShader::setUniforms(GLuint textureId, glm::vec2 position, glm::vec2 size)
	{
		TextureVertices v;

		glm::vec2 viewport_size = glm::vec2(640, 360);
		glm::vec4 posCoords[4] = { {position.x, position.y, 0, 1}, {position.x, position.y + size.y, 0, 1}, {position.x + size.x, position.y, 0, 1}, {position.x + size.x, position.y + size.y, 0, 1} };

		for (unsigned int i = 0; i < 4; i++) {
			auto p = identity_quad[i];
			p *= viewport_size;
			p.x = posCoords[i].x;
			p.y = posCoords[i].y;

			v.vertices[i].position[0] = p.x;
			v.vertices[i].position[1] = p.y;
			v.vertices[i].texCoord[0] = identity_quad[i].x;
			v.vertices[i].texCoord[1] = identity_quad[i].y;
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureVertices), &v);
		//use();
		//glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(uRgbTexture, 0);
	}
}