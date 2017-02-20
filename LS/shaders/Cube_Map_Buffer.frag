#version 400

in vec3 Position0;

layout(location = 0) out vec4 ColorOut;


/* Uniforms
*/
uniform samplerCube shadowCubeMap;
uniform uint bufferID;

void main () {

	ColorOut.w = 1.0f;
    float f;
    if (bufferID == 0)
        f = texture(shadowCubeMap, vec3(Position0.xy, -1)).r;
    else if (bufferID == 1)
        f = texture(shadowCubeMap, vec3(1, Position0.y, Position0.x)).r;
    else if (bufferID == 2)
        f = texture(shadowCubeMap, vec3(-Position0.x, Position0.y, 1)).r;
    else if (bufferID == 3)
        f = texture(shadowCubeMap, vec3(-1, Position0.y, Position0.x)).r;
    else if (bufferID == 4)
        f = texture(shadowCubeMap, vec3(Position0.y, 1, Position0.x)).r;
    else if (bufferID == 5)
        f = texture(shadowCubeMap, vec3(Position0.y, -1, Position0.x)).r;

    ColorOut.xyz = vec3(f, f, f);
}
