#include "Game/Objects/PointLightObject.h"



PointLightObject::PointLightObject()
	: GameObject(), _lightInfo()
{
}
PointLightObject::PointLightObject(const PointLightValue &light, Node *parent)
	: GameObject(parent, light._pos, type::PointLight), _lightInfo(light) {
	_prevPositionDestination = glm::vec3(0.0f);
	_positionOffset = glm::vec3(0.0f);
	_positionDestination = glm::vec3(0.0f);
	_timeToOffset = 0.0f;
}

PointLightObject::~PointLightObject() {}

void PointLightObject::update(float dT) {
	Node::update(dT);
	_lightInfo._light._pos = _modelMatrix[3];
	//flickering
	if (_timeToOffset > 0.0f)
	{
		glm::vec3 diff = _positionDestination - _prevPositionDestination;
        _positionOffset = _prevPositionDestination + _timeToOffset*diff;
		_timeToOffset -= dT * 6.0f;
	}
	else
	{
		_timeToOffset = 1.0f;
		_prevPositionDestination = _positionDestination;
		_positionOffset = _positionDestination;
		_positionDestination = glm::vec3((std::fmod((float)rand(), 1000.0f)-500.0f)/20000.0f, (std::fmod((float)rand(), 1000.0f)-500.0f)/20000.0f, (std::fmod((float)rand(), 1000.0f)-500.0f)/20000.0f);
	}
	// _lightInfo._light._diffuse = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	_lightInfo._light._pos += _positionOffset;
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
