#pragma once
#include<vector>
#define GLM_FORCE_RADIANS
#include<glm/mat4x4.hpp>
#include"Render/Mesh/Model.h"
#include"Render/RenderInfo.h"
#include"Render/PointLight.h"
#include"Render/AntiLightValues.h"

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
	std::vector<AntiLightValues> _grenadeInfo;
public:
	DrawFrame();
	~DrawFrame();

	/* Add a drawable to be rendered
	*/
	void add(const Model &m, const glm::mat4 &modelMatrix);
	/* Add a point point light to be rendered
	*/
	void add(const PointLight &light);
	/* Add a grenade to be rendered
	*/
	void add(const AntiLightValues &grenade);
	/* Render the frame
	*/
	void render(RenderInfo &rI);
	/* Render all meshes in the drawframe with the specific shader,  not using any material links */
	void renderMeshOnly(RenderInfo &rI, MeshShader* shader);
	/* Render all meshes without skeleton animations in the drawframe with the specific shader, not using linked material links */
	void renderNonAnimatedMeshes(RenderInfo &rI, MeshShader* shader);
	/* Render all meshes with skelton animations in the drawframe with the specific shader, not using linked material links */
	void renderAnimatedMeshes(RenderInfo &rI, MeshShader* shader);
	void cullLightsByDistance(glm::vec3 &pos);
	/* Get batched light info for the frame
	*/
	std::vector<PointLight>& getLightInfo();
	/* Get batched grenade info for the frame
	*/
	std::vector<AntiLightValues>& getGrenadeInfo();
};
