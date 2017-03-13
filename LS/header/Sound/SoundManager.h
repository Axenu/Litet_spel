#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <irrklang/irrKlang.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Sound/Sound.h"

class SoundManager
{
private:
    irrklang::ISoundEngine *_engine;
    bool _hasSound = true;
    std::vector<Sound *> _sounds;
    // std::map<std::string, irrklang::ISoundSource*> _sources;
public:
    static SoundManager& getInstance();
    SoundManager();
    ~SoundManager();
    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&)  = delete;

    Sound *play2DSound(std::string path, bool looped, bool play);
    Sound *play3DSound(std::string path, glm::vec3 pos, bool looped, bool play);
    void setListenerPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 up);
    void pauseAllSounds(bool pause);
    void setGlobalVolume(float vol);
    void setHasSound(bool sound);

};
