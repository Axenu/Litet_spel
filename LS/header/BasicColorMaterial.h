#pragma once

#include <gl/GLFunctions.h>
#include <gl/glInclude.h>
#include <glm/glm.hpp>

#include "Material.h"

class BasicColorMaterial : public Material
{
public:
	BasicColorMaterial();
	BasicColorMaterial(const BasicColorMaterial&);
	virtual ~BasicColorMaterial();
	BasicColorMaterial(GLuint);
	void SetMaterialColor(float, float, float, float);
	void SetUp();
private:
	GLuint _vertexColorLocation;
	glm::vec4 _color;
};

