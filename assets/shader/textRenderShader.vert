#version 330 core
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec4 vColor;
out vec2 texCoord;
out vec4 fColor;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(vPosition.xy, 0.0, 1.0);
	texCoord = vTexCoord;
	fColor = vColor;
}