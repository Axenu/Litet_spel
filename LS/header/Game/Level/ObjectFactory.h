#pragma once
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


/* Factory creating game objects
*/
class ObjectFactory
{
private:
	/* Resource path */
	std::string _path;
	/* The mesh shader of the game.
	*/
	DeferredMeshShader _meshShader;
	DefSkinnedMeshShader _skinnedShader;
	ModelLoader _models;
	Scene& _scene;
	EventManager& _events;
	Level* _level;

	glm::vec3 calcPos(glm::ivec2 square, const AABB &box);

public:
	ObjectFactory(Scene &scene, EventManager &events, const std::string &resourcePath = "");
	~ObjectFactory();

	Level* createLevel(const std::string &level);
	Character* createCharacter(glm::ivec2 square, float height);
	Character* createCharacter(glm::ivec2 square, float height, AntiLightGrenade & grenade);
	Guard* createGuard(const std::string &model, glm::ivec2 square, Character& player);
	AntiLightGrenade* createAntiLightGrenade(const std::string &model, glm::ivec2 square);
	GameObject* createObject(const std::string &model, glm::ivec2 square);
	LootObject* createLoot(const std::string &model, glm::vec3 pos);
	PointLightObject* createLight(PointLight light, glm::ivec2 square);
	PointLightObject* createLight(PointLight light, Node *parent = nullptr);
	MeshShader& getShader();
};

