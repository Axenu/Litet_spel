#pragma once
#ifndef MESH_H
#define MESH_H

#include <gl/glInclude.h>
#include <glm/glm.hpp>
#include <vector>

struct MVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
};

class Mesh
{
private:
	std::vector<MVertex> _vertices;
	std::vector<GLuint> _indices;
	GLuint _VAO, _VBO, _EBO;
	
public:
	void setMesh(const std::vector<MVertex> vertices, const std::vector<GLuint> indices, int nrOfAttributes);

};

#endif
