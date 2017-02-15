#include"Render/GraphicsResource.h"


GraphicsResource::GraphicsResource(gl::DefferredSettings &setting)
: _quad(), _deferred(setting)
{
	// for (int i = 0; i < 8; i++)
	// {
	// 	_shadowMap[i].resize(SHADOW_WIDTH, SHADOW_HEIGHT);
	// }
}
GraphicsResource::~GraphicsResource()
{

}
const gl::RenderQuad& GraphicsResource::getQuad() const
{
	return _quad;
}
const gl::DeferredBuffer& GraphicsResource::getDeffered() const
{
	return _deferred;
}
const gl::CubeMapBuffer& GraphicsResource::getCubeMap(int lightID) const
{
	if (lightID > 7)
	{
		lightID = 7;
	}
	return _shadowMap[lightID];
}
