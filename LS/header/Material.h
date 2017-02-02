#pragma once

#include <gl/glInclude.h>

class Material
{
public:
	Material();
	Material(GLuint shaderProgram);
	virtual ~Material();
	virtual void SetUp() {};
protected:
	GLuint _shaderProgram;
};