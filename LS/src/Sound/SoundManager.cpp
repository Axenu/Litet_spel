#include "Sound/SoundManager.h"
/*
Sound::Sound()
{
	_engine = createIrrKlangDevice();

	if (!_engine)
		return;

	_sound = nullptr;
}
*/
Sound::~Sound()
{
	if (_engine)
	{
		_engine->drop();
		_engine = nullptr;
	}

	if (_sound)
	{
		_sound->drop();
		_sound = nullptr;
	}
	//Unlisten to events
	_eventManager->unlisten(this, &Sound::PlaySource3DSoundevent);
}

Sound::Sound(EventManager * eventManager)
{
	_engine = createIrrKlangDevice();

	if (!_engine)
		return;

	_sound = nullptr;
	_eventManager = eventManager;
	//Listen to events
	_eventManager->listen(this, &Sound::PlaySource3DSoundevent);
}

void Sound::PlaySource2DSound(ISoundSource* source, bool loop)
{
	_engine->play2D(source, loop);
}

void Sound::PlaySource3DSoundevent(const ThreeDSoundEvent & event)
{
	PlaySource3DSound(GetSound( event._filename), event._loop, event._listenerPos, event._origin, event._lookDir, event._up, event._dt, event._update);
}

void Sound::PlaySource3DSound(ISoundSource * source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt, bool update)
{
	origin = glm::vec3(origin - listenerPos);

	if (_sound == nullptr || _sound->isFinished())
	{
		_sound = _engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, true, true);
	}
	else
	{
		_sound->setIsPaused(update);
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(0.0f, 0.0f, 0.0f), vec3df(up.x, up.y, up.z));
	}

}

void Sound::PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up)
{
	origin = glm::vec3(origin - listenerPos);

	_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(0.0f, 0.0f, 0.0f), vec3df(up.x, up.y, up.z));
	_engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, false, false);
}

ISoundSource* Sound::GetSound(char* filename)
{
	return _engine->getSoundSource(filename);
}

