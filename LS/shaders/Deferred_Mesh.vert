#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
//layout(location = 2) in vec2 uv_coord;


out vec3 Position0;
out vec3 Normal0;
//layout(location = 2) out vec2 TexCoord0;

uniform mat4 mv; //If scaling is applied to models this needs to be the so called "normalmatrix" which is the (mv^-1)^T = inverse transpose of model view matrix.
uniform mat4 mvp;


void main() {
	gl_Position = mvp * vec4(vertex_position, 1.0);
	//TexCoord0 = vec2(uv_coord.x, 1 + uv_coord.y * -1.0);

	//Calc normals:
	Normal0 = (mv * vec4(vertex_normal, 0)).xyz;
	Position0 =  (mv * vec4(vertex_position, 1.0)).xyz;
}
