#version 330 core
in vec2 texCoord;
out vec4 color;

uniform sampler2D texture_atlas;
uniform vec4 pColor;
void main() {
    float sampleR = texture(texture_atlas, texCoord).r;
    vec4 sampled = vec4(1.0, 1.0, 1.0, sampleR);
    color = pColor * sampled;
}