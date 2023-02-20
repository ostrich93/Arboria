#version 330 core
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 in_screen_position;
layout(location = 2) in vec2 in_screen_size;
uniform vec2 viewport_size;
out vec2 texCoord;
void main() {
	texCoord = vertex.zw;
	vec2 tmp_pos = in_screen_position + vertex.xy * in_screen_size;
	tmp_pos = ((tmp_pos/viewport_size) - vec2(0.5, 0.5)) * vec2(1,1);
	gl_Position = vec4(tmp_pos.x, tmp_pos.y, 0, 1);
}