#pragma once

#include <stdlib.h>
#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#pragma warning(disable:4996)

using namespace irrklang;

class Sound
{
public:
	Sound(char* filename);
	~Sound();
	
	/*FileName is the name of the file in resources, Looping is if it should loop or not*/
	void PlaySource2DSound(std::string fileName,bool looping);
	void PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 playerPos, glm::vec3 objectPos, glm::vec3 lookDir, glm::vec3 up, float dt);
	ISoundSource* GetSoundSource(char* filename);

private:

	ISoundEngine* _engine;
	std::vector<ISoundSource*> _sources;
	float _timer;
};
