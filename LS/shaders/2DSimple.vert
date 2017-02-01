#version 400 core


layout(location = 0) in vec2 in_Position;

uniform vec3 position;
uniform vec2 size;

void main() {
    gl_Position = vec4(in_Position.xy * size + position.xy - vec2(1,1), position.z, 1.0);
}
