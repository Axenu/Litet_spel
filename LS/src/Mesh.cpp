#include "Mesh.h"




void Mesh::setMesh(const std::vector<MVertex> vertices, const std::vector<GLuint> indices, int nrOfAttributes)
{
	_vertices = vertices;
	_indices = indices;

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices), &_vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &_EBO);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(_indices), &_indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nrOfAttributes * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, nrOfAttributes * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}
