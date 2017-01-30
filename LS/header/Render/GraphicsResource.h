#pragma once
#include"gl/DeferredBuffer.h"
#include"gl/DefferedSettings.h"
#include"gl/RenderQuad.h"


	/* Class holding the graphical resources used for rendering.
	 * Not designed to hold assests.
	 */
class GraphicsResource {

	gl::RenderQuad _quad;
	/* Container of GL data used for deferred rendering
	*/
	gl::DeferredBuffer _deferred;

public:

	GraphicsResource(const gl::DefferredSettings &setting);
	~GraphicsResource();

	const gl::RenderQuad& getQuad() const;
	const gl::DeferredBuffer& getDeffered() const;
};
