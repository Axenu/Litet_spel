#pragma once

#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace irrklang;

static class IrrKlang
{
public:
	IrrKlang();
	~IrrKlang();
	
	/*FileName is the name of the file in resources, Looping is if it should loop or not*/
	void PlaySource2DSound(ISoundSource* source, bool loop);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up);
	ISoundSource* GetSound(char* filename);

private:

	ISoundEngine* _engine;
	float _timer;

} sound;
