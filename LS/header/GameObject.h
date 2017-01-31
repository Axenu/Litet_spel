#pragma once 
#ifndef GAMEOBJECT_H 
#define GAMEOBJECT_H 
#include "node.h" 
#include "Mesh.h" 
#include "gl/glInclude.h"

class GameObject : public Node
{
public:
	GameObject();
	~GameObject();
	void onUpdate(float dt);
	void render();
	void setMesh(Mesh *mesh);
	glm::mat4 getModelMatrix();
private:
	Mesh *_mesh;
};




#endif 