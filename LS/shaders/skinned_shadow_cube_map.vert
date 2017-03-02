#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 2) in vec2 weight_0;
layout(location = 3) in vec2 weight_1;
layout(location = 4) in vec2 weight_2;
layout(location = 5) in vec2 weight_3;

uniform mat4 bones[64];

void main()
{
	gl_Position = weight_0.y * (bones[int(weight_0.x)] * vec4(vertex_position, 1.0));
	gl_Position += weight_1.y * (bones[int(weight_1.x)] * vec4(vertex_position, 1.0));
	gl_Position += weight_2.y * (bones[int(weight_1.x)] * vec4(vertex_position, 1.0));
	gl_Position += weight_3.y * (bones[int(weight_1.x)] * vec4(vertex_position, 1.0));
}
