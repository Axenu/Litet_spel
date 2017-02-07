#pragma once 
#ifndef GAMEOBJECT_H 
#define GAMEOBJECT_H 
#include "node.h"
#include "Render/Mesh/Model.h"
#include "gl/glInclude.h"
#include "AABB.h"
#include "Game/Scene/DrawFrame.h"
#include "GameObjectType.h"

class GameObject : public Node
{
public:

	const type::GameObjectType _type;

	GameObject();
	GameObject(Node* parent, const glm::vec3 &pos, type::GameObjectType type);
	/* Create a doodad object
	*/
	GameObject(Model &m);
	GameObject(Model &m, type::GameObjectType type);
	virtual ~GameObject();
	void onUpdate(float dt);
	void setModel(Model &m);
	const Model& getModel();
	const glm::mat4& getModelMatrix();
	bool pick(glm::vec4 origin, glm::vec4 dir);
	bool pick(Camera &cam);
	/* Add the object to the frame
	*/
	virtual void addToFrame(DrawFrame &dF);
private:
	Model _model;
};




#endif 