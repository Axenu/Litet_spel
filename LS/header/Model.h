#pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#include <glm/glm.hpp>

#include <iostream>
#include "camera.h"
#include "node.h"

class Model : public Node {

private:
	GLint numberOfVertices;

	GLuint VBO;
	GLuint EBO;
	GLuint VAO;

	GLuint shaderID;
public:
	Model();
	Model(GLuint shaderID);


	void render() const;
	void update(float dt);

	~Model();

};
