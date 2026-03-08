#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec4 vTint;

uniform vec2 uTexSize;
uniform bool uFlipY;

out vec2 fTexCoord;
out vec4 fTint;

void main() {
	fTexCoord = vTexCoord * uTexSize;
	fTint = vTint;
	if (uFlipY) gl_Position = vec4(vPosition.x, -vPosition.y, 0.0, 1.0);
	else gl_Position = vec4(vPosition.x, vPosition.y, 0.0, 1.0);
}