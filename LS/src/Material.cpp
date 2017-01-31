#include "Material.h"

Material::Material()
{

}

Material::Material(const Material& other)
{

}

Material::~Material()
{

}

Material::Material(GLuint shaderProgram)
{
	GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
	
	glUseProgram(shaderProgram);
	
	glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
}

void Material::SetMaterialColor(GLuint shaderProgram, float r, float g, float b, float a)
{
	GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
	
	glUseProgram(shaderProgram);

	glUniform4f(vertexColorLocation, r, g, b, a);
}
