#pragma once
#ifndef MESH_H
#define MESH_H

#include <gl/glInclude.h>
#include <vector>

struct MVertex
{
	MVector pos;
	MVector normal;
};

struct MVector
{
	GLfloat x, y, z;
};

class Mesh
{
private:
	std::vector<MVertex> _vertices;
	std::vector<GLuint> _indices;
	GLuint _VAO, _VBO, _EBO;
	
public:
	void setMesh(const std::vector<MVertex> vertices, const std::vector<GLuint> indices);

};

#endif
