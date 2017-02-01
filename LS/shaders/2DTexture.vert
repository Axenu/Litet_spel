#version 400 core


layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_UV;

out vec2 pass_UV;

uniform vec3 position;
uniform vec2 size;

void main() {
	pass_UV = in_UV;
    gl_Position = vec4(in_Position.xy * size + position.xy - vec2(1,1), position.z, 1.0);
}
