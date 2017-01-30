#include "GameObject.h" 

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
}

void GameObject::onUpdate(float dt)
{

}

void GameObject::onRender()
{
	mesh->render();
}

glm::mat4 GameObject::getModelMatrix()
{
	return modelMatrix;
}