#ifndef MESH_H
#define MESH_H

#include <gl/glInclude.h>
#include "gl/VAData.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include "AABB.h"


class Mesh
{
private:
	std::vector<glm::vec3> _position;
	std::vector<glm::vec3> _normal;
	std::vector<GLuint> _indices;
	gl::VAData _VA;
	AABB _aabb;
	void setUpMesh();
public:
	Mesh();
	Mesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices);
	~Mesh();
	/* Move the data
	*/
	Mesh(Mesh &&move);
	/* Move the data
	*/
	Mesh& operator=(Mesh &&move);

	/* Cant copy this data
	*/
	Mesh(const Mesh &copy) = delete;
	/* Cant copy this data
	*/
	Mesh& operator=(const Mesh &copy) = delete;
	void setMesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices);
	void render();
	bool pick(glm::vec3 origin, glm::vec3 dir);
};

#endif
