#include "Model.h"

Model::Model() {

	
}

Model::Model(MeshShader &shader)
	: _shader(&shader)
{

	GLfloat vertices[] = {
		0.0, 0.5, 0.0,
		0.5, 0.0, 0.0,
		-0.5, 0.0, 0.0
	};

	GLuint indices[] = {
		0, 1, 2
	};
	this->numberOfVertices = 9;


	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	int attributes = 3;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::render(FrameData &fD) const
{
	_shader->assignUniforms(fD);

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, this->numberOfVertices, GL_UNSIGNED_INT, NULL);
}

void Model::update(float dt) {
}

Model::~Model() {

}