#include "Render/Mesh/Mesh.h"
#include "gl/GLFunctions.h"
#include "math/intersectionFunctions.h"

//Private
void Mesh::setUpMesh(const std::vector<glm::vec3> &normals)
{
	_aabb = AABB(&_position);
	std::vector<gl::VertexAttribute> attri;
	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 3, sizeof(float))); //Pos attribute
	attri.push_back(gl::VertexAttribute(1, GL_FLOAT, 3, sizeof(float))); //Norm attribute
	std::vector<const void*> vertexData;

	vertexData.push_back(&_position[0]);	//Get position array start pointer
	vertexData.push_back(&normals[0]);		//Get normal array start pointer
	_VA = gl::generateVAO_SoA(vertexData, attri, _position.size(), &_indices[0], sizeof(_indices[0]), _indices.size()); // Create VAO
	
	gl::CheckGLErrors();
}



//Public
Mesh::Mesh()
{
	//Create base cube
	//Front
	_position.push_back(glm::vec3(1, 1, 0));
	_position.push_back(glm::vec3(0, 1, 0));
	_position.push_back(glm::vec3(1, 0, 0));
	_position.push_back(glm::vec3(0, 0, 0));

	std::vector<glm::vec3> normal;
	normal.push_back(glm::vec3(0, 0, -1));
	normal.push_back(glm::vec3(0, 0, -1));
	normal.push_back(glm::vec3(0, 0, -1));
	normal.push_back(glm::vec3(0, 0, -1));

	_indices.push_back(GLuint(1));
	_indices.push_back(GLuint(0));
	_indices.push_back(GLuint(2));
	_indices.push_back(GLuint(1));
	_indices.push_back(GLuint(2));
	_indices.push_back(GLuint(3));

	//Back
	_position.push_back(glm::vec3(0, 1, 1));
	_position.push_back(glm::vec3(1, 1, 1));
	_position.push_back(glm::vec3(0, 0, 1));
	_position.push_back(glm::vec3(1, 0, 1));

	normal.push_back(glm::vec3(0, 0, 1));
	normal.push_back(glm::vec3(0, 0, 1));
	normal.push_back(glm::vec3(0, 0, 1));
	normal.push_back(glm::vec3(0, 0, 1));

	_indices.push_back(GLuint(5));
	_indices.push_back(GLuint(4));
	_indices.push_back(GLuint(6));
	_indices.push_back(GLuint(5));
	_indices.push_back(GLuint(6));
	_indices.push_back(GLuint(7));

	//Right
	_position.push_back(glm::vec3(0, 1, 0));
	_position.push_back(glm::vec3(0, 1, 1));
	_position.push_back(glm::vec3(0, 0, 0));
	_position.push_back(glm::vec3(0, 0, 1));

	normal.push_back(glm::vec3(-1, 0, 0));
	normal.push_back(glm::vec3(-1, 0, 0));
	normal.push_back(glm::vec3(-1, 0, 0));
	normal.push_back(glm::vec3(-1, 0, 0));

	_indices.push_back(GLuint(9));
	_indices.push_back(GLuint(8));
	_indices.push_back(GLuint(10));
	_indices.push_back(GLuint(9));
	_indices.push_back(GLuint(10));
	_indices.push_back(GLuint(11));

	//Left
	_position.push_back(glm::vec3(1, 1, 1));
	_position.push_back(glm::vec3(1, 1, 0));
	_position.push_back(glm::vec3(1, 0, 1));
	_position.push_back(glm::vec3(1, 0, 0));

	normal.push_back(glm::vec3(1, 0, 0));
	normal.push_back(glm::vec3(1, 0, 0));
	normal.push_back(glm::vec3(1, 0, 0));
	normal.push_back(glm::vec3(1, 0, 0));

	_indices.push_back(GLuint(13));
	_indices.push_back(GLuint(12));
	_indices.push_back(GLuint(14));
	_indices.push_back(GLuint(13));
	_indices.push_back(GLuint(14));
	_indices.push_back(GLuint(15));
	//Top
	_position.push_back(glm::vec3(1, 1, 1));
	_position.push_back(glm::vec3(0, 1, 1));
	_position.push_back(glm::vec3(1, 1, 0));
	_position.push_back(glm::vec3(0, 1, 0));

	normal.push_back(glm::vec3(0, 1, 0));
	normal.push_back(glm::vec3(0, 1, 0));
	normal.push_back(glm::vec3(0, 1, 0));
	normal.push_back(glm::vec3(0, 1, 0));

	_indices.push_back(GLuint(17));
	_indices.push_back(GLuint(16));
	_indices.push_back(GLuint(18));
	_indices.push_back(GLuint(17));
	_indices.push_back(GLuint(18));
	_indices.push_back(GLuint(19));
	//Bottom
	_position.push_back(glm::vec3(1, 0, 0));
	_position.push_back(glm::vec3(0, 0, 0));
	_position.push_back(glm::vec3(1, 0, 1));
	_position.push_back(glm::vec3(0, 0, 1));

	normal.push_back(glm::vec3(0, -1, 0));
	normal.push_back(glm::vec3(0, -1, 0));
	normal.push_back(glm::vec3(0, -1, 0));
	normal.push_back(glm::vec3(0, -1, 0));

	_indices.push_back(GLuint(21));
	_indices.push_back(GLuint(20));
	_indices.push_back(GLuint(22));
	_indices.push_back(GLuint(21));
	_indices.push_back(GLuint(22));
	_indices.push_back(GLuint(23));

	this->setUpMesh(normal);
}

