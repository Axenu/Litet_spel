#pragma once
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
	PointLight() {}
	PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float fadeDist)
		: _pos(pos), _fadeDist(fadeDist), _diffuse(diffuse, 0.0f), _specular(specular, 0.0f){
	}
};