#include "PointLightObject.h"



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