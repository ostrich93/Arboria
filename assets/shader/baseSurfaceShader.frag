#version 330 core
in vec2 fTexCoord;
in vec4 fTint;

uniform int uIsPaletted;
uniform isampler2D uPaletteTexture;
uniform sampler2D uRgbTexture;
uniform sampler2D uPalette;

out vec4 fragmentColor;
void main() {
    vec4 tempColor;
    if (uIsPaletted == 1) {
        int idx = texelFetch(uPaletteTexture, ivec2(fTexCoord.x, fTexCoord.y), 0).r;
        if (idx == 0) discard;
        tempColor = texelFetch(uPalette, ivec2(idx, 0), 0);
    } else {
        vec2 normalizedTexCoords = fTexCoord / vec2(textureSize(uRgbTexture, 0));
        tempColor = texture(uRgbTexture, normalizedTexCoords);
    }
    fragmentColor = tempColor * fTint;
}