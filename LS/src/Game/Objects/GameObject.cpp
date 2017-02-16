#include "Game/Objects/GameObject.h" 

GameObject::GameObject()
	: Node(), _model(), _type(type::Empty)
{
}
GameObject::GameObject(Model &m)
	: GameObject(m, type::Doodad) {
}
GameObject::GameObject(Node* parent, const glm::vec3 &pos, type::GameObjectType type)
	: Node(pos, parent), _model(), _type(type) {
}
GameObject::GameObject(Model &m, type::GameObjectType type)
	: _model(m), _type(type) {
}

GameObject::~GameObject()
{

}

void GameObject::update(float dt)
{
	//_model.transform(_modelMatrix);
	Node::update(dt);
	_worldAABB = _model.getBox().transform(_modelMatrix);
}


void GameObject::setModel(Model &m) {
	_model = m;
}
void GameObject::setModelAABB(const AABB & aabb)
{
	_model.setAABB(aabb);
}
const Model& GameObject::getModel() {
	return _model;
}

AABB GameObject::getAABB()
{
	return _worldAABB;
}

bool GameObject::pick(glm::vec4 origin, glm::vec4 dir)
{
	glm::mat4 modelMatInverse = glm::inverse(_modelMatrix);
	origin = modelMatInverse * origin;
	dir = modelMatInverse * dir;
	glm::vec3 nOrigin = glm::vec3(origin.x, origin.y, origin.z);
	glm::vec3 nDir = glm::vec3(dir.x, dir.y, dir.z);
	return _model.pick(nOrigin, nDir);
}
bool GameObject::pick(Camera & cam)
{
	return pick(cam.getWorldPos(), cam.getLookAt());
}
/* Add the object to the frame
*/
void GameObject::addToFrame(DrawFrame &dF) {
	dF.add(_model, _modelMatrix);
}


