#include "Config.h"
namespace Config
{
    bool hasShadowMap = true;
    int shadowMapSize = 512;
    float masterVolume = 1.0f;
    bool hasSound = true;
    glm::ivec2 resolution = glm::ivec2(1280, 720);
    bool borderLess = false;
    bool showFPS = true;

    bool loadConfig(std::string path)
    {
        std::ifstream confFile;
		confFile.open(path);
		if (!confFile.is_open()) {
			std::cout << "Config file: " + path + " could not be found";
			return false;
		}
        std::string line;

        while(!confFile.eof())
		{
			std::stringstream lineStream;
			std::getline( confFile, line );
			lineStream << line;

            std::string varName;
            lineStream >> varName;
            if(varName == "resolution")
            {
                lineStream >> resolution.x >> resolution.y;
            }
            else if (varName == "hasSound")
            {
                lineStream >> hasSound;
            }
            else if (varName == "masterVolume")
            {
                lineStream >> masterVolume;
            }
            else if (varName == "hasShadowMap")
            {
                lineStream >> hasShadowMap;
            }
            else if (varName == "shadowMapSize")
            {
                lineStream >> shadowMapSize;
            }
            else if (varName == "borderLess")
            {
                lineStream >> borderLess;
            }
            else if (varName == "showFPS")
            {
                lineStream >> showFPS;
            }
            else
            {
                std::cout << "Var not found in config: " << varName << std::endl;
            }
        }
        return true;
    }
    bool saveConfig(std::string path)
    {
        std::ofstream confFile;
		confFile.open(path, std::ios::trunc);
		if (!confFile.is_open()) {
			std::cout << "Config file: " + path + " could not be found" << std::endl;
			return false;
		}
        confFile << "resolution " << resolution.x << " " << resolution.y << std::endl;
        confFile << "hasSound " << hasSound << std::endl;
        confFile << "masterVolume " << masterVolume << std::endl;
        confFile << "hasShadowMap " << hasShadowMap << std::endl;
        confFile << "shadowMapSize " << shadowMapSize << std::endl;
        confFile << "borderLess " << borderLess << std::endl;
        confFile << "showFPS " << showFPS << std::endl;
        return true;
    }
}
