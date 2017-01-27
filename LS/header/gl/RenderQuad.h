#pragma once

#include"GLFunctions.h"

namespace gl {

	/* A render quad used for applying a shader to the screen.
	*/
	class RenderQuad
	{
		GLuint _quadVAO;

	public:
		/* Load a simple render quad to the device
		*/
		RenderQuad();
		~RenderQuad();

		/* Render the screen quad
		*/
		void render() const;
	};

}

