#pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "ModelPart.h"
#include "Shader/MeshShader.h"
#include "Render/RenderInfo.h"
#include "Mesh.h"
#include "Material.h"
#include "math/AABB.h"


class Model{
public:
	Model();
	Model(std::vector<ModelPart> &parts, Skeleton *skeleton = nullptr);
	Model(ModelPart &part);

	void render(RenderInfo &fD, glm::mat4 &modelMatrix) const;
	const std::vector<ModelPart>& getParts() const;
	bool pick(glm::vec3 origin, glm::vec3 dir);
	void setName(std::string &name);
	void setMaterial(Material &mat);
	void setAABB(const AABB &aabb);
	/* Set the skeleton parent of this model, can be null */
	void setAnimController(AnimatedSkeleton *skeleton);
	Skeleton* getSkeleton();
	std::string getName();
	/* Update model boxes from model space
	*/
	void transform(const glm::mat4 &mat); //Transforms the meshparts AABB and then updates Models world AABB
	const AABB& getBox() const;
	~Model();
private:
	void initAABB();
	AABB _aabb;
	std::string _name;
	std::vector<ModelPart> _meshParts;
	Skeleton *_skeleton = nullptr;
};