Mesh::Mesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices)
{
	this->setMesh(position, normal, indices);
}
Mesh::Mesh(std::vector<glm::vec3> &position, std::vector<GLuint> &indices, gl::VAData &vaData)
	: _position(std::move(position)), _indices(std::move(indices)), _VA(std::move(vaData)), _aabb(&_position), _skeleton()
{
}
Mesh::Mesh(std::vector<glm::vec3> &position, std::vector<GLuint> &indices, gl::VAData &vaData, SkeletonPart &skeleton)
	: _position(std::move(position)), _indices(std::move(indices)), _VA(std::move(vaData)), _aabb(&_position), _skeleton(std::move(skeleton))
{
}
Mesh::~Mesh()
{
}
/* Move the data
*/
Mesh::Mesh(Mesh &&move)
	: _position(std::move(move._position)), _indices(std::move(move._indices)), _VA(std::move( move._VA))
{
	move._VA = 0;
	_aabb = move._aabb;
}
/* Move the data
*/
Mesh& Mesh::operator=(Mesh &&move) {
	if (this == &move)
		return *this;
	_position = std::move(move._position);
	_indices = std::move(move._indices);
	_VA = std::move(move._VA);
	return *this;
}


void Mesh::setMesh(const std::vector<glm::vec3> &position, const std::vector<glm::vec3> &normal, const std::vector<GLuint> &indices)
{
	_position = position;
	_indices = indices;

	this->setUpMesh(normal);
}

void Mesh::render()
{
	_VA.bindVAO();
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, gl::bufferOffset(0));
}
const AABB& Mesh::getBox() const {
	return _aabb;
}
const SkeletonPart& Mesh::getSkeletonPart() const {
	return _skeleton;
}

bool Mesh::pick(glm::vec3 origin, glm::vec3 dir) const
{
	//picking in modelspace
	float dist;
	if (AABBIntersect(_aabb, dir, origin, dist))
	{
		glm::vec3 tri1, tri2, tri3;
		for (unsigned int i = 0; i < _indices.size(); i += 3)
		{
			tri1 = _position[_indices[i]];
			tri2 = _position[_indices[i + 1]];
			tri3 = _position[_indices[i + 2]];
			if (TriangleIntersection(tri1, tri2, tri3, origin, dir))
			{
				return true;
			}
		}
	}

	return false;
}

