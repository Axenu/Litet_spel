#pragma once
#include"gl/DeferredBuffer.h"
#include"gl/CubeMapBuffer.h"
#include"gl/DefferedSettings.h"
#include"gl/RenderQuad.h"

	/* Class holding the graphical resources used for rendering.
	 * Not designed to hold assets?
	 */
class GraphicsResource {

	gl::RenderQuad _quad;
	/* Container of GL data used for deferred rendering
	*/
	gl::DeferredBuffer _deferred;
	gl::CubeMapBuffer _shadowMap[8];

	static const unsigned int SHADOW_WIDTH = 1024;
	static const unsigned int SHADOW_HEIGHT = 1024;

public:

	GraphicsResource(gl::DefferredSettings &setting);
	~GraphicsResource();

	const gl::RenderQuad& getQuad() const;
	const gl::DeferredBuffer& getDeffered() const;
	const gl::CubeMapBuffer& getCubeMap(int lightID) const;
};
