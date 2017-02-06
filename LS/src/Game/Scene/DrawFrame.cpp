#include "Game/Scene/DrawFrame.h"



DrawFrame::DrawFrame()
{
}


DrawFrame::~DrawFrame()
{
}
void DrawFrame::add(const Model &m, const glm::mat4 &modelMatrix) {
	const std::vector<MeshPart> &parts = m.getParts();
	for (unsigned int i = 0; i < parts.size(); i++)
		_meshes.push_back(MeshDrawable(parts[i], modelMatrix));
}

void DrawFrame::add(const PointLight &light) {
	_lightInfo.push_back(light);
}

/* Render the frame
*/
void DrawFrame::render(RenderInfo &fD) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		ref._part->render(fD, *ref._transform);
	}
}


/* Get batched light info for the frame
*/
std::vector<PointLight>& DrawFrame::getLightInfo() {
	return _lightInfo;
}
