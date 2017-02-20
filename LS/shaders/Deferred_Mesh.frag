#version 400

in vec3 Normal0;
//layout(location = 2) in vec2 TexCoord0;

layout(location = 0) out vec4 ColorOut;
layout(location = 1) out vec4 NormalOut;
layout(location = 2) out vec4 SpecularOut;

/* Uniforms
*/
uniform vec3 diffuseCol;
uniform vec4 specularCol;

/* Forward declare functions
*/
vec3 encodeNormal(in vec3 normal);

void main () {

	ColorOut = vec4(diffuseCol, 1);
	//Compress normal:
	NormalOut = vec4(encodeNormal(Normal0), 1);
	SpecularOut = specularCol;
}

vec3 encodeNormal(in vec3 normal){
	return normal * 0.5f + 0.5f;
}
