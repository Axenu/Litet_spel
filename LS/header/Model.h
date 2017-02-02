#pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "MeshPart.h"
#include "Render/MeshShader.h"
#include "Render/RenderInfo.h"
#include "Mesh.h"
#include "Render/Material.h"
#include "AABB.h"


class Model{
public:
	Model();
	Model(std::vector<MeshPart> &parts);
	Model(MeshPart &part);

	void render(RenderInfo &fD, glm::mat4 &modelMatrix) const;
	const std::vector<MeshPart>& getParts() const;
	bool pick(glm::vec3 origin, glm::vec3 dir);
	~Model();
private:
	std::vector<MeshPart> _meshParts;
};
