#include "Game/TestGame.h"



TestGame::TestGame(Setting &setting, EventManager *events)
	: Game(setting, events), _bufferRenderer(_resource.getQuad(), events), _cubeMapRenderer(_resource.getQuad(), events), _renderBufferKey(events, GLFW_KEY_R), _cubeMapBufferKey(events, GLFW_KEY_C)
{
}
TestGame::~TestGame()
{
	std::cout << "delete game" << std::endl;
}

std::unique_ptr<Scene> TestGame::spawnScene() {

	//Create the scene, and the level
	Level* level;
	std::unique_ptr<Scene> scene = _factory.createLevel("Demo1.bmp", level);
	//Initiate camera and renderer vars:
	Camera& cam = scene->setCamera(_setting);
	_deferred.setWindowSize((float)_setting.Width(), (float)_setting.Height(), cam);
	_bufferRenderer.setWindowSize((float)_setting.Width(), (float)_setting.Height(), scene->getCamera());
	Character* player = _factory.createCharacter(glm::ivec2(3, 5), 1.3f);
	_player = player;


	std::vector<guardData> guards;
	std::vector<lootData> loot;
	std::vector<doorData> doorList;

	_factory.loadSceneFromFile("level.txt", guards, loot, doorList);

	for (unsigned short int i = 0; i < doorList.size(); i++)
	{
		if (doorList[i].open)
			_factory.CreateDoor("Door.obj", doorList[i].pos, doorList[i].rotation);
		else
			_factory.CreateDoor("Door_Closed.obj", doorList[i].pos, doorList[i].rotation);
	}
	for (unsigned int i = 0; i < guards.size(); i++)
		_factory.createGuard("Character.dae", guards[i].spawnPosition, *player, guards[i].walkingPoints);
	_factory.createRandomLoot(loot, 20.f);

	/* Preload
	*/
	_factory.preLoadModel("grenade.obj");
	return scene;
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
