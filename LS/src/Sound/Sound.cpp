#include "Sound/Sound.h"

Sound::Sound(char* filename)
{
	_engine = createIrrKlangDevice();
	if (!_engine)
		return;

	_timer = 0.0f;

	_sources.push_back(_engine->addSoundSourceFromFile(filename));
}

Sound::~Sound()
{
	if (_engine)
	{
		_engine->drop();
		_engine = nullptr;
	}
}

void Sound::PlaySource2DSound(std::string fileName, bool loop)
{
	fileName = "Resources/" + fileName;
	char * test = new char[fileName.length() + sizeof("Resources/")];
	std::strcpy(test, fileName.c_str());
	_engine->play2D(test, loop);
	
	delete test;
	test = nullptr;
}

void Sound::PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt)
{
	
	origin = glm::vec3(origin - listenerPos);
 
	if (_timer < (source->getPlayLength() / 1000.f))
	{
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z));
	}
	else
	{
		_engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, false, true);
		_timer = 0.0f;
	}

	_timer += dt;
}

ISoundSource* Sound::GetSoundSource(char* filename)
{
	return _sources[0];
}
