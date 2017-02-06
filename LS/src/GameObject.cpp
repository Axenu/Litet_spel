#include "GameObject.h" 

GameObject::GameObject()
	: _model()
{
}
GameObject::GameObject(Model &m)
 : _model(m) {
}

GameObject::~GameObject()
{

}

void GameObject::onUpdate(float dt)
{

}


void GameObject::setModel(Model &m) {
	_model = m;
}
const Model& GameObject::getModel() {
	return _model;
}


const glm::mat4& GameObject::getModelMatrix()
{
	return _modelMatrix;
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


