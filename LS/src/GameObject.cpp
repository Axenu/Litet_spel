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