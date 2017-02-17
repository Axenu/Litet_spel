#include "Game/Objects/PointLightObject.h"



PointLightObject::PointLightObject()
	: GameObject(), _lightInfo()
{
}

PointLightObject::PointLightObject(const PointLight &light)
	: PointLightObject(light, nullptr) {
}
PointLightObject::PointLightObject(const PointLight &light, Node *parent)
	: GameObject(parent, light._pos, type::PointLight), _lightInfo(light) {

}

PointLightObject::~PointLightObject() {
}

void PointLightObject::update(float dT) {
	Node::update(dT);
	AABB aabb = AABB(glm::vec3(-_lightInfo._fadeDist), glm::vec3(_lightInfo._fadeDist));
	GameObject::setModelAABB(aabb.transform(_modelMatrix));
	_lightInfo._pos = _modelMatrix[3];
}

void PointLightObject::init()
{
	Node::init();
	AABB aabb = AABB(glm::vec3(-_lightInfo._fadeDist), glm::vec3(_lightInfo._fadeDist));
	GameObject::setModelAABB(aabb.transform(_modelMatrix));
	_lightInfo._pos = _modelMatrix[3];
}

/* Add the object to the frame
*/
void PointLightObject::addToFrame(DrawFrame &dF) {
	GameObject::addToFrame(dF);
	dF.add(_lightInfo);
}

/* Get the light information
*/
const PointLight& PointLightObject::getLightInfo() {
	return _lightInfo;
}