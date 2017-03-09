#pragma once

#include <iostream>
#include <string>
#include <irrklang/irrKlang.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Sound
{
private:
    irrklang::ISound *_sound;
public:
    Sound(irrklang::ISound *sound);
    ~Sound();

    void pause();
    void play();
    void setPlayPosition(int pos);
    void setVolume(float vol);
    void drop();
    void setPosition(glm::vec3 pos);
};
