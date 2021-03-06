#include "Game/Level/ObjectFactory.h"
#include "math/MathFunctions.h"

ObjectFactory::ObjectFactory(EventManager *events, const std::string &resourcePath, std::string modelPath)
	: _path(resourcePath), _modelPath(modelPath), _meshShader(), _skinnedShader(), _models()
{
	_eventManager = events;
}

ObjectFactory::~ObjectFactory()
{
}

/* Preload a model not created at start
*/
void ObjectFactory::preLoadModel(const std::string &model)
{
	Model tmpModel = _models.GetModel(_modelPath + model, &_meshShader);
}

void ObjectFactory::createRandomLoot(std::vector<lootData> loot, int totalValue)
{
	seed((unsigned int)time(NULL));
	int currentValue = 0;
	int randNumber;
	while (loot.size() != 0)
	{
		randNumber = getRand(loot.size());
		if (loot[randNumber].value + currentValue >= totalValue)
		{
			createLoot(loot[randNumber].modelName, loot[randNumber].pos, loot[randNumber].rotation, totalValue - currentValue);
			return;
		}
		else
		{
			currentValue += loot[randNumber].value;
			createLoot(loot[randNumber].modelName, loot[randNumber].pos, loot[randNumber].rotation, loot[randNumber].value);
		}
		loot.erase(loot.begin() + randNumber);
	}
}

glm::vec3 ObjectFactory::calcPos(glm::ivec2 square, const AABB &box)
{
	glm::vec3 vec = _level->getGrid().getCenter(square);
	//Move so object is centered on top of the square.
	vec.y -= box.getMin().y;
	return vec;
}

glm::vec3 ObjectFactory::calcRot(glm::ivec2 square)
{
	if (square.x == _level->getGrid().getWidth() - 1)
		return glm::vec3(0.f, 90.f, 0.f);
	else if (square.x == 0)
		return glm::vec3(0.f, -90.f, 0.f);
	else if (square.y == _level->getGrid().getHeight() - 1)
		return glm::vec3(0.f, 0.f, 0.f);
	else if (square.y == 0)
		return glm::vec3(0.f, 180.f, 0.f);
	else
	{
		int offSet = 1;
		if (square.x == _level->getGrid().getWidth() - 1)
		{
			offSet = -1;
		}
		if (_level->getGrid().getTypeNC(square.y, square.x + offSet) == nothing)
		{
			return glm::vec3(0.f, -90.f, 0.f);
		}
		else
		{
			return glm::vec3(0.f, 180.f, 0.f);
		}
	}
}

std::unique_ptr<Scene> ObjectFactory::createLevel(const std::string &level, Level *&lvl)
{
	_level = new Level(_path + level, _eventManager, _meshShader);
	lvl = _level;
	_level->init();
	std::unique_ptr<GameObject> ptr(_level);
	std::unique_ptr<Scene> scene(new Scene(ptr, _level->getAABB()));
	_level->setScene(scene.get());
	_scene = scene.get();
	std::vector<glm::ivec2> windows(_level->getGrid().getWindowData());
	for (unsigned short int i = 0; i < windows.size(); i++)
	{
		createObject("Window_A.obj", windows[i], calcRot(windows[i]), gridType::window, glm::vec3(0.f, 0.375f, 0.f));
	}
	return scene;
}

Character* ObjectFactory::createCharacter(glm::ivec2 square, float height)
{

	Character* player = new Character(_level->getGrid().getCenter(square), _eventManager, 5, height);
	player->setLevel(&_level->getGrid());
	player->setScene(_scene);
	player->init();
	_scene->getCamera().setParent(player);
	_scene->getCamera().setPositionY(height);
	_scene->add(player,true);
	return player;
}

