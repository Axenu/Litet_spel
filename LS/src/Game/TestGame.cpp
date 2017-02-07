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

	_wallMesh = _gridtest.generateMesh();
	player = new Character(glm::vec3(3.0f, 0.8f, 5.0f), &_event);
	player->setLevel(&_gridtest);
	player->setCamera(&_camera);
	player->setScene(&_scene);
	_camera.setParent(player);

	MeshPart guardModelMeshPart(&_cube, &_material);
	Model guardModel(guardModelMeshPart);
	MeshPart goModelMeshPart(&_wallMesh, &_material);
	Model goModel(goModelMeshPart);
	//Add some more game objects
	_scene.add(player);
	_scene.add(new Guard(guardModel, &_gridtest));
	_scene.add(new GameObject(goModel));
	//Add some lights
	_scene.add(new PointLightObject(PointLight(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f), player));
	_scene.add(new PointLightObject(PointLight(glm::vec3(4.0f, 1.0f, 5.0f), glm::vec3(0.8f, 0.3f, 0.3f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));
	//Add loot
	//_lootMat.setColor("diffuse", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//_lootMat.setColor("spec", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//MeshPart *meshPart = new MeshPart(&_cube, &_lootMat);
	//Model *model = new Model(*meshPart);
	//LootObject *loot1 = new LootObject(*model);
	//LootObject *loot2 = new LootObject(*model, 100);

	//loot1->setPosition(glm::vec3(5, 0.5, 3));
	//loot2->setPosition(glm::vec3(5, 0.5, 5));

	//_scene.add(loot1);
	//_scene.add(loot2);
	std::vector<glm::vec3>* pLootPosList = _gridtest.getLootLocations();
	Model* tmpModel = _modelLoader.GetModel("Resources/cube.obj", &_shader);
	for (unsigned int i = 0; i < pLootPosList->size(); i++)
	{
		LootObject *tmpLoot = new LootObject(*tmpModel);
		tmpLoot->setPosition((*pLootPosList)[i]);
		tmpLoot->setY(1.0f);
		_scene.add(tmpLoot);
	}
}


void TestGame::compose(RenderInfo &rI) {
	if (_renderBuffer._active)
		_bufferRenderer.render(rI);
	else
		Game::compose(rI);
}