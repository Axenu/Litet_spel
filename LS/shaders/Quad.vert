#version 400
layout(location = 0) in vec3 vertex_position;

layout(location = 0) out vec3 Position0;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
}
