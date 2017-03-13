#include "Game/Objects/PointLightObject.h"
#include "math/MathFunctions.h"


PointLightObject::PointLightObject()
	: GameObject(), _lightInfo()
{
}
PointLightObject::PointLightObject(const PointLightValue &light, Node *parent)
	: GameObject(parent, light._pos, type::PointLight), _lightInfo(light) {
	_prevPositionDestination = glm::vec3(0.0f);
	_nextFlicker = true;
	_positionDestination = glm::vec3(0.0f);
	_previousSize = 4.f;
	_targetSize = 4.f;
	_timeToOffset = 0.0f;
	_enableFlicker = true;
}

PointLightObject::~PointLightObject() {}

void PointLightObject::update(float dT) {
	Node::update(dT);
	_lightInfo._light._pos = _modelMatrix[3];
	if (_enableFlicker)
	{
		//flickering
		glm::vec3 offset;
		if (_timeToOffset > 0.0f)
		{
			offset = lerp(_positionDestination, _prevPositionDestination, _timeToOffset);
			_lightInfo._light._fadeDist = lerp(_targetSize, _previousSize, _timeToOffset);
			_timeToOffset -= dT * 1.f;
		}
		else
		{
			_timeToOffset = 1.f;
			_prevPositionDestination = _positionDestination;
			offset = _positionDestination;
			_previousSize = _targetSize;
			_lightInfo._light._fadeDist = _targetSize;
			if (_nextFlicker)
			{
				const float flickerDist = 0.01f;
				_positionDestination = glm::vec3(getRand(-flickerDist, flickerDist), 0.f, getRand(-flickerDist, flickerDist));
				_targetSize = getRand(3.5f, 4.5f);
			}
			else 
			{
				_positionDestination = glm::vec3(0.f);
				_targetSize = 4.f;
			}
			_nextFlicker = !_nextFlicker;
		}
		_lightInfo._light._pos += offset;
	}
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

void PointLightObject::lightFlicker(bool enabled)
{
	_enableFlicker = enabled;
}
/* Get the light information
*/
const PointLightValue& PointLightObject::getLightInfo() {
	return _lightInfo._light;
}
