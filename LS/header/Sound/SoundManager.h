#pragma once

#include <iostream>
#include <string>
#include <irrklang/irrKlang.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class SoundManager
{
private:
    irrklang::ISoundEngine *_engine;
public:
    static SoundManager& getInstance();
    SoundManager();
    ~SoundManager();
    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&)  = delete;

    // irrklang::ISound *generateSoundSource(std::string path);
    // irrklang::ISound *play2DSound(std::string path, bool looped);
    irrklang::ISound *play2DSound(std::string path, bool looped, bool play);
    irrklang::ISound *play3DSound(std::string path, glm::vec3 pos, bool looped, bool play);
    void setListenerPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 up);

    //playing Sounds
	// void PlaySource3DSound(ISoundSource* source, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, glm::vec3 velocity);
};
