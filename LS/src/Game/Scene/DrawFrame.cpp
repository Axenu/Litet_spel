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

void DrawFrame::renderMeshOnly(RenderInfo &fD, GLint matrixLocation) {
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		//bind modelMatrix TODO
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &(*ref._transform)[0][0]);
		ref._part->renderMeshOnly();
	}
}


/* Get batched light info for the frame
*/
std::vector<PointLight>& DrawFrame::getLightInfo() {
	return _lightInfo;
}
