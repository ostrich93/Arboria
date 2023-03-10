#version 330 core
in vec2 texCoord;
out vec4 color;

uniform sampler2D texture_atlas;
uniform vec3 pColor;
void main() {
    color = vec4(1.0f, 1.0f, 1.0f, texture2D(texture_atlas, texCoord.r)) * vec4(pColor, 1.0f);
}