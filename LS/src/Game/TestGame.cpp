#include "Game/TestGame.h"




TestGame::TestGame(Setting &setting, EventManager &events)
	: Game(setting, events), _bufferRenderer(_resource.getQuad(), events), _renderBuffer(events, GLFW_KEY_R) {

	_bufferRenderer.setWindowSize((float)setting.Width(), (float)setting.Height(), _scene.getCamera());
}
TestGame::~TestGame() {

}

void TestGame::initiate() {

	Level* level = _factory.createLevel();
	Character* player = _factory.createCharacter(glm::ivec2(3, 5), 1.3f);
	_factory.createGuard("cube.obj", glm::ivec2(2, 2), *player);
	_factory.createObject("Table.obj", glm::ivec2(3, 5));
	//Add some lights
	PointLight l(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
	_factory.createLight(l, player);
	l = PointLight(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 3.0f);
	_factory.createLight(l, glm::ivec2(4, 5));
	_factory.createLight(l, glm::ivec2(12, 7));
	_factory.createLight(l, glm::ivec2(5, 15));
	_factory.createLight(l, glm::ivec2(25, 7));
	//Add some loot
	std::vector<glm::vec3>* pLootPosList = level->getGrid().getLootLocations();
	for (unsigned int i = 0; i < pLootPosList->size(); i++)
		GameObject *tmpLoot = _factory.createLoot("Chalice.obj", (*pLootPosList)[i] + glm::vec3(0.f, 1.f, 0.f));
}


void TestGame::compose(RenderInfo &rI) {
	if (_renderBuffer._active)
		_bufferRenderer.render(rI);
	else
		Game::compose(rI);
}
