#pragma once

#include <iostream>
#include "gl/glInclude.h"
#include <IL/il.h>

class Texture {
private:
	GLint _texture;

public:
	Texture();
	Texture(std::string path);
	~Texture();

    void bindTexture(int index);

	static GLuint createTexture(std::string path);
};
