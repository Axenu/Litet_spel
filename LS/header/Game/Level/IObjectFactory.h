#pragma once
#include "Game/Objects/LootObject.h"
#include "Game/Objects/PointLightObject.h"
#include "Game/Objects/AntiLightGrenade.h"

class IObjectFactory
{
public:
	/* Create a scene object
	model << Model to load
	*/
	virtual GameObject* createObject(const std::string &model, glm::ivec2 square, glm::vec3 rotation, enum gridType type, glm::vec3 positionOffset) = 0;
	virtual LootObject* createLoot(const std::string &model, glm::ivec2 square, glm::vec3 rotation, int value) = 0;
	virtual PointLightObject* createLight(PointLightValue light, Node *parent = nullptr) = 0;
	/* Create an anti light grenade */
	virtual AntiLightGrenade* createLightGrenade(const std::string &model, glm::vec3 pos, glm::vec3 dir) = 0;
};