#version 400

in vec3 Position0;
in vec3 Normal0;
//layout(location = 2) in vec2 TexCoord0;

out vec3 ColorOut;
out vec3 NormalOut;
out vec3 SpecularOut;

/* Uniforms
*/
uniform vec3 specularCol;
uniform vec3 diffuseCol;

/* Forward declare functions
*/
vec3 encodeNormal(in vec3 normal);

void main () {

	ColorOut = diffuseCol;
	//Compress normal:
	NormalOut = encodeNormal(Normal0);
	SpecularOut = specularCol;
}

vec3 encodeNormal(in vec3 normal){
	return normal * 0.5f + 0.5f;
}
