#pragma once

#include <stdlib.h>
#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

#pragma warning(disable:4996)

using namespace irrklang;

class IrrKlang
{
public:
	IrrKlang();
	~IrrKlang();
	
	/*FileName is the name of the file in resources, Looping is if it should loop or not*/
	void PlaySource2DSound(std::string fileName,bool looping);
	void PlaySource3DSound(std::string fileName, bool looping, glm::vec3 playerPos, glm::vec3 objectPos, glm::vec3 lookDir,float dt);
	void PlaySource3DSoundOnce(std::string fileName, bool looping, glm::vec3 playerPos, glm::vec3 objectPos, glm::vec3 lookDir);
	void update(glm::vec3 lookDir);
private:
	ISoundEngine* _engine;
	ISound* _music;
	bool newSound;
	float _timer;
};
