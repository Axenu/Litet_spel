#pragma once
#include"Mesh.h"
#include"Render/RenderInfo.h"
#include"Material.h"
#include"AABB.h"


class MeshPart
{
private:
	Mesh *_mesh;
	Material *_material;
public:
	/* Mesh box in local space
	*/
	AABB _box;

	MeshPart();
	MeshPart(Mesh *mesh, Material *mat);
	virtual ~MeshPart();
	void render(RenderInfo &fD, const glm::mat4 &modelMatrix) const;
	Mesh& getMesh();
};
