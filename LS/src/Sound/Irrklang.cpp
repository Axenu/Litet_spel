#include "Sound/Irrklang.h"

Sound::Sound()
{
	_engine = createIrrKlangDevice();

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
	if (!_engine)
		return;

	_engine->play2D(source, loop);
}

void Sound::PlaySource3DSound(ISoundSource * source, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, glm::vec3 velocity)
{
	if (!_engine)
		return;

	if (!_sound || _sound->isFinished()) 
	{
		origin = glm::vec3(origin - listenerPos);
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(velocity.x, velocity.y, velocity.z), vec3df(up.x, up.y, up.z));
		_sound = _engine->play3D(source, vec3df(origin.x, origin.y, origin.z), false, true, true, true);
	}
}

void Sound::Update()
{
	if (_sound)
	{
		_sound->setIsPaused(false);
		_engine->update();
	}
}

void Sound::Pause()
{
	if (_sound)
		_sound->setIsPaused(true);
}

void Sound::SetMasterVolume(float volume)
{
	if (!_engine)
		return;

	_engine->setSoundVolume(volume);
}

void Sound::SetVolume(float volume)
{
	if (_sound)
		_sound->setVolume(volume);
}

ISoundSource* Sound::GetSound(char* filename)
{
	if (!_engine)
		return nullptr;
		
	return _engine->getSoundSource(filename);
}

