#include "Game/TestGame.h"

TestGame::TestGame(Setting &setting, EventManager &events)
	: Game(setting, events), _bufferRenderer(_resource.getQuad(), events), _renderBuffer(events, GLFW_KEY_R),
	  _material(&_shader), _lootMat(&_shader) {

	_bufferRenderer.setWindowSize((float)setting.Width(), (float)setting.Height(), _camera);
}
TestGame::~TestGame() {

}

void TestGame::initiate() {
	_material.setColor("diffuse", glm::vec4(0.8f));
	_material.setColor("spec", glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	_material.setFloat("shine", 20.f);

	_level = new Level(_event, _shader);
	_scene.add(_level);

	_player = new Character(glm::vec3(3.0f, 0.8f, 5.0f), &_event);
	_player->setLevel(&_level->getGrid());
	_player->setCamera(&_camera);
	_player->setScene(&_scene);
	_camera.setParent(_player);

	ModelPart guardModelMeshPart(&_cube, _material);
	Model guardModel(guardModelMeshPart);
	
	//Add some more game objects
	_scene.add(_player);
	_scene.add(new Guard(_player, &_event, guardModel, &_level->getGrid()));
	//Add some lights
	_scene.add(new PointLightObject(PointLight(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f), _player));
	_scene.add(new PointLightObject(PointLight(glm::vec3(4.0f, 1.0f, 5.0f), glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));
	_scene.add(new PointLightObject(PointLight(glm::vec3(12.0f, 1.0f, 7.0f), glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));
	_scene.add(new PointLightObject(PointLight(glm::vec3(5.0f, 1.0f, 15.0f), glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));
	_scene.add(new PointLightObject(PointLight(glm::vec3(25.0f, 1.0f, 7.0f), glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));
	
	//Add loot
	std::vector<glm::vec3>* pLootPosList = _level->getGrid().getLootLocations();
	Material tmpMat(&_shader);
	tmpMat.setColor("diffuse", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	tmpMat.setColor("spec", glm::vec4(1.0f));
	tmpMat.setFloat("shine", 20.0f);
	Model tmpModel = _modelLoader.GetModel("Resources/cube.obj", tmpMat);
	for (unsigned int i = 0; i < pLootPosList->size(); i++)
	{
		LootObject *tmpLoot = new LootObject(tmpModel);
		tmpLoot->setPosition((*pLootPosList)[i]);
		tmpLoot->setY(1.0f);
		tmpLoot->setScale(0.5f);
		_scene.add(tmpLoot);
	}
}


void TestGame::compose(RenderInfo &rI) {
	if (_renderBuffer._active)
		_bufferRenderer.render(rI);
	else
		Game::compose(rI);
}

Character * TestGame::getCharacter()
{
	return _player;
}
