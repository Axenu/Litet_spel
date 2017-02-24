#include "Game/Level/ObjectFactory.h"



ObjectFactory::ObjectFactory(EventManager &events, const std::string &resourcePath)
	: _path(resourcePath), _meshShader(), _skinnedShader(), _models(),  _events(events)
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
	Model tmpModel = _models.GetModel(_path + model, mat);
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
	Model tmpModel = _models.GetModel(_path + model, mat);
	AntiLightGrenade* grenade = new AntiLightGrenade(tmpModel);
	grenade->setLevel(&_level->getGrid());
	grenade->setScale((float)0.0675);
	_scene->add(grenade, true);
	return grenade;
}


GameObject* ObjectFactory::createObject(const std::string &model, glm::ivec2 square)
{
	Material mat(&_meshShader);
	mat.setColor("diffuse", glm::vec4(0.627f, 0.3215f, 0.176f, 1.0f));
	Model tmpModel = _models.GetModel(_path + model, mat);
	GameObject* object = new GameObject(tmpModel, type::Doodad);
	object->setPosition(calcPos(square, tmpModel.getBox()));
	object->init();
	_scene->add(object, false);
	_level->getGrid().addObject(object, gridType::object);
	return object;
}
LootObject* ObjectFactory::createLoot(const std::string &model, glm::ivec2 square)
{
	Material tmpMat(&_meshShader);
	tmpMat.setColor("diffuse", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	tmpMat.setColor("spec", glm::vec4(1.0f));
	tmpMat.setFloat("shine", 20.0f);
	Model tmpModel = _models.GetModel(_path + model, &_meshShader);
	LootObject* object = new LootObject(tmpModel, type::Doodad);

	object->setPosition(calcPos(square, tmpModel.getBox()));
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
	_level->getLight(&light);
	return object;
}
PointLightObject* ObjectFactory::createLight(PointLight light, Node *parent)
{
	PointLightObject* object = new PointLightObject(light, parent);
	object->init();
	_scene->add(object, true);
	return object;
}

void ObjectFactory::loadSceneFromFile(std::string path)
{
	std::ifstream file;
  	file.open(_path + path);
	std::string line;
	PointLight l(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f), 3.0f);
	while (std::getline(file, line))
	{
		// std::cout << line << std::endl;
	    std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "model")
		{
			std::string fileName;
			int x, y;
			iss >> x >> y >> fileName;
			createObject(fileName, glm::ivec2(x, y));
		}
		else if (type == "light")
		{
			float x, y;
			iss >> x >> y;
			createLight(l, glm::vec3(x, 0.5f, y));
		}
		else if (type == "guard")
		{
			int x, y;
			iss >> x >> y;
			std::vector<glm::vec2> walkingPoints;
			while (x != -1 && y != -1)
			{
				walkingPoints.push_back(glm::vec2(x, y));
				iss >> x >> y;
			}
			guardWalkingPoints.push_back(walkingPoints);
		}
	    // int a, b;
	    // if (!(iss >> a >> b)) { break; } // error

	    // process pair (a,b)
	}
}

MeshShader& ObjectFactory::getShader() {
	return _meshShader;
}

std::vector<std::vector<glm::vec2>> ObjectFactory::getGuardsWalkingPoints()
{
	return this->guardWalkingPoints;
}
