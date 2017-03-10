#pragma once
#include "Mesh.h"
#include "Render/RenderInfo.h"
#include "Animation/AnimatedSkeleton.h"
#include "Material.h"
#include "math/AABB.h"


class ModelPart
{
private:
	Mesh *_mesh;
	/* Reference to a linked skeleton parent, can be null */
	AnimatedSkeleton *_skeleton;
	Material _material;
	bool _hasShadow = true;
public:
	/* Mesh box in local space
	*/
	AABB _box;

	ModelPart();
	ModelPart(Mesh *mesh, Material &mat);
	virtual ~ModelPart();
	void render(RenderInfo &fD, const glm::mat4 &modelMatrix) const;
	void renderMeshOnly() const;
	const Mesh& getMesh() const;
	void setMaterial(Material &mat);
	/* Get the reference to the skeleton if there is one */
	const AnimatedSkeleton* getSkeleton() const;
	/* Check if the object has a skeleton and is animated */
	bool hasSkeleton() const;
	/* Set a skeleton reference telling that the part is animated */
	void setSkeleton(AnimatedSkeleton *skel);
	const AABB& getBox();
	void setHasShadows(bool shadow);
	bool getHasShadow() const;
};
