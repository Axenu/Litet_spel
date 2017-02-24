#include "Game/TestGame.h"



TestGame::TestGame(Setting &setting, EventManager &events)
	: Game(setting, events), _bufferRenderer(_resource.getQuad(), events), _cubeMapRenderer(_resource.getQuad(), events), _renderBufferKey(events, GLFW_KEY_R), _cubeMapBufferKey(events, GLFW_KEY_C)
{
}
TestGame::~TestGame()
{

}

void TestGame::setupRI(RenderInfo &rI) {
	rI._lightGrenadePos = _player->getGrenadeData()._grenadePositionWhenLanded;
	rI._lightGrenadeExpansionAndFading = glm::vec2(_player->getGrenadeData().expanding, _player->getGrenadeData().fading);
}
void TestGame::initiate() {

	//Create the scene, and the level
	Level* level;
	_scene = _factory.createLevel("Demo1.bmp", level);
	//Initiate camera and renderer vars:
	Camera& cam = _scene->setCamera(_setting);
	_deferred.setWindowSize((float)_setting.Width(), (float)_setting.Height(), cam);
	_bufferRenderer.setWindowSize((float)_setting.Width(), (float)_setting.Height(), _scene->getCamera());

	AntiLightGrenade* grenade = _factory.createAntiLightGrenade("models/grenade.obj", glm::ivec2(2, 2));
	Character* player = _factory.createCharacter(glm::ivec2(3, 5), 1.3f, *grenade);
	_player = player;

	_factory.loadSceneFromFile("level.txt");

	//Add guards
	std::vector<guardData> guardsWalkingPoints = _factory.getGuardsWalkingPoints();
	for (unsigned int i = 0; i < guardsWalkingPoints.size(); i++)
	{
		_factory.createGuard("models/Character.dae", guardsWalkingPoints[i].spawnPosition, *player, guardsWalkingPoints[i].walkingPoints);
	}

	//Add some loot
	std::vector<lootData> lootList = _factory.getLootData();
	for (unsigned int i = 0; i < lootList.size(); i++)
	{
		GameObject *tmpLoot = _factory.createLoot("models/Chalice.obj", lootList[i].pos, lootList[i].rotation, lootList[i].value);
	}
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
