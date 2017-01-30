#pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>
#include "camera.h"
#include "node.h"
#include "Render/MeshShader.h"
#include "Render/FrameData.h"

class Model : public Node {

private:
	GLint numberOfVertices;

	GLuint _VBO;
	GLuint _EBO;
	GLuint _VAO;

	MeshShader *_shader;
public:
	Model();
	Model(MeshShader &shader);


	void render(FrameData &fD) const;
	void update(float dt);

	~Model();

};
