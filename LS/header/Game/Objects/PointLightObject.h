#pragma once
#include "node.h"
#include "Render/PointLight.h"
#include "GameObject.h"
#include <math.h>

class PointLightObject
	: public GameObject
{
private:
	PointLight _lightInfo;
	glm::vec3 _prevPositionDestination;
	glm::vec3 _positionDestination;
	float _timeToOffset, _targetSize, _previousSize;
	bool _nextFlicker, _enableFlicker;
public:
	PointLightObject();
	PointLightObject(const PointLightValue &light, Node *parent = nullptr);
	virtual ~PointLightObject();

	virtual void update(float dT);

	virtual void init();

	/* Add the object to the frame
	*/
	virtual void addToFrame(DrawFrame &dF);
	/* Enable/disable light flickers */
	void lightFlicker(bool enabled);

	/* Get the light information
	*/
	const PointLightValue &getLightInfo();
};
