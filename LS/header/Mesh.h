#pragma once
#ifndef MESH_H
#define MESH_H

#include <gl/glInclude.h>
#include <glm/glm.hpp>
#include <vector>


class Mesh
{
private:
	std::vector<glm::vec3> _position;
	std::vector<glm::vec3> _normal;
	std::vector<GLuint> _indices;
	GLuint _VAO;
	
public:
	void setMesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices, int nrOfAttributes);
	void render();
};

#endif
