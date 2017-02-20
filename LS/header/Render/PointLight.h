#pragma once
#define GLM_FORCE_RADIANS
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>

/* Struct holding the point light data
*/
struct PointLight {
	glm::vec3 _pos;
	/* Fade of distance of the lights strength. Padding pos as vec4.
	*/
	float _fadeDist;
	glm::vec4 _diffuse, _specular;
	glm::mat4 _shadowMatrices[6];
	GLfloat _shadowAspect;
	glm::mat4 _shadowProj;
	PointLight()
		: PointLight(glm::vec3(0.0f), glm::vec3(0.8f), glm::vec3(1.0f), 5.0f)
	{

	}
	PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist)
		: _pos(pos), _fadeDist(fadeDist), _diffuse(diffuse, 0.0f), _specular(specular, 0.0f), _shadowAspect(1.0f)
	{
		_shadowProj = glm::perspective(glm::radians(90.0f), _shadowAspect, 0.0f, _fadeDist);
		updateMatrices();
	}
	void updateMatrices()
	{
		_shadowMatrices[0] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0));
		_shadowMatrices[1] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0));
		_shadowMatrices[2] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0));
		_shadowMatrices[3] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0));
		_shadowMatrices[4] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0));
		_shadowMatrices[5] = _shadowProj * glm::lookAt(_pos, _pos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0));
	}
};
