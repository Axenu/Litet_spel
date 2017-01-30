#include"Render/FrameData.h"

FrameData::FrameData(const GraphicsResource& resource, const Camera &cam)
	: _resource(resource), _V(cam.getViewMatrix()), _VP(cam.VPMatrix), _eye(cam.getPosition()) {

}
FrameData::~FrameData() {

}
