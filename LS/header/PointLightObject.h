#pragma once
#include"node.h"
#include"Render/PointLight.h"

class PointLightObject
	: public Node
{
private:
	PointLight _lightInfo;
public:
	PointLightObject();
	PointLightObject(const PointLight &light);
	PointLightObject(const PointLight &light, Node *parent);
	~PointLightObject();

	virtual void update(float dT);



	/* Get the light information
	*/
	const PointLight &getLightInfo();
};

