#include "Game/Scene/DrawFrame.h"



DrawFrame::DrawFrame()
{
}


DrawFrame::~DrawFrame()
{
}
void DrawFrame::add(const Model &m, const glm::mat4 &modelMatrix) {
	const std::vector<ModelPart> &parts = m.getParts();
	for (unsigned int i = 0; i < parts.size(); i++)
		_meshes.push_back(MeshDrawable(parts[i], modelMatrix));
}
//add animated skeleton
void DrawFrame::add(AnimatedSkeleton *skeleton) {
	_animatedSkeletons.push_back(skeleton);
}

void DrawFrame::add(const PointLight &light) {
	_lightInfo.push_back(light);
}
/* Add a grenade to be rendered
*/
void DrawFrame::add(const AntiLightValues &grenade) {
	_grenadeInfo.push_back(grenade);
}

/* Render the frame
*/
void DrawFrame::render(RenderInfo &fD) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		ref._part->render(fD, *ref._transform);
	}
}

void DrawFrame::renderMeshOnly(RenderInfo &rI, MeshShader* shader) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		shader->assignUniforms(rI, *_meshes[i]._part, *ref._transform, nullptr);
		ref._part->renderMeshOnly();
	}
}

void DrawFrame::renderNonAnimatedMeshes(RenderInfo &rI, MeshShader* shader) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		if (!ref._part->hasSkeleton())
		{
			shader->assignUniforms(rI, *_meshes[i]._part, *ref._transform, nullptr);
			ref._part->renderMeshOnly();
		}
	}
}

void DrawFrame::renderAnimatedMeshes(RenderInfo &rI, MeshShader* shader) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		if (ref._part->hasSkeleton())
		{
			shader->assignUniforms(rI, *_meshes[i]._part, *ref._transform, nullptr);
			ref._part->renderMeshOnly();
		}
	}
}

// update visauls e.g. Skeletal animation
void DrawFrame::updateVisuals(float dt)
{
	for (unsigned int i = 0; i < _animatedSkeletons.size(); i++) {
		AnimatedSkeleton *ref = _animatedSkeletons[i];
		ref->update(dt);
	}
}

/* Get batched light info for the frame
*/
std::vector<PointLight>& DrawFrame::getLightInfo() {
	return _lightInfo;
}
/* Get batched grenade info for the frame
*/
std::vector<AntiLightValues>& DrawFrame::getGrenadeInfo() {
	return _grenadeInfo;
}

void DrawFrame::cullLightsByDistance(glm::vec3 &pos)
{
	if (_lightInfo.size() > 8)
	{
		unsigned long count = _lightInfo.size()-8;
		for (size_t i = 0; i < count; i++)
		{
			int largest = 0;
			float dist = glm::distance(pos, _lightInfo[0]._light._pos);
			for (size_t j = 1; j < _lightInfo.size(); j++)
			{
				float d = glm::distance(pos, _lightInfo[j]._light._pos);
				if (d > dist)
				{
					dist = d;
					largest = j;
				}
			}
			_lightInfo.erase(_lightInfo.begin()+largest);
		}
	}
}
