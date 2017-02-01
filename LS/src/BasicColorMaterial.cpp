#include "BasicColorMaterial.h"

BasicColorMaterial::BasicColorMaterial()
{

}

BasicColorMaterial::BasicColorMaterial(const BasicColorMaterial& other)
{

}

BasicColorMaterial::~BasicColorMaterial()
{

}

BasicColorMaterial::BasicColorMaterial(GLuint shaderProgram)
	:Material(shaderProgram)
{
	_vertexColorLocation = glGetUniformLocation(_shaderProgram, "u_Color");
}

void BasicColorMaterial::SetMaterialColor(float r, float g, float b, float a)
{
	_color.r = r;
	_color.g = g;
	_color.b = b;
	_color.a = a;
}

void BasicColorMaterial::SetUp()
{
	glUseProgram(_shaderProgram);
	glUniform4f(_vertexColorLocation, _color.r, _color.g, _color.b, _color.a);
}
