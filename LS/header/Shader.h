#ifndef __First__Shader__
#define __First__Shader__

#include "gl/glInclude.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include"Render/FrameData.h"

class Shader {

private:
	std::string name;
	GLint createShader(std::string path, GLenum shaderType) const;
	GLint createShaderFromString(std::string *shader, GLenum shaderType) const;
public:

	GLuint shaderProgram;

	Shader();
	Shader(const std::string &file);
	Shader(const std::string &vert, const std::string &frag);
	Shader(std::string *vertexShader, std::string *fragmentShader);
	~Shader();
	/* Move the Shader data
	*/
	Shader(Shader &&move);
	/* Move the Shader data
	*/
	Shader& operator=(Shader &&move);

	/* Cant copy this data
	*/
	Shader(const Shader &copy) = delete;
	/* Cant copy this data
	*/
	Shader& operator=(const Shader &copy) = delete;

	void bind();
	/* Get a uniform from the shader. The shader needs to be bound to device.
	*/
	GLint getUniform(const std::string &varName);
	/*	Queries the shader for a sampler uniform and binds it to the slot. Shaderprogram needs to be bound on device.
	varName		<<	Variable name defined in the shader program
	sampleSlot	<<	Sampler slot
	return		>>	If the variable was found in the shader program
	*/
	bool bindSampler(const std::string &varName, int sampleSlot);

	void printListOfUniforms();
};

#endif
