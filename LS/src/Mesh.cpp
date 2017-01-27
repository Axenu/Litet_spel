#include "Mesh.h"
#include "gl/GLFunctions.h"




void Mesh::setMesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices, int nrOfAttributes)
{
	_position = position;
	_normal = normal;
	_indices = indices;

	std::vector<gl::VertexAttribute> attri;
	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 3, sizeof(float))); //Pos attribute
	attri.push_back(gl::VertexAttribute(1, GL_FLOAT, 3, sizeof(float))); //Nor

	std::vector<const void*> vertexData;
	vertexData.push_back(&_position[0]);
	vertexData.push_back(&_normal[0]);

	_VAO = gl::generateVAO_SoA(vertexData, attri, _position.size(), &_indices[0], sizeof(_indices[0]), _indices.size());
	gl::CheckGLErrors();
}

void Mesh::render()
{
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, gl::bufferOffset(0));
}



