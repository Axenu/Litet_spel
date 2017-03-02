#include "Sound/Irrklang.h"

IrrKlang::IrrKlang()
{
	_engine = createIrrKlangDevice();
	if (!_engine)
		return;

	_timer = 0.f;
	newSound = true;
}

IrrKlang::~IrrKlang()
{
	if (_engine)
		_engine->drop();
}

void IrrKlang::PlaySource2DSound(std::string fileName,bool looping)
{
	fileName = "Resources/" + fileName;
	char * test = new char[fileName.length() + sizeof("Resources/")];
	std::strcpy(test, fileName.c_str());
	_engine->play2D(test, looping);
	
	delete test;
	test = nullptr;
}

void IrrKlang::PlaySource3DSound(std::string fileName, bool looping, glm::vec3 playerPos, glm::vec3 objectPos, glm::vec3 lookDir,float dt)
{
	if (newSound == true)
	{
		fileName = "Resources/" + fileName;
		char * test = new char[fileName.length() + sizeof("Resources/")];
		std::strcpy(test, fileName.c_str());

		objectPos = glm::vec3(objectPos - playerPos);

	
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z));
		_music = _engine->play3D(test, vec3df(objectPos.x, objectPos.y, objectPos.z), looping, false, true, ESM_AUTO_DETECT, true);
		delete test;
		test = nullptr;
		newSound = false;
	}
	std::cout << _music->getPlayLength() << std::endl;
	if (_timer < (_music->getPlayLength()/1000.f) && newSound == false)
	{
		update(lookDir);		
	}
	else
	{
		newSound = true;
		_timer = 0;
	}
	_timer += dt;
}

void IrrKlang::PlaySource3DSoundOnce(std::string fileName, bool looping, glm::vec3 playerPos, glm::vec3 objectPos, glm::vec3 lookDir)
{
	fileName = "Resources/" + fileName;
	char * test = new char[fileName.length() + sizeof("Resources/")];
	std::strcpy(test, fileName.c_str());

	objectPos = glm::vec3(objectPos - playerPos);


	_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z));
	_music = _engine->play3D(test, vec3df(objectPos.x, objectPos.y, objectPos.z), looping, false, true, ESM_AUTO_DETECT, true);
	delete test;
	test = nullptr;
	newSound = false;
}

void IrrKlang::update(glm::vec3 lookDir)
{
	if(_music != nullptr)
	{
		_music->setIsPaused(true);
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z));
		_music->setIsPaused(false);
	}
}
