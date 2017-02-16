#include "Game/Level/ObjectFactory.h"



ObjectFactory::ObjectFactory(Scene &scene, EventManager &events, const std::string &resourcePath)
	: _path(resourcePath), _meshShader(), _models(), _scene(scene), _events(events)
{
}

ObjectFactory::~ObjectFactory()
{}

glm::vec3 ObjectFactory::calcPos(glm::ivec2 square, const AABB &box) {
	glm::vec3 vec = _level->getGrid().getCenter(square);
	//Move so object is centered on top of the square.
	vec.y -= box.getMin().y;
	return vec;
}

Level* ObjectFactory::createLevel(const std::string &level) {
	_level = new Level(_path + level, _events, _meshShader);
	//Updates to initiate aabb
	_level->update(0.0f);
	_scene.initQuadTree(_level->getAABB());
	_scene.add(_level, false);
	return _level;
}
Character* ObjectFactory::createCharacter(glm::ivec2 square, float height) {

	Character* player = new Character(_level->getGrid().getCenter(square) + glm::vec3 (0.f, height, 0.f), &_events);
	player->setLevel(&_level->getGrid());
	player->setScene(&_scene);
	_scene.getCamera().setParent(player);
	_scene.add(player, true);
	return player;
}
Guard* ObjectFactory::createGuard(const std::string &model, glm::ivec2 square, Character& player) {
	Material mat(&_meshShader);
	mat.setColor("diffuse", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Model tmpModel = _models.GetModel(_path + model, mat);
	Guard* guard = new Guard(&player, &_events, tmpModel, &_level->getGrid());
	guard->setLevel(&_level->getGrid());
	guard->StartGridBuild();
	guard->setScale(0.00625f);
	_scene.add(guard, true);

	return guard;
}


GameObject* ObjectFactory::createObject(const std::string &model, glm::ivec2 square) {
	Model tmpModel = _models.GetModel(_path + model, &_meshShader);
	GameObject* object = new GameObject(tmpModel, type::Doodad);
	object->setPosition(calcPos(square, tmpModel.getBox()));
	_scene.add(object, false);
	return object;
}
LootObject* ObjectFactory::createLoot(const std::string &model, glm::vec3 pos) {
	Material tmpMat(&_meshShader);
	tmpMat.setColor("diffuse", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	tmpMat.setColor("spec", glm::vec4(1.0f));
	tmpMat.setFloat("shine", 20.0f);
	Model tmpModel = _models.GetModel(_path + model, &_meshShader);
	LootObject* object = new LootObject(tmpModel, type::Doodad);
	object->setPosition(pos);
	object->init();
	_scene.add(object, false);
	return object;
}
PointLightObject* ObjectFactory::createLight(PointLight light, glm::ivec2 square) {
	AABB box;
	light._pos += calcPos(square, box);
	PointLightObject* object = new PointLightObject(light, nullptr);
	object->update(0.0f);
	_scene.add(object, false);
	return object;
}
PointLightObject* ObjectFactory::createLight(PointLight light, Node *parent) {
	PointLightObject* object = new PointLightObject(light, parent);
	_scene.add(object, true);
	return object;
}


MeshShader& ObjectFactory::getShader() {
	return _meshShader;
}
