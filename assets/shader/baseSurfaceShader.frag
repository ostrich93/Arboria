#version 330 core
in vec2 vTexCoord;
in vec4 vColor;
uniform sampler2D uTexture;
out vec4 color;
void main() {
    vec2 normalized_tex_coords = vTexCoord/textureSize(uTexture, 0));
    vec4 tmp_color = texture(uTexture, normalize_tex_coords);
    color = tmp_color * vColor;
}