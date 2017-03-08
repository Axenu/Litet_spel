#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "node.h"
#include "Render/Mesh/Model.h"
#include "gl/glInclude.h"
#include "math/AABB.h"
#include "Game/Scene/DrawFrame.h"
#include "GameObjectType.h"
#include "Render/Mesh/Animation/AnimatedSkeleton.h"

class GameObject : public Node
{
public:

	const type::GameObjectType _type;

	GameObject();
	GameObject(type::GameObjectType type);
	GameObject(Node* parent, const glm::vec3 &pos, type::GameObjectType type);
	/* Create a doodad object
	*/
	GameObject(Model &m);
	GameObject(Model &m, type::GameObjectType type);
	virtual ~GameObject();
	virtual void update(float dt);
	virtual void init();
	void setModel(Model &m);
	void setModelAABB(const AABB &aabb);
	const Model& getModel();
	AABB getAABB();
	bool pick(glm::vec4 origin, glm::vec4 dir);
	bool pick(Camera &cam);
	/* Add the object to the frame
	*/
	virtual void addToFrame(DrawFrame &dF);
	/* Get the skeleton of this object, can be null */
	const AnimatedSkeleton* getSkeleton() const;
	/* Returns if the object has a skeleton and thus animated */
	bool hasSkeleton() const;
protected:
	/* The gameobjects animated skeleton controller */
	std::unique_ptr<AnimatedSkeleton> _animatedSkel;
private:
	Model _model;

};




#endif
