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
uniform uint GrenadeAmount;
uniform vec3 viewGrenadePosition[10];
uniform vec2 GrenadeExpansionFading[10];
uniform samplerCube shadowMap[8];

//X: 1 / width  Y : 1 / height
uniform vec2 screenInv;
//Near and far plane
uniform float near, far;
//Right X coord on nearplane, Top Y coord on nearplane
uniform float right, top;

uniform vec3 camPos;

uniform mat4 VM;

//Light components
uniform uint pNumLight;
uniform vec3 pLightPos[8];
uniform vec3 pLightDif[8];
uniform vec3 pLightSpec[8];
uniform float pLightFade[8];

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord();
vec3 positionFromDepth(in float depth, in vec2 frameCoord);
vec3 decodeNormal(in vec3 normal);
vec3 shadowTest(in uint i, in vec3 pos, in vec3 color);
vec3 pointLightCalc(in vec3 pos, in vec3 nor, in vec3 diffuseCol, in vec3 specularCol, in float shininess, in vec3 pLightPos, in float pLightFade, in vec3 pLightDif, in vec3 pLightSpec);
float lightCalc(in vec3 lightDir, in vec3 pos, in vec3 nor, in float shininess, out float lambertian);

vec3 antiLightGrenadeCal(in vec3 GrenadePosition,in vec3 pos,in vec3 diffuseCol,in vec3 color,in vec2 GrenadeExpansionFading);

/* Composition main function
*/
void main () {

	//Calculate frame texture coordinate:
	vec2 frameCoord = calcFrameCoord();

	vec3 color = texture(colBuffer, frameCoord).xyz;
	vec3 normal = texture(norBuffer, frameCoord).xyz;
	vec4 specular = texture(specBuffer, frameCoord);
	float depth  = texture(depthBuffer, frameCoord).x;

	normal = decodeNormal(normal);
	vec3 position = positionFromDepth(depth, frameCoord);

	//Calc light
	ColorOut = vec4(color * 0.2f, 1.0f); //Add ambient

	//player light
	ColorOut.xyz += pointLightCalc(position, normal, color, specular.xyz, specular.w, vec3(0,0,0), 2.5f, vec3(0.5,0.5,0.5), vec3(0,0,0));

	for(uint i = 0; i < pNumLight; i++)
		ColorOut.xyz += shadowTest(i, position, pointLightCalc(position, normal, color, specular.xyz, specular.w, pLightPos[i], pLightFade[i], pLightDif[i], pLightSpec[i]));

	for(uint i = 0;i<GrenadeAmount;i++)
	ColorOut.xyz=antiLightGrenadeCal(viewGrenadePosition[i],position,color,ColorOut.xyz,GrenadeExpansionFading[i]);
}

/*Calculate the frame coordinate. (Texture coordinate of the window)
return	>>	The texture coordinate of the window
*/
vec2 calcFrameCoord(){
	//Divide window coordinate by screen size => Range [0, 1]
	return vec2(gl_FragCoord.x * screenInv.x,
				gl_FragCoord.y * screenInv.y);
}

/*
AntiLightgrenade Setting the color to diffured in a set space
*/
vec3 antiLightGrenadeCal(in vec3 GrenadePosition,in vec3 pos,in vec3 diffuseCol,in vec3 color,in vec2 GrenadeExpansionFading)
{
	float Distance = length(GrenadePosition - pos);
	if(GrenadeExpansionFading.x>Distance)
	{
		return color * GrenadeExpansionFading.y;
	}
	return color;
}

vec3 shadowTest(in uint i, in vec3 pos, in vec3 color)
{
	//shadow
	vec4 lightPosView = VM * vec4(pLightPos[i], 1.0); //world position
	vec4 wPos = VM * (vec4(pos.xyz, 1.0));
	vec3 lightDirShadow = wPos.xyz - lightPosView.xyz;
	float f = texture(shadowMap[i], lightDirShadow).r;
	float currentDepth = length(lightDirShadow)/pLightFade[i];
	if (f + 0.005 > currentDepth)
	{
		return color;										//Light fade off
	}
	return vec3(0,0,0);
}

/*	Point light specular calculation:
*/
vec3 pointLightCalc(in vec3 pos, in vec3 nor, in vec3 diffuseCol, in vec3 specularCol, in float shininess, in vec3 pLightPos, in float pLightFade, in vec3 pLightDif, in vec3 pLightSpec)
{

	//Vector to light
	vec3 lightDir = normalize(pLightPos - pos);

	float lambertian;
	float specular = lightCalc(lightDir, pos, nor, shininess, lambertian);

	//Light fades of in the distance:
	float fade_factor = max(1 - (distance(pos, pLightPos) / pLightFade), 0);

	return (diffuseCol * pLightDif * lambertian +	//Diffuse calculation
	 pLightSpec * specularCol * specular)			//Specular calculation
	 * fade_factor;										//Light fade off

}
/*	BlinnPhong specular calculation:
*/
float lightCalc(in vec3 lightDir, in vec3 pos, in vec3 nor, in float shininess, out float lambertian){

	lambertian = max(dot(lightDir,nor), 0.0f);

	if(lambertian > 0.0f) {

		vec3 viewDir = normalize(-pos);

		// Blinn-Phong calculation:
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, nor), 0.0f);
		return pow(specAngle, shininess);
		// Phon shading:
		//vec3 reflectDir = reflect(-lightDir, nor);
		//float specAngle = max(dot(reflectDir, viewDir), 0.0f);
		//return pow(specAngle, shininess * 0.25f);
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
	ndc.y = ((frameCoord.y) * 2.0 - 1.0);


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
	// return vec3(ndc, 1.0);
  return eye;
}
