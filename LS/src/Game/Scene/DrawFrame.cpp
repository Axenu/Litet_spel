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
	std::cout << _meshes.size() << std::endl;
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		ref._part->render(fD, *ref._transform);
	}
}

void DrawFrame::renderMeshOnly(GLint matrixLocation) {
	std::cout << _meshes.size() << std::endl;
	for (unsigned int i = 0; i < _meshes.size(); i++) {
		const MeshDrawable &ref = _meshes[i];
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &(*ref._transform)[0][0]);
		ref._part->renderMeshOnly();
	}
}


/* Get batched light info for the frame
*/
std::vector<PointLight>& DrawFrame::getLightInfo() {
	return _lightInfo;
}

void DrawFrame::cullLightsByDistance(glm::vec3 &pos)
{
	if (_lightInfo.size() > 8)
	{
		unsigned long count = _lightInfo.size()-8;
		for (size_t i = 0; i < count; i++)
		{
			int largest = 0;
			float dist = glm::distance(pos, _lightInfo[0]._pos);
			for (size_t j = 1; j < _lightInfo.size(); j++)
			{
				float d = glm::distance(pos, _lightInfo[j]._pos);
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
