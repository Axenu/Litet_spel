#version 400 core


layout(location = 0) in vec3 in_Position;

void main() {
    gl_Position = vec4(in_Position.xyz, 1.0);
}
