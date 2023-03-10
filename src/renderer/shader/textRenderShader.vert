#version 330 core
layout(location = 0) in vec4 vertex;
out vec2 texCoord;

uniform mat4 projection;
uniform int buffer_number;

void main() {
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	texCoord = vertex.zw;
}