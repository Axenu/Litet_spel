#include "Sound/Irrklang.h"

Sound::Sound()
{
	_engine = createIrrKlangDevice();

	if (!_engine)
		return;

	_sound = nullptr;
}

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
}

void Sound::PlaySource2DSound(ISoundSource* source, bool loop)
{
	_engine->play2D(source, loop);
}

void Sound::PlaySource3DSound(ISoundSource * source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt, bool update)
{
	origin = glm::vec3(origin - listenerPos);

	if (_sound == nullptr || _sound->isFinished())
	{
		_sound = _engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, true, true);
		source->setDefaultMinDistance(2.0f);
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

void Sound::PlaySource3DSound(ISoundSource * source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt, bool update, float volume)
{
	origin = glm::vec3(origin - listenerPos);

	if (_sound == nullptr || _sound->isFinished())
	{
		_sound = _engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, true, true);
		source->setDefaultMinDistance(2.0f);
	}
	else
	{
		_sound->setIsPaused(update);
		_sound->setVolume(volume);
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(0.0f, 0.0f, 0.0f), vec3df(up.x, up.y, up.z));
	}
}

ISoundSource* Sound::GetSound(char* filename)
{
	return _engine->getSoundSource(filename);
}

