#include "DrawSurfaceShader.h"
#include "../framework/Camera.h"

namespace Arboria {

    DrawSurfaceShader::DrawSurfaceShader(unsigned int bufferCount) : ShaderProgram("baseSurfaceShader") {
        use();
        uTexture = getUniformLocation("uTexture");
        glUniform1i(uTexture, 0);

        uTexSize = getUniformLocation("uTexSize");
        uModel = getUniformLocation("uModel");
        uView = getUniformLocation("uView");
        uProjection = getUniformLocation("uProjection");

        for (unsigned int u = 0; u < bufferCount; u++) {
            SurfaceTextureBuffer b;

            glGenVertexArrays(1, &b.vao);
            glBindVertexArray(b.vao);
            glGenBuffers(1, &b.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(SurfaceVertexData), NULL, GL_STREAM_DRAW);
            
            glEnableVertexAttribArray(vScreenPosition);
            glVertexAttribPointer(vScreenPosition, 2, GL_FLOAT, GL_FALSE, sizeof(SurfaceVertexDescription), reinterpret_cast<void*>(offsetof(SurfaceVertexDescription, position)));
            glEnableVertexAttribArray(vTexCoord);
            glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(SurfaceVertexDescription), reinterpret_cast<void*>(offsetof(SurfaceVertexDescription, texCoord)));
            glEnableVertexAttribArray(vColor);
            glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, sizeof(SurfaceVertexDescription), reinterpret_cast<void*>(offsetof(SurfaceVertexDescription, color)));

            buffers.append(b);
        }
    }

    DrawSurfaceShader::~DrawSurfaceShader()
    {
        destroy();
        for (int u = 0; u < buffers.getLength(); u++) {
            glDeleteVertexArrays(1, &buffers[u].vao);
            glDeleteBuffers(1, &buffers[u].vbo);
        }
        buffers.clearFree();
    }

    void DrawSurfaceShader::drawTexture(Texture* img, Vector2<float> displayOnScreenSize, Vector2<float> screenPosition, Vector2<float> texSize, Vector2<float> rotationCenter, float rotationRadians, Color _color) {
        
        SurfaceVertexData v;
        static const Vector2<float> identityQuad[4] = { {0,0}, {0,1}, {1,0}, {1,1} };
        for (int i = 0; i < 4; i++) {
            v.vertices[i].position = identityQuad[i];
            v.vertices[i].texCoord = identityQuad[i];
            v.vertices[i].color = _color;
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(screenPosition, 0.0f));
        model = glm::translate(model, glm::vec3(0.5 * displayOnScreenSize.x, 0.5 * displayOnScreenSize.y, 0.0f));
        model = glm::rotate(model, rotationRadians, Vector3<float>{0.0f, 0.0f, 1.0f});
        model = glm::translate(model, glm::vec3(-0.5 * displayOnScreenSize.x, -0.5 * displayOnScreenSize.y, 0.0f));
        model = glm::scale(model, glm::vec3(displayOnScreenSize, 1.0f));
        
        setVector2f("uTexSize", texSize);
        setMatrix4("uModel", model);
        setMatrix4("uView", renderDevice->getCamera()->getViewMatrix());
        setMatrix4("uProjection", renderDevice->getCamera()->getViewProjection());

        auto& buf = buffers[currentBuffer];
        glBindBuffer(GL_ARRAY_BUFFER, buf.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SurfaceVertexData), &v);
        
        glBindVertexArray(buf.vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}