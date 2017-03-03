#pragma once
#include"node.h"
#include"Render/PointLight.h"
#include"GameObject.h"

class PointLightObject
	: public GameObject
{
private:
	PointLight _lightInfo;
public:
	PointLightObject();
	PointLightObject(const PointLightValue &light, Node *parent = nullptr);
	virtual ~PointLightObject();

	virtual void update(float dT);

	virtual void init();

	/* Add the object to the frame
	*/
	virtual void addToFrame(DrawFrame &dF);


	/* Get the light information
	*/
	const PointLightValue &getLightInfo();
};

