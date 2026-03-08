#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec4 vColor;

uniform vec2 uViewportSize;

out vec4 fColor;

void main() {
	fColor = vColor;
	vec2 tmpPos = vPosition;
	tmpPos /= uViewportSize;
	tmpPos -= vec2(0.5,0.5);
	tmpPos *= vec2(2,2);
	gl_Position = vec4(tmpPos.x, tmpPos.y, 0, 1);
}