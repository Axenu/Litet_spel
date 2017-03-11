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
#include "IObjectFactory.h"

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
	std::vector<glm::ivec2> walkingPoints;
	int walkType;
	glm::ivec2 spawnPosition, face;
	PointLightValue _light;
	guardData(glm::ivec2 spawn, glm::ivec2 face, unsigned int walkType, PointLightValue light)
		: walkingPoints(), walkType(walkType), spawnPosition(spawn), face(face), _light(light)
	{
	}
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
	: public IObjectFactory
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
	unsigned int _guardCount = 0;
	glm::vec3 calcPos(glm::ivec2 square, const AABB &box);
	glm::vec3 calcRot(glm::ivec2 square);

public:
	void createRandomLoot(std::vector<lootData> loot, int totalValue);
	ObjectFactory(EventManager *events, const std::string &resourcePath = "", std::string modelPath = "");
	~ObjectFactory();

	/* Preload a model not created at start
	*/
	void preLoadModel(const std::string &model);

	std::unique_ptr<Scene> createLevel(const std::string &level, Level *&outLevel);

	virtual LootObject* createLoot(const std::string &model, glm::ivec2 square, glm::vec3 rotation, int value);
	virtual PointLightObject* createLight(PointLightValue light, Node *parent = nullptr);
	/* Create a scene object
		model << Model to load
	*/
	GameObject* createObject(const std::string &model, glm::ivec2 square, glm::vec3 rotation, enum gridType type, glm::vec3 positionOffset);

	Character* createCharacter(glm::ivec2 square, float height);
	virtual AntiLightGrenade* createLightGrenade(const std::string &model, glm::vec3 pos, glm::vec3 dir);
	Guard* createGuard(const std::string &model, Character& player, guardData& info);
	Door* CreateDoor(const std::string &model, glm::ivec2 square, glm::vec3 rotation);
	void loadSceneFromFile(std::string path, std::vector<guardData> &guards, std::vector<lootData> &loot, std::vector<doorData> &doorList);
	MeshShader& getShader();
};
