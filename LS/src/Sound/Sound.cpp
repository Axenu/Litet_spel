#include "Sound/Sound.h"


Sound::Sound(irrklang::ISound *sound) : _sound(sound)
{

}
Sound::~Sound()
{

}
void Sound::pause()
{
    if (_sound)
        _sound->setIsPaused(true);
}
void Sound::play()
{
    if (_sound)
        _sound->setIsPaused(false);
}
void Sound::setPlayPosition(int pos)
{
    if (_sound)
        _sound->setPlayPosition(pos);
}
void Sound::setVolume(float vol)
{
    if (_sound)
        _sound->setVolume(vol);
}
void Sound::drop()
{
    if (_sound)
    {
        _sound->drop();
        _sound = nullptr;
    }
}
void Sound::setPosition(glm::vec3 pos)
{
    irrklang::vec3df posSound(pos.x, pos.y, pos.z);
    if (_sound)
        _sound->setPosition(posSound);
}
