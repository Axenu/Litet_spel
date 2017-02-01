#include "PointLightObject.h"



PointLightObject::PointLightObject()
	: Node(), _lightInfo()
{
}

PointLightObject::PointLightObject(const PointLight &light)
	: PointLightObject(light, nullptr) {
}
PointLightObject::PointLightObject(const PointLight &light, Node *parent)
	: Node(light._pos, parent), _lightInfo(light) {

}

PointLightObject::~PointLightObject() {
}

void PointLightObject::update(float dT) {
	Node::update(dT);
	_lightInfo._pos = _position;
}


/* Get the light information
*/
const PointLight& PointLightObject::getLightInfo() {
	return _lightInfo;
}