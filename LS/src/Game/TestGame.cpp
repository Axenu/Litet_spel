#include "Game/TestGame.h"




TestGame::TestGame(Setting &setting, EventManager &events)
	: Game(setting, events), _bufferRenderer(_resource.getQuad(), events), _cubeMapRenderer(_resource.getQuad(), events), _renderBufferKey(events, GLFW_KEY_R), _cubeMapBufferKey(events, GLFW_KEY_C)
{
	_bufferRenderer.setWindowSize((float)setting.Width(), (float)setting.Height(), _scene.getCamera());
}
TestGame::~TestGame()
{

}

void TestGame::setupRI(RenderInfo &rI) {
	rI._lightGrenadePos = _player->getGrenadeData()._grenadePositionWhenLanded;
	rI._lightGrenadeExpansionAndFading = glm::vec2(_player->getGrenadeData().expanding, _player->getGrenadeData().fading);
}
void TestGame::initiate() {

	Level* level = _factory.createLevel("Demo1.bmp");
	AntiLightGrenade* grenade = _factory.createAntiLightGrenade("models/cube.obj", glm::ivec2(2, 2));

	Character* player = _factory.createCharacter(glm::ivec2(3, 5), 1.3f, *grenade);
	_player = player;
	std::vector<glm::vec3>* pGuardPosList = level->getGrid().getGuardLocations();
	int sizesaved = pGuardPosList->size();
	for (int i = 0; i < sizesaved; i++)
		_factory.createGuard("models/Character.dae", glm::ivec2(5, 7), *player);

	_factory.loadSceneFromFile("level.txt");

	// _factory.createObject("models/Table.obj", glm::ivec2(3, 1));
	//Add some lights
	// PointLight l(glm::vec3(0.0f), glm::vec3(0.6f), glm::vec3(0.7f), 5.0f);
	// _factory.createLight(l, player);
	//Add some loot
	std::vector<glm::vec3>* pLootPosList = level->getGrid().getLootLocations();
	for (unsigned int i = 0; i < pLootPosList->size(); i++)
		GameObject *tmpLoot = _factory.createLoot("models/Chalice.obj", (*pLootPosList)[i] + glm::vec3(0.f, 1.f, 0.f));
}


void TestGame::compose(RenderInfo &rI)
{
	if (_renderBufferKey._active)
	{
		_bufferRenderer.render(rI);
	}
	else if (_cubeMapBufferKey._active)
	{
		_cubeMapRenderer.render(rI);
	}
	else
	{
		Game::compose(rI);
	}
}


Character* TestGame::getCharacter()
{
	return _player;
}
