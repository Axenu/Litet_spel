#pragma once
#define GLM_FORCE_RADIANS
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include "math/AABB.h"
#include "gl/glInclude.h"

/* Struct holding the point light lighting values
*/
struct PointLightValue
{
	glm::vec3 _pos;
	/* Fade of distance of the lights strength. Padding pos as vec4.
	*/
	float _fadeDist;
	glm::vec4 _diffuse, _specular;
	PointLightValue();
	PointLightValue(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist);
};
/* Struct holding the point light data
*/
struct PointLight {
	PointLightValue _light;
	glm::mat4 _shadowMatrices[6];
	GLfloat _shadowAspect;
	glm::mat4 _shadowProj;
	PointLight();
	PointLight(PointLightValue light);
	PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist);
	void updateMatrices();
	AABB generateAABB();
};
