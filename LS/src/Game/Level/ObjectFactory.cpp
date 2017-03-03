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
void ObjectFactory::createRandomLoot(std::vector<lootData> loot, float totalValue)
{
	for (unsigned int i = 0; i < loot.size(); i++)
		createLoot(loot[i].modelName, loot[i].pos, loot[i].rotation, loot[i].value);
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
		createObject("Window_A.obj", windows[i], calcRot(windows[i]), gridType::nothing, glm::vec3(0.f, 0.375f, 0.f));
	}
	return scene;
}

Character* ObjectFactory::createCharacter(glm::ivec2 square, float height)
{

	Character* player = new Character(_level->getGrid().getCenter(square), _eventManager, 10, height);
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


Guard* ObjectFactory::createGuard(const std::string &model, glm::ivec2 square, Character& player, std::vector<glm::vec2>& walkingPoints)
{
	Material mat(&_skinnedShader);
	mat.setColor("diffuse", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Model tmpModel = _models.GetModel(_modelPath + model, mat);
	glm::vec3 pos = calcPos(square, tmpModel.getBox());
	Guard* guard = new Guard(pos, &player, _eventManager, tmpModel, _level, walkingPoints);
	guard->_id = _guardCount++;
	guard->init();
	_scene->add(guard, true);

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
	GameObject* object = new GameObject(tmpModel, type::Doodad);
	object->setPosition(calcPos(square, tmpModel.getBox()) + positionOffset);
	object->setRotEuler(rotation);
	object->init();
	_scene->add(object, false);
	if (type == gridType::object)
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
	object->moveY(1.f);
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
	std::string line;
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
		glm::vec3 pos(0.f), rot(0.f);
		float value = 0.f;
		std::string modelName;
		//Switch statement readin next data type
		while (iss.peek() != EOF)
		{
			switch (iss.get())
			{
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
			default:
				break;
			}
		}
		if (type == "model")
			createObject(modelName, square, rot, gridType::object, glm::vec3(0.f, 0.f, 0.f));
		else if (type == "light")
		{
			light._pos = pos;
			if (value > 0.f)
				light._fadeDist = value;
			createLight(light);
		}
		else if (type == "listDoors")
		{
			//Generate open doors
			for (unsigned short int i = 0; i < squareList.size(); i++)
				doorList.push_back(doorData(squareList[i], calcRot(squareList[i]), true));
			//Close some doors, might close same door twice but... don't care?
			int closeCount = std::max((int)value, 1);
			for (int i = 0; i < closeCount; i++)
				doorList[getRand(squareList.size())].open = false;
		}
		else if (type == "loot")
			loot.push_back(lootData(square, rot, modelName, (int)value ));
		else if (type == "guard")
		{
			guards.push_back({ std::vector<glm::vec2>(), square });
		}
	}
}

MeshShader& ObjectFactory::getShader() {
	return _meshShader;
}
