#pragma once
#include"Mesh.h"
#include"Render/RenderInfo.h"
#include"Render/Material.h"


class MeshPart
{
private:
	Mesh *_mesh;
	Material *_material;
public:

	MeshPart();
	MeshPart(Mesh *mesh, Material *mat);
	void render(RenderInfo &fD, const glm::mat4 &modelMatrix) const;

};
