#version 400

in vec3 Position0;

layout(location = 0) out vec4 ColorOut;


/* Uniforms
*/
// Frame buffer textures:
uniform sampler2D colBuffer;
uniform sampler2D norBuffer;
uniform sampler2D specBuffer;
uniform sampler2D depthBuffer;

//X: 1 / width  Y : 1 / height
uniform vec2 screenInv;
//Near and far plane
uniform float near, far;
//Right X coord on nearplane, Top Y coord on nearplane
uniform float right, top;
uniform uint bufferID;

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord();
vec3 positionFromDepth(in float depth, in vec2 frameCoord);
vec3 decodeNormal(in vec3 normal);

float mod1(in float value){
	value = abs(value);
	return value - floor(value);
}
/* Composition main function
*/
void main () {

	//Calculate frame texture coordinate:
	vec2 frameCoord = calcFrameCoord();

	vec3 color = texture(colBuffer, frameCoord).xyz;
	vec3 normal = texture(norBuffer, frameCoord).xyz;
	vec4 specular = texture(specBuffer, frameCoord);
	float depth  = texture(depthBuffer, frameCoord).x;

	//normal = decodeNormal(normal);
	vec3 pos = positionFromDepth(depth, frameCoord);

	ColorOut.w = 1.0f;
	//Calc light
	if (bufferID == 0)
		ColorOut.xyz = color;
	else if (bufferID == 1)
		ColorOut.xyz = normal;
	else if (bufferID == 2)
		ColorOut.xyz = specular.xyz;
	else if (bufferID == 3)
		ColorOut.xyz = vec3(specular.w);
	else if (bufferID == 4)
		ColorOut.xyz = vec3(depth);
	else if (bufferID == 5)
		ColorOut.xyz = vec3(mod1(-pos.z / (far - near)));
	else if (bufferID == 6)
		ColorOut.xyz = vec3(mod1(pos.x));
	else if (bufferID == 7)
		ColorOut.xyz = vec3(mod1(pos.y));
	else if (bufferID == 8)
		ColorOut.xyz = vec3(mod1(pos.z));
	else if (bufferID == 9)
		ColorOut.xyz = vec3(mod1(pos.x), mod1(pos.y), mod1(pos.z));
}

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord(){
	//Divide window coordinate by screen size => Range [0, 1]
	return vec2(gl_FragCoord.x * screenInv.x,
				gl_FragCoord.y * screenInv.y);
}
/* Decode normal
*/
vec3 decodeNormal(in vec3 normal){
	//The decode needs to be renormalized due to rounding errors
	return normalize((normal - 0.5f) * 2.0f);
}

/* Calculates position coordinate in viewspace from depth
depth	  	<<	Depth of the fragment (gl_FragDepth)
fragCoord	<<	Frame coordinate of the fragment (calcFrameCoord)
return		>>	The viewspace position
*/
vec3 positionFromDepth(in float depth, in vec2 frameCoord)
{
	vec2 ndc;             // Reconstructed NDC-space position
	vec3 eye;             // Reconstructed EYE-space position

	ndc.x = (frameCoord.x * 2.0 - 1.0);
	ndc.y = (frameCoord.y * 2.0 - 1.0);


	/* Z_eye is calculated by separating it from the calculation from view->ndc space:
	Z_clip	= Z_eye * -(far + near)/(far - near) -  2 * far * near/(far - near)
	Z_ndc	= Z_clip / -Z_eye
	depth	= Z_ndc * 0.5 + 1
	*/
	eye.z = near * far / ((depth * (far - near)) - far);

	/* X_eye and Y_eye is then calculated from the same principle:
	X_ndc = (X_eye * near/right) / -Z_eye
	Y_ndc = (Y_eye * near/top) / -Z_eye
	*/
	eye.x = (ndc.x * -eye.z) * right;
	eye.y = (ndc.y * -eye.z) * top;

  return eye;
}
