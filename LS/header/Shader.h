#ifndef __First__Shader__
#define __First__Shader__

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Shader {

private:
	std::string name;
	GLint createShader(std::string path, GLenum shaderType) const;
	GLint createShaderFromString(std::string *shader, GLenum shaderType) const;
public:

	GLuint shaderProgram;
	
	Shader();
	Shader(std::string file);
	Shader(std::string vert, std::string frag);
	Shader(std::string *vertexShader, std::string *fragmentShader);
	~Shader();

	void printListOfUniforms();
};

#endif