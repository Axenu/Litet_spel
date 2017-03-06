#include "Game/Objects/PointLightObject.h"



PointLightObject::PointLightObject()
	: GameObject(), _lightInfo()
{
}
PointLightObject::PointLightObject(const PointLightValue &light, Node *parent)
	: GameObject(parent, light._pos, type::PointLight), _lightInfo(light) {

}

PointLightObject::~PointLightObject() {}

void PointLightObject::update(float dT) {
	Node::update(dT);
	// std::cout << "plight update" << std::endl;
	_lightInfo._light._pos = _modelMatrix[3];
	GameObject::setModelAABB(_lightInfo.generateAABB());
	_lightInfo.updateMatrices();
}

void PointLightObject::init()
{
	Node::init();
	_lightInfo._light._pos = _modelMatrix[3];
	GameObject::setModelAABB(_lightInfo.generateAABB());
	_lightInfo.updateMatrices();
}

/* Add the object to the frame
*/
void PointLightObject::addToFrame(DrawFrame &dF) {
	GameObject::addToFrame(dF);
	dF.add(_lightInfo);
}

/* Get the light information
*/
const PointLightValue& PointLightObject::getLightInfo() {
	return _lightInfo._light;
}
