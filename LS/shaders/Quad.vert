#version 400
layout(location = 0) in vec3 vertex_position;

out vec3 Position0;

void main() {
	Position0 = vertex_position;
	gl_Position = vec4(vertex_position, 1.0);
}
