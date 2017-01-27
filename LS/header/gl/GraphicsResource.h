#pragma once
#include"DeferredBuffer.h"
#include"DefferedSettings.h"


namespace gl {
	/* Class holding the graphical resources used for rendering.
	 * Not designed to hold assests.
	 */
	class GraphicsResource {

		/* Container of GL data used for deferred rendering
		*/
		DeferredBuffer _deferred;

	public:

		GraphicsResource(const DefferredSettings &setting);
		~GraphicsResource();
	};
}