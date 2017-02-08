#pragma once
#include<vector>
#include<glm/mat4x4.hpp>
#include"Render/Mesh/Model.h"
#include"Render/RenderInfo.h"
#include"Render/PointLight.h"

/* Struct holding drawables rendered for a frame
*/
class DrawFrame
{
protected:
	/* Struct holding mesh data awaiting draw.
	*/
	struct MeshDrawable{
		const ModelPart* _part;
		const glm::mat4* _transform;
		MeshDrawable(){}
		MeshDrawable(const ModelPart &part, const glm::mat4 &modelMatrix)
			: _part(&part), _transform(&modelMatrix){
		}
	};
	/* The drawables.
	*/
	std::vector<MeshDrawable> _meshes;
	std::vector<PointLight> _lightInfo;
public:
	DrawFrame();
	~DrawFrame();

	/* Add a drawable to be rendered
	*/
	void add(const Model &m, const glm::mat4 &modelMatrix);
	/* Add a point point light to be rendered
	*/
	void add(const PointLight &light);
	/* Render the frame
	*/
	void render(RenderInfo &fD);
	/* Get batched light info for the frame
	*/
	std::vector<PointLight>& getLightInfo();
};

