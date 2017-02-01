#pragma once
#include<vector>
#include<glm/mat4x4.hpp>
#include"Model.h"
#include"Render/FrameData.h"

/* Struct holding drawables rendered for a frame
*/
class DrawFrame
{
protected:
	/* Struct holding mesh data awaiting draw.
	*/
	struct MeshDrawable{
		const MeshPart* _part;
		const glm::mat4* _transform;
		MeshDrawable(){}
		MeshDrawable(const MeshPart &part, const glm::mat4 &modelMatrix)
			: _part(&part), _transform(&modelMatrix){
		}
	};
	/* The drawables.
	*/
	std::vector<MeshDrawable> _meshes;
public:
	DrawFrame();
	~DrawFrame();

	/* Add a drawable to be rendered
	*/
	void add(const Model &m, const glm::mat4 &modelMatrix);
	/* Render the frame
	*/
	void render(FrameData &fD);
};

