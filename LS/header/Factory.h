#pragma once

#include <map>
#include "Render/Shader.h"

class Factory
{
public:
    static Factory& getInstance();
    Factory();
    ~Factory();
    Factory(Factory const&) = delete;
    void operator=(Factory const&)  = delete;

    Shader* getShader(std::string name);
    Shader* getShader(std::string vert, std::string frag);

private:
    std::map<std::string, Shader*> _shaders;
};
