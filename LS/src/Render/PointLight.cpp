#include "Render/PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

PointLightValue::PointLightValue()
: PointLightValue(glm::vec3(0.0f), glm::vec3(0.8f), glm::vec3(1.0f), 5.0f)
{

}
PointLightValue::PointLightValue(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist)
	: _pos(pos), _fadeDist(fadeDist), _diffuse(diffuse, 0.0f), _specular(specular, 0.0f)
{
}

PointLight::PointLight()
	: PointLight(PointLightValue(glm::vec3(0.0f), glm::vec3(0.8f), glm::vec3(1.0f), 5.0f))
{
}

PointLight::PointLight(PointLightValue light)
	: _light(light), _shadowAspect(1.0f)
{
	_shadowProj = glm::perspective(glm::radians(90.0f), _shadowAspect, 0.0f, _light._fadeDist);
	updateMatrices();
}
PointLight::PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist)
	: PointLight(PointLightValue(pos, diffuse, specular, fadeDist))
{
}
void PointLight::updateMatrices()
{
	_shadowMatrices[0] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	_shadowMatrices[1] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	_shadowMatrices[2] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	_shadowMatrices[3] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	_shadowMatrices[4] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	_shadowMatrices[5] = _shadowProj * glm::lookAt(_light._pos, _light._pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
}

AABB PointLight::generateAABB()
{
	return AABB(glm::vec3(-_light._fadeDist) + _light._pos, glm::vec3(_light._fadeDist) + _light._pos);
}