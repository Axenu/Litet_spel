#include "Factory.h"

Factory& Factory::getInstance()
{
    static Factory instance;
    return instance;
}

Factory::Factory()
{

}
Factory::~Factory()
{
    for (std::map<std::string,Shader*>::iterator it = _shaders.begin(); it != _shaders.end(); it++)
    {
        delete it->second;
    }
    for (std::map<std::string,gui::Font*>::iterator it = _fonts.begin(); it != _fonts.end(); it++)
    {
        delete it->second;
    }
}
Shader* Factory::getShader(std::string name)
{
    return getShader(name, name);
}
Shader* Factory::getShader(std::string vert, std::string frag)
{
    std::string name = vert+frag;
    std::map<std::string,Shader*>::iterator it = _shaders.find(name);
    if (it != _shaders.end())
    {
        return it->second;
    }
    Shader *s = new Shader(vert, frag);
    _shaders[name] = s;
    return s;
}
gui::Font *Factory::getFont(std::string path)
{
    std::map<std::string,gui::Font*>::iterator it = _fonts.find(path);
    if (it != _fonts.end())
    {
        return it->second;
    }
    gui::Font *f = new gui::Font(path);
    _fonts[path] = f;
    return f;
}
