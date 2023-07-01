#version 330 core
in vec2 vPosition;
in vec4 in_color;
out vec4 vColor;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(vPosition.xy, 0.0, 1.0);
	vColor = in_color;
}