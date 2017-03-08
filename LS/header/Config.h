#pragma once
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

namespace Config
{
    extern bool hasShadowMap;
    extern int shadowMapSize;
    extern float masterVolume;
    extern bool hasSound;
    extern glm::ivec2 resolution;
    extern bool borderLess;
    extern bool showFPS;

    bool loadConfig(std::string path);
    bool saveConfig(std::string path);
}
