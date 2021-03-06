#include "Game/TestGame.h"



TestGame::TestGame(Setting &setting, EventManager *events)
	: Game(setting, events)
#ifdef DEBUG_CHECK
	, _bufferRenderer(_resource.getQuad(), events), _cubeMapRenderer(_resource.getQuad(), events)
	, _renderBufferKey(events, GLFW_KEY_R), _cubeMapBufferKey(events, GLFW_KEY_C)
#endif
{
}
TestGame::~TestGame()
{
}

std::unique_ptr<Scene> TestGame::spawnScene() {

	//Create the scene, and the level
	Level* level;
	std::unique_ptr<Scene> scene = _factory.createLevel("roomtest.bmp", level);
	//Initiate camera and renderer vars:
	Camera& cam = scene->setCamera(_setting);
	_deferred.setWindowSize((float)_setting.Width(), (float)_setting.Height(), cam);
#ifdef DEBUG_CHECK
	_bufferRenderer.setWindowSize((float)_setting.Width(), (float)_setting.Height(), scene->getCamera());
#endif
	Character* player = _factory.createCharacter(glm::ivec2(48, 48), 1.3f);
	_player = player;


	std::vector<guardData> guards;
	std::vector<lootData> loot;
	std::vector<doorData> doorList;

	_factory.loadSceneFromFile("tmpLevel.txt", guards, loot, doorList);

	for (unsigned short int i = 0; i < doorList.size(); i++)
	{
		if (doorList[i].open)
			_factory.CreateDoor("Door.obj", doorList[i].pos, doorList[i].rotation);
		else
			_factory.createObject("Door_Closed.obj", doorList[i].pos, doorList[i].rotation, gridType::wall, glm::vec3(0.f, 0.f, 0.f));
	}
	for (unsigned int i = 0; i < guards.size(); i++)
		_factory.createGuard("Guard.dae", *player, guards[i]);
	_factory.createRandomLoot(loot, MAX_LOOT_LEVEL);

	/* Preload
	*/
	_factory.preLoadModel("grenade.obj");
	return scene;
}


void TestGame::compose(RenderInfo &rI)
{
#ifdef DEBUG_CHECK
	if (_renderBufferKey._active)
	{
		_bufferRenderer.render(rI);
	}
	else if (_cubeMapBufferKey._active)
	{
		_cubeMapRenderer.render(rI);
	}
	else
#endif
	{
		Game::compose(rI);
	}
}


Character* TestGame::getCharacter()
{
	return _player;
}
