#include "Game/Level/ObjectFactory.h"



ObjectFactory::ObjectFactory(EventManager &events, const std::string &resourcePath, std::string modelPath)
	: _path(resourcePath), _modelPath(modelPath), _meshShader(), _skinnedShader(), _models(),  _events(events)
{
}

ObjectFactory::~ObjectFactory()
{

}

glm::vec3 ObjectFactory::calcPos(glm::ivec2 square, const AABB &box)
{
	glm::vec3 vec = _level->getGrid().getCenter(square);
	//Move so object is centered on top of the square.
	vec.y -= box.getMin().y;
	return vec;
}

std::unique_ptr<Scene> ObjectFactory::createLevel(const std::string &level, Level *&lvl)
{
	_level = new Level(_path + level, _events, _meshShader);
	lvl = _level;
	_level->init();
	std::unique_ptr<GameObject> ptr(_level);
	std::unique_ptr<Scene> scene(new Scene(ptr, _level->getAABB()));
	_level->setScene(scene.get());
	_scene = scene.get();
	return std::move(scene);
}
Character* ObjectFactory::createCharacter(glm::ivec2 square, float height)
{

	Character* player = new Character(_level->getGrid().getCenter(square), &_events);
	player->setLevel(&_level->getGrid());
	player->setScene(_scene);
	player->init();
	_scene->getCamera().setParent(player);
	_scene->getCamera().setPositionY(height);
	_scene->add(player, true);
	return player;
}

Character* ObjectFactory::createCharacter(glm::ivec2 square, float height, AntiLightGrenade & grenade)
{

	Character* player = new Character(_level->getGrid().getCenter(square), &_events,&grenade);
	player->setLevel(&_level->getGrid());
	player->setScene(_scene);
	player->init();
	_scene->getCamera().setParent(player);
	_scene->getCamera().setPositionY(height);
	_scene->add(player,true);
	return player;
}


Guard* ObjectFactory::createGuard(const std::string &model, glm::ivec2 square, Character& player, std::vector<glm::vec2>& walkingPoints)
{
	Material mat(&_skinnedShader);
	mat.setColor("diffuse", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Model tmpModel = _models.GetModel(_modelPath + model, mat);
	glm::vec3 pos = calcPos(square, tmpModel.getBox());
	Guard* guard = new Guard(pos, &player, &_events, tmpModel, _level, &walkingPoints);
	guard->init();
	_scene->add(guard, true);

	return guard;
}

AntiLightGrenade * ObjectFactory::createAntiLightGrenade(const std::string & model, glm::ivec2 square)
{
	Material mat(&_meshShader);

	mat.setColor("diffuse", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Model tmpModel = _models.GetModel(_modelPath + model, mat);
	AntiLightGrenade* grenade = new AntiLightGrenade(tmpModel);
	grenade->setLevel(&_level->getGrid());
	grenade->setScale((float)0.0675);
	_scene->add(grenade, true);
	return grenade;
}


GameObject* ObjectFactory::createObject(const std::string &model, glm::ivec2 square, glm::vec3 rotation)
{
	Model tmpModel = _models.GetModel(_modelPath + model, &_meshShader);
	GameObject* object = new GameObject(tmpModel, type::Doodad);
	object->setPosition(calcPos(square, tmpModel.getBox()));
	object->init();
	_scene->add(object, false);
	_level->getGrid().addObject(object, gridType::object);
	return object;
}
LootObject* ObjectFactory::createLoot(const std::string &model, glm::ivec2 square, glm::vec3 rotation, int value)
{
	Material tmpMat(&_meshShader);
	tmpMat.setColor("diffuse", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	tmpMat.setColor("spec", glm::vec4(1.0f));
	tmpMat.setFloat("shine", 20.0f);
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
PointLightObject* ObjectFactory::createLight(PointLight light, glm::vec3 position)
{
	light._pos += position;
	PointLightObject* object = new PointLightObject(light, nullptr);
	// Model tmpModel = _models.GetModel(_path + "", &_meshShader);
	object->init();
	_scene->add(object, true);
	return object;
}
PointLightObject* ObjectFactory::createLight(PointLight light, Node *parent)
{
	PointLightObject* object = new PointLightObject(light, parent);
	object->init();
	_scene->add(object, true);
	return object;
}

void ObjectFactory::loadSceneFromFile(std::string path, std::vector<guardData> &guards, std::vector<lootData> &loot)
{
	std::ifstream file;
  	file.open(_path + path);
	std::string line;
	PointLight l(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f), 3.0f);
	glm::vec2 walkP;
	while (std::getline(file, line))
	{
		// std::cout << line << std::endl;
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		//Data items to fill on each line
		glm::ivec2 square(0);
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
				break;
			case 'M': //Model
				iss >> modelName;
				break;
			case 'V': //Value, custom
				iss >> value;
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
			createObject(modelName, square, rot);
		else if (type == "light")
			createLight(l, pos);
		else if (type == "loot")
			loot.push_back({ modelName, square, rot, (int)value });
		else if (type == "guard")
		{
			guards.push_back({ std::vector<glm::vec2>(), square });
		}
		//else if (type == "path")
		// int a, b;
		// if (!(iss >> a >> b)) { break; } // error

		// process pair (a,b)
	}
}

MeshShader& ObjectFactory::getShader() {
	return _meshShader;
}

