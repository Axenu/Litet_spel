#version 400 core

out vec4 Frag_Data;

uniform vec4 color;

void main() {
    Frag_Data = color;
}
