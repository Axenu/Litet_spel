#version 400

layout(location = 0) in vec3 Position0;

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

//Light components
uniform vec3 pLightspecCol;
uniform vec3 pLightDiffCol;
uniform vec3 pLightPos;
uniform float pLight_Fade;

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord();
vec3 positionFromDepth(in float depth, in vec2 frameCoord);
vec3 decodeNormal(in vec3 normal);
vec3 pointLightCalc(in vec3 pos, in vec3 nor, in vec3 diffuseCol, in vec3 specularCol);
float lightCalc(in vec3 lightDir, in vec3 pos, in vec3 nor, out float lambertian);

/* Composition main function
*/
void main () {

	//Calculate frame texture coordinate:
	vec2 frameCoord = calcFrameCoord();

	vec3 color = texture(colBuffer, frameCoord).xyz;
	vec3 normal = texture(norBuffer, frameCoord).xyz;
	vec3 specular = texture(specBuffer, frameCoord).xyz;
	float depth  = texture(depthBuffer, frameCoord).x;

	//normal = decodeNormal(normal);
	//vec3 position = positionFromDepth(depth, frameCoord);

	//Calc light
	ColorOut = vec4(1.0f);
	  // pointLightCalc(position, normal, color, specular) +
	   //color * 0.2f, 1.0f); //Add ambient
}

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord(){
	//Divide window coordinate by screen size => Range [0, 1]
	return vec2(gl_FragCoord.x * screenInv.x,
				gl_FragCoord.y * screenInv.y);
}
/*	Point light specular calculation:
*/
vec3 pointLightCalc(in vec3 pos, in vec3 nor, in vec3 diffuseCol, in vec3 specularCol){

	//Vector to light
	vec3 lightDir = normalize(pLightPos - pos);

	float lambertian;
	float specular = lightCalc(lightDir, pos, nor, lambertian);

	//Light fades of in the distance:
	float fade_factor = max(1 - (distance(pos, pLightPos) / pLight_Fade), 0);

	return (diffuseCol * pLightDiffCol * lambertian +		//Diffuse calculation
	 diffuseCol * pLightspecCol * specularCol * specular)	//Specular calculation
	 * fade_factor;											//Light fade off
}
/*	BlinnPhong specular calculation:
*/
float lightCalc(in vec3 lightDir, in vec3 pos, in vec3 nor, out float lambertian){

	lambertian = max(dot(lightDir,nor), 0.0f);

	if(lambertian > 0.0f) {

		vec3 viewDir = normalize(-pos);

		// Blinn-Phong calculation:
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, nor), 0.0f);
		return pow(specAngle, 40.0f);
	}
	return 0.0f;
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