AntiLightGrenade* ObjectFactory::createLightGrenade(const std::string &model, glm::vec3 pos, glm::vec3 dir)
{
	Material mat(&_meshShader);

	mat.setColor("diffuse", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Model tmpModel = _models.GetModel(_modelPath + model, mat);
	AntiLightGrenade* grenade = new AntiLightGrenade(*_eventManager, tmpModel, pos, dir);
	grenade->setLevel(&_level->getGrid());
	grenade->setScale((float)0.0675);
	_scene->add(grenade, true);
	return grenade;
}


Guard* ObjectFactory::createGuard(const std::string &model, Character& player, guardData &data)
{
	//Setup guard
	Model tmpModel = _models.GetModel(_modelPath + model, &_skinnedShader);
	glm::vec3 pos = _level->getGrid().getCenter(data.spawnPosition);
	WalkPoints points(data.walkingPoints, data.walkType, data.face);
	Guard* guard = new Guard(pos, &player, _eventManager, tmpModel, _level, points);
	guard->_id = _guardCount++;
	guard->init();

	_scene->add(guard, true);

	//Setup guard's lantern
	if (guard->hasSkeleton())
	{
		BoneNode* node = guard->getSkeleton()->getBoneNode("Lantern");
		if (node)
		{
			PointLightObject* lantern = createLight(data._light, node);
			lantern->lightFlicker(false);
		}
	}

	return guard;
}


Door * ObjectFactory::CreateDoor(const std::string & model, glm::ivec2 square, glm::vec3 rotation)
{
	Model tmpModel = _models.GetModel(_modelPath + model, &_meshShader);
	Door* object = new Door(tmpModel, type::Door);
	object->setPosition(calcPos(square, tmpModel.getBox()));
	object->setRotEuler(rotation);
	object->init();
	_scene->add(object, false);
	return object;
}


GameObject* ObjectFactory::createObject(const std::string &model, glm::ivec2 square, glm::vec3 rotation, enum gridType type, glm::vec3 positionOffset)
{
	Model tmpModel = _models.GetModel(_modelPath + model, &_meshShader);
	GameObject* object = new StaticGameObject(tmpModel, type::Doodad);
	object->setPosition(calcPos(square, tmpModel.getBox()) + positionOffset);
	object->setRotEuler(rotation);
	if (type == gridType::nothing)
	{
		float height = _level->getGrid().getHeight(square.y, square.x);
		object->moveY(height);
	}
	object->init();
	_scene->add(object, false);
	if (type == gridType::object || type == gridType::wall)
		_level->getGrid().addObject(object, type);
	return object;
}
LootObject* ObjectFactory::createLoot(const std::string &model, glm::ivec2 square, glm::vec3 rotation, int value)
{
	Model tmpModel = _models.GetModel(_modelPath + model, &_meshShader);
	LootObject* object = new LootObject(tmpModel, type::Doodad);

	object->setValue(value);
	object->setPosition(calcPos(square, tmpModel.getBox()));
	object->setRotEuler(rotation);
	//Add rotation somewhere
	object->init();
	//Set Loot ontop of object in square
	object->moveY(_level->getGrid().getHeight(square.y, square.x) - object->getAABB().getMin().y);
	object->init();
	_scene->add(object, false);
	return object;
}
PointLightObject* ObjectFactory::createLight(PointLightValue light, Node *parent)
{
	PointLightObject* object = new PointLightObject(light, parent);
	object->init();
	_scene->add(object, true);
	return object;
}

void ObjectFactory::loadSceneFromFile(std::string path, std::vector<guardData> &guards, std::vector<lootData> &loot, std::vector<doorData> &doorList)
{
	std::ifstream file;
  	file.open(_path + path);
	std::string line, comment;
	PointLightValue light(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f), 3.0f);
	glm::vec2 walkP;
	while (std::getline(file, line))
	{
		// std::cout << line << std::endl;
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		//If begins with 'list'
		bool isList = type.compare(0, 4, "list") == 0;
		//Data items to fill on each line
		glm::ivec2 square(0);
		std::vector<glm::ivec2> squareList;
		glm::vec3 pos(0.f), rot(0.f), offSet(0.f);
		float value = 0.f;
		std::string modelName;
		//Switch statement readin next data type
		while (iss.peek() != EOF)
		{
			switch (iss.get())
			{
			case 'O': //Offset
				iss >> offSet.x >> offSet.y >> offSet.z;
				break;
			case 'P': //Position
				iss >> pos.x >> pos.y >> pos.z;
				break;
			case 'R': //Position
				iss >> rot.x >> rot.y >> rot.z;
				break;
			case 'S': //Square
				iss >> square.x >> square.y;
				if (isList)
					squareList.push_back(square);
				break;
			case 'M': //Model
				iss >> modelName;
				break;
			case 'V': //Value, custom
				iss >> value;
				break;
			case 'D': //Light diffuse
				iss >> light._diffuse.x >> light._diffuse.y >> light._diffuse.z;
				break;
			case 's': //Light Spec
				iss >> light._specular.x >> light._specular.y >> light._specular.z;
				break;
			case 'H':
				iss >> walkP.x >> walkP.y;
				if (guards.size() > 0)
					guards.back().walkingPoints.push_back(walkP);
				break;
			case '%':
				//Read comment
				iss >> comment;
			default:
				break;
			}
		}
		if (type == "model")
			createObject(modelName, square, rot, value == 0.f ? gridType::object : gridType::nothing, offSet);
		else if (type == "light")
		{
			light._pos = pos;
			if (value > 0.f)
				light._fadeDist = 4.f;
			createLight(light);
		}
		else if (type == "listDoors")
		{
			seed((unsigned int)time(NULL));
			int lastSize = doorList.size();
			//Generate open doors
			for (unsigned short int i = 0; i < squareList.size(); i++)
				doorList.push_back(doorData(squareList[i], calcRot(squareList[i]), true));
			//Close some doors, might close same door twice but... don't care?
			int closeCount = getRand((int)value + 1);
			for (int i = 0; i < closeCount; i++)
			{
				doorList[lastSize + getRand(squareList.size())].open = false;
			}
		}
		else if (type == "loot")
			loot.push_back(lootData(square, rot, modelName, (int)value));
		else if (type == "listSuperLoot")
			createLoot(modelName, squareList[getRand(squareList.size())], rot, (int)value);
		else if (type == "exitDoor")
			createObject(modelName, square, rot, gridType::nothing, offSet);
		else if (type == "guard")
		{
			light._pos = pos;
			light._fadeDist = 4.f;
			guards.push_back(guardData(square, square, (unsigned int)value, light));
		}
		else if (type == "path")
		{
			if (guards.size() > 0)
			{
				guards.back().face = square;
			}
		}
	}
}

MeshShader& ObjectFactory::getShader() {
	return _meshShader;
}
