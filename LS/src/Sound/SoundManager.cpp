#include "Sound/SoundManager.h"

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}
SoundManager::SoundManager()
{
    _engine = irrklang::createIrrKlangDevice();
}
SoundManager::~SoundManager()
{
    if (_engine)
    {
        _engine->drop();
		_engine = nullptr;
    }
    for (size_t i = 0; i < _sounds.size(); i++)
    {
        _sounds[i]->drop();
        delete _sounds[i];
    }
}

Sound *SoundManager::play2DSound(std::string path, bool looped, bool play)
{
    Sound *s = nullptr;
    if (!_engine || !_hasSound)
        s = new Sound(nullptr);
    else
        s = new Sound(_engine->play2D(path.c_str(), looped, !play, true));
    _sounds.push_back(s);
    return s;
}
Sound *SoundManager::play3DSound(std::string path, glm::vec3 pos, bool looped, bool play)
{
    Sound *s = nullptr;
    if (!_engine || !_hasSound)
        s = new Sound(nullptr);
    else
        s = new Sound(_engine->play3D(path.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), looped, !play, true));
    _sounds.push_back(s);
    return s;
}
void SoundManager::setListenerPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 up)
{
    if (_engine && _hasSound)
        _engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), -irrklang::vec3df(lookDir.x, lookDir.y, lookDir.z), irrklang::vec3df(velocity.x, velocity.y, velocity.z), irrklang::vec3df(up.x, up.y, up.z));
}
void SoundManager::pauseAllSounds(bool pause)
{
    if (_engine && _hasSound)
        _engine->setAllSoundsPaused(pause);
}
void SoundManager::setGlobalVolume(float vol)
{
    if (_engine && _hasSound)
        _engine->setSoundVolume(vol);
}
void SoundManager::setHasSound(bool sound)
{
    _hasSound = sound;
    if (!_hasSound)
        pauseAllSounds(true);
}
