#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Game/Scene/Scene.h"
#include "Render/Mesh/ModelLoader.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "Render/Mesh/Shader/DefSkinnedMeshShader.h"
#include "Game/Level/Level.h"
#include "Game/Objects/LootObject.h"
#include "Game/Objects/PointLightObject.h"
#include "Game/Objects/Character.h"
#include "Game/Objects/Guard.h"
#include "Game/Objects/AntiLightGrenade.h"
#include "Game/Objects/Door.h"


struct worldData {
	worldData(glm::ivec2 pos, glm::vec3 rotation)
	{
		this->pos = pos;
		this->rotation = rotation;
	}
	worldData() {};
	glm::ivec2 pos;
	glm::vec3 rotation;
};

struct guardData {
	std::vector<glm::vec2> walkingPoints;
	glm::ivec2 spawnPosition;
};

struct lootData : worldData {
	lootData() {};
	lootData(glm::ivec2 pos, glm::vec3 rotation, std::string modelName, int value)
	{
		this->pos = pos;
		this->rotation = rotation;
		this->modelName = modelName;
		this->value = value;
	};
	std::string modelName;
	int value;
};

struct doorData : worldData {
	doorData() {};
	doorData(glm::ivec2 pos, glm::vec3 rotation, bool open)
	{
		this->pos = pos;
		this->rotation = rotation;
		this->open = open;
	}
	bool open;
};

/* Factory creating game objects
*/
class ObjectFactory
{
private:
	/* Resource path */
	std::string _path, _modelPath;
	/* The mesh shader of the game.
	*/
	DeferredMeshShader _meshShader;
	DefSkinnedMeshShader _skinnedShader;
	ModelLoader _models;
	Scene* _scene;
	EventManager* _eventManager;
	Level* _level;
	glm::vec3 calcPos(glm::ivec2 square, const AABB &box);
	glm::vec3 calcRot(glm::ivec2 square);

public:
	ObjectFactory(EventManager *events, const std::string &resourcePath = "", std::string modelPath = "");
	~ObjectFactory();

	std::unique_ptr<Scene> createLevel(const std::string &level, Level *&outLevel);
	Character* createCharacter(glm::ivec2 square, float height);
	Character* createCharacter(glm::ivec2 square, float height, std::vector<AntiLightGrenade *>grenade);
	Guard* createGuard(const std::string &model, glm::ivec2 square, Character& player, std::vector<glm::vec2>& walkingPoints);
	AntiLightGrenade* createAntiLightGrenade(const std::string &model, glm::ivec2 square);
	Door* CreateDoor(const std::string &model, glm::ivec2 square, glm::vec3 rotation);
	/* Create a scene object
	model << Model to load
	*/
	GameObject* createObject(const std::string &model, glm::ivec2 square, glm::vec3 rotation, enum gridType type, glm::vec3 positionOffset);
	PointLightObject* createLight(PointLight light, glm::vec3 position);
	LootObject* createLoot(const std::string &model, glm::ivec2 square, glm::vec3 rotation, int value);
	PointLightObject* createLight(PointLight light, Node *parent = nullptr);
	void loadSceneFromFile(std::string path, std::vector<guardData> &guards, std::vector<lootData> &loot, std::vector<doorData> &doorList);
	MeshShader& getShader();
};
