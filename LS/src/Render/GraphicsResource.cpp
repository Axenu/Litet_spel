#include"Render/GraphicsResource.h"


GraphicsResource::GraphicsResource(const gl::DefferredSettings &setting)
: _quad(), _deferred(setting){

}
GraphicsResource::~GraphicsResource() {

}
const gl::RenderQuad& GraphicsResource::getQuad() const {
	return _quad;
}
const gl::DeferredBuffer& GraphicsResource::getDeffered() const {
	return _deferred;
}