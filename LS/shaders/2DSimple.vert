#version 400 core


layout(location = 0) in vec2 in_Position;

uniform float positionZ;
uniform mat3 modelMatrix;

void main() {
    gl_Position = vec4((modelMatrix * vec3(in_Position.xy, 1.0)).xy, positionZ, 1.0);
    // gl_Position = vec4(in_Position.xy * size + position.xy - vec2(1,1), position.z, 1.0);
}
