#pragma once 
#ifndef GAMEOBJECT_H 
#define GAMEOBJECT_H 
#include "node.h"
#include"Model.h"
#include "gl/glInclude.h"

class GameObject : public Node
{
public:
	GameObject();
	GameObject(Model &m);
	~GameObject();
	void onUpdate(float dt);
	void setModel(Model &m);
	const Model& getModel();
	const glm::mat4& getModelMatrix();
private:
	Model _model;
};




#endif 