#include "Sound/SoundManager.h"

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}
SoundManager::SoundManager()
{
    _engine = irrklang::createIrrKlangDevice();
    if (!_engine)
    {
        _engine->drop();
		_engine = nullptr;
    }
}
SoundManager::~SoundManager()
{
    if (_engine)
    {
        _engine->drop();
		_engine = nullptr;
    }
}

Sound *SoundManager::play2DSound(std::string path, bool looped, bool play)
{
    if (!_engine)
        return new Sound(nullptr);
    return new Sound(_engine->play2D(path.c_str(), looped, !play, true));
}
Sound *SoundManager::play3DSound(std::string path, glm::vec3 pos, bool looped, bool play)
{
    if (!_engine)
        return new Sound(nullptr);
    return new Sound(_engine->play3D(path.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), looped, !play, true));
}
void SoundManager::setListenerPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 up)
{
    if (_engine)
        _engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), -irrklang::vec3df(lookDir.x, lookDir.y, lookDir.z), irrklang::vec3df(velocity.x, velocity.y, velocity.z), irrklang::vec3df(up.x, up.y, up.z));
}
void SoundManager::pauseAllSounds(bool pause)
{
    if (_engine)
        _engine->setAllSoundsPaused(pause);
}
// void SoundManager::PlaySource3DSound(ISoundSource * source, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, glm::vec3 velocity)
// {
// 	if (!_engine)
// 		return;
//
// 	if (!_sound || _sound->isFinished())
// 	{
// 		origin = glm::vec3(origin - listenerPos);
// 		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(velocity.x, velocity.y, velocity.z), vec3df(up.x, up.y, up.z));
// 		_sound = _engine->play3D(source, vec3df(origin.x, origin.y, origin.z), false, true, true, true);
// 	}
// }
