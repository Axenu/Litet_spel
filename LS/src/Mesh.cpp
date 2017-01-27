#include "Mesh.h"




void Mesh::setMesh(const std::vector<MVertex> vertices, const std::vector<GLuint> indices)
{

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices), 
}
