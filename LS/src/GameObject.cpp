#include "GameObject.h" 

GameObject::GameObject()
{
	_mesh = nullptr;
}

GameObject::~GameObject()
{

}

void GameObject::onUpdate(float dt)
{

}

void GameObject::render()
{
	if (_mesh != nullptr)
	{
		_mesh->render();
	}
}

void GameObject::setMesh(Mesh * mesh)
{
	_mesh = mesh;
}


glm::mat4 GameObject::getModelMatrix()
{
	return modelMatrix;
}