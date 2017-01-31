#ifndef MATERIAL_H
#define MATERIAL_H

#include <gl/GLFunctions.h>

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	Material(GLuint);
	void SetMaterialColor(GLuint, float, float, float, float);
};

#endif