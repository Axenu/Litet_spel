#include"Render/RenderInfo.h"

RenderInfo::RenderInfo(const GraphicsResource& resource, const Camera &cam, std::vector<PointLight> &pLights)
	: _resource(resource), _V(cam.getViewMatrix()), _VP(cam.VPMatrix), _eye(cam.getPosition()), _pLightInfo(pLights) {

}
RenderInfo::~RenderInfo() {

}
