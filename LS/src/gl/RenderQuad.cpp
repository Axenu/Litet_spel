#include"gl/RenderQuad.h"

#include"glm/vec3.hpp"

namespace gl{

	const static glm::vec3 QUAD_VERTS[4] = {
		{ -1, -1, 0 }, { 1, -1, 0 }, { 1, 1, 0 }, { -1, 1, 0 }
	};
	const static int QUAD_INDICES[6] = {
		0, 1, 2, 2, 3, 0
	};

	RenderQuad::RenderQuad()
		: _quadVAO(gl::generateVAO_Simple((void*)QUAD_VERTS, 4, (void*)QUAD_INDICES, 4, 6))
	{

	}


	RenderQuad::~RenderQuad()
	{
	}

	void RenderQuad::render() const {
		//Bind and render quad
		glBindVertexArray(_quadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	}

}
