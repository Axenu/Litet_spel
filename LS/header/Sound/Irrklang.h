#pragma once

#include <conio.h>
#include <stdlib.h>
#include "../include/irrklang/irrKlang.h"
#include <iostream>
#include <cstring>
#include <string.h>
#include <glm/glm.hpp>
#pragma warning(disable:4996)
class IrrKlang
{
public:
	IrrKlang();
	~IrrKlang();
	void PlaySource2DSound(std::string fileName,bool looping); //FileName is the name of the file in resources, Looping is if it should loop or not
	void PlaySource3DSound(std::string fileName, bool looping, glm::vec3 fromPlayerToGuard);
private:
	irrklang::ISoundEngine* engine;
};