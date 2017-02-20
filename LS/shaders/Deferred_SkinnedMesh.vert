#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 weight_0;
layout(location = 3) in vec2 weight_1;
layout(location = 4) in vec2 weight_2;
layout(location = 5) in vec2 weight_3;
//layout(location = 2) in vec2 uv_coord;

out vec3 Normal0;
//layout(location = 2) out vec2 TexCoord0;

uniform mat4 vp; //If scaling is applied to models this needs to be the so called "normalmatrix" which is the (mv^-1)^T = inverse transpose of model view matrix.
uniform mat4 v;
uniform mat4 bones[64];

void main() {
	int index = int(weight_0.x);
	gl_Position = weight_0.y * (bones[index] * vec4(vertex_position, 1.0));
	Normal0 = weight_0.y * (bones[index] * vec4(vertex_position, 0.f)).xyz;
	index = int(weight_1.x);
	gl_Position += weight_1.y * (bones[index] * vec4(vertex_position, 1.0));
	Normal0 += weight_1.y * (bones[index] * vec4(vertex_position, 0.f)).xyz;
	index = int(weight_1.x);
	gl_Position += weight_2.y * (bones[index] * vec4(vertex_position, 1.0));
	Normal0 += weight_2.y * (bones[index] * vec4(vertex_position, 0.f)).xyz;
	index = int(weight_1.x);
	gl_Position += weight_3.y * (bones[index] * vec4(vertex_position, 1.0));
	Normal0 += weight_3.y * (bones[index] * vec4(vertex_position, 0.f)).xyz;

	gl_Position = vp * gl_Position;
	Normal0 =  (v * vec4(Normal0, 0.f)).xyz;
}
