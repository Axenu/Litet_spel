#pragma once

#include <map>
#include "Render/Shader.h"
#include "gui/Font.h"

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
    gui::Font *getFont(std::string path);

private:
    std::map<std::string, Shader*> _shaders;
    std::map<std::string, gui::Font*> _fonts;
};
