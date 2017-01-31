#pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "camera.h"
#include "Render/MeshShader.h"
#include "Render/FrameData.h"
#include "Mesh.h"
#include "Material.h"

struct MeshPart
{
	Mesh *_mesh;
	MeshShader *_shader;
	Material *_material;
};


class Model{
public:
	Model();
	Model(std::vector<Mesh*> pMeshes, std::vector<MeshShader*> pShaders, std::vector<Material*> pMaterials);
	Model(Mesh* pMesh, MeshShader* pShader, Material* pMaterial);

	void render(FrameData &fD, glm::mat4 &modelMatrix) const;
	const std::vector<MeshPart>& getParts() const;

	~Model();
private:
	std::vector<MeshPart> _meshParts;
};
