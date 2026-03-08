#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexPosition;
layout (location = 2) in vec2 vSpriteSize;
layout (location = 3) in vec2 vScreenPosition;
layout (location = 4) in vec2 vDisplaySize;
layout (location = 5) in int vPage;
layout (location = 6) in int vUsesPalette;
layout (location = 7) in vec4 vTint;

uniform vec2 uViewportSize;

out vec2 fTexCoord;
out vec4 fTint;
flat out int fPage;
flat out int uses_palette;

void main() {
	uses_palette = vUsesPalette;
	fTint = vTint;
	fTexCoord = vTexPosition + vPosition * vSpriteSize;
	vec2 tmp_position = vScreenPosition + vPosition * vDisplaySize;

	tmp_position = ((tmp_position/uViewportSize) - vec2(0.5, 0.5)) * vec2(2,2);
	
	fPage = vPage;
	
	gl_Position = vec4(tmp_position.x, tmp_position.y, 0.0, 1.0);
}