#include"Render/RenderInfo.h"

RenderInfo::RenderInfo(const GraphicsResource& resource, const Camera &cam, std::vector<PointLight> &pLights, std::vector<AntiLightValues> &aLightInfo)
	: _resource(resource), _V(cam.getViewMatrix()), _VP(cam.VPMatrix), _eye(cam.getPosition()), _pLightInfo(pLights), _aLightInfo(aLightInfo){

}
RenderInfo::~RenderInfo() {

}
