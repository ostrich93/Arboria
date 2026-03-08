#version 330 core

in vec2 fTexCoord;
in vec4 fTint;
flat in int fPage;
flat in int uses_palette;

uniform isampler2DArray uPalettedSpritesheets;
uniform sampler2DArray uRgbaSpritesheets;
uniform sampler2D uPalette;

out vec4 out_color;

void main() {
	if (uses_palette == 1) {
		int index = texelFetch(uPalettedSpritesheets, ivec3(fTexCoord.x, fTexCoord.y, fPage), 0).r;
		if (index == 0) discard;
		out_color = texelFetch(uPalette, ivec2(index, 0), 0) * fTint;
	} else {
		out_color = texelFetch(uRgbaSpritesheets, ivec3(fTexCoord.x, fTexCoord.y, fPage), 0) * fTint;
	}
}