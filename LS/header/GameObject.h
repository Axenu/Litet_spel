#pragma once 
#ifndef GAMEOBJECT_H 
#define GAMEOBJECT_H 
#include "node.h"
#include"Model.h"
#include "gl/glInclude.h"
#include "AABB.h"

class GameObject : public Node
{
public:
	GameObject();
	GameObject(Model &m);
	virtual ~GameObject();
	void onUpdate(float dt);
	void setModel(Model &m);
	const Model& getModel();
	const glm::mat4& getModelMatrix();
	bool pick(glm::vec4 origin, glm::vec4 dir);
private:
	Model _model;
};




#endif 