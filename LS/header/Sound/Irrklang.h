#pragma once

#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace irrklang;

static class Sound
{
public:
	Sound();
	~Sound();
	
	/*FileName is the name of the file in resources, Looping is if it should loop or not*/
	void PlaySource2DSound(ISoundSource* source, bool loop);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt,bool update);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt, bool update,float volume);
	ISoundSource* GetSound(char* filename);

private:
	ISoundEngine* _engine;
	ISound *_sound;

} sound;
