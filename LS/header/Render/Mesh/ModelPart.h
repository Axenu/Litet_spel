#pragma once
#include"Mesh.h"
#include"Render/RenderInfo.h"
#include"Material.h"
#include"AABB.h"


class ModelPart
{
private:
	Mesh *_mesh;
	Material _material;
public:
	/* Mesh box in local space
	*/
	AABB _box;

	ModelPart();
	ModelPart(Mesh *mesh, Material &mat);
	virtual ~ModelPart();
	void render(RenderInfo &fD, const glm::mat4 &modelMatrix) const;
	Mesh& getMesh();
	void setMaterial(Material &mat);
	const AABB& getBox();
};
