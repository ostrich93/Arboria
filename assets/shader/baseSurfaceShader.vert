#version 330 core
layout(location = 0) in vec2 vScreenPosition;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec4 in_color;
uniform vec2 uTexSize;
uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;
out vec2 vTexCoord;
out vec4 vColor;
void main() {
	vTexCoord = in_texCoord * uTexSize;
	vColor = in_color;
	gl_Position = uProjection * uView * uModel * vec4(vScreenPosition.xy, 0.0, 1.0);
}