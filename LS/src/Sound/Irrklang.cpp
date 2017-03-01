#include "Sound/Irrklang.h"

IrrKlang::IrrKlang()
{
	engine = irrklang::createIrrKlangDevice();
	if (!engine)
		return;
}

IrrKlang::~IrrKlang()
{


	
}

void IrrKlang::PlaySource2DSound(std::string fileName,bool looping)
{
	fileName = "Resources/" + fileName;
	char * test = new char[fileName.length() + sizeof("Resources/")];
	std::strcpy(test, fileName.c_str());
engine->play2D(test, looping);
}

void IrrKlang::PlaySource3DSound(std::string fileName, bool looping, glm::vec3 fromPlayerToGuard) //under working condition
{
	fileName = "Resources/" + fileName;
	char * test = new char[fileName.length() + sizeof("Resources/")];
	std::strcpy(test, fileName.c_str());
//	engine->play3D()
}
