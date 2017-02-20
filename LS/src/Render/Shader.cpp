#include "Render/Shader.h"
#include "gl/GLFunctions.h"

Shader::Shader()
{
	_shaderProgram = -1;
}

Shader::Shader(const std::string &file) : Shader(file, file)
{
}

Shader::Shader(const std::string &vert, const std::string &frag)
{
	/* Load shader, prints gl errors. If false is returned an error occured.
	*/
	if (!gl::loadShaderProgram("shaders/" + vert + ".vert", "shaders/" + frag + ".frag", _shaderProgram))
	{
		std::cout << "Shader failed to load: " + vert + ", " + frag << std::endl;
		//throw new std::exception("Shader load fail");
	}
	_name = vert + frag;
}

Shader::Shader(const std::string &vert, const std::string &geom, const std::string &frag)
{
	/* Load shader, prints gl errors. If false is returned an error occured.
	*/
	if (!gl::loadShaderProgram("shaders/" + vert + ".vert", "shaders/" + geom + ".geom", "shaders/" + frag + ".frag", _shaderProgram))
	{
		std::cout << "Shader failed to load: " + vert + ", " + frag << std::endl;
		//throw new std::exception("Shader load fail");
	}
	_name = vert + frag;
}

Shader::Shader(std::string *vertexShader, std::string *fragmentShader)
{
	if (gl::loadShaderProgramString(*vertexShader, *fragmentShader, _shaderProgram))
	{
		std::cout << "Shader failed to load from strings\n\nVertex:\n" + *vertexShader + "\n\nFragment:\n" + *fragmentShader << std::endl;
		//throw new std::exception("Shader load fail");
	}
	_name = "Material shader";
}

/* Move the Shader data
*/
Shader::Shader(Shader &&move)
 : _shaderProgram(move._shaderProgram), _name(move._name)
 {
	move._shaderProgram = 0;
}
/* Move the Shader data
*/
Shader& Shader::operator=(Shader &&move)
{
	if (this != &move)
	{
		return *this;
	}
	_name = move._name;
	_shaderProgram = move._shaderProgram;
	move._shaderProgram = 0;
	return *this;
}

void Shader::bind() const
{
	gl::CheckGLErrors("before glUseProgram");
	glUseProgram(_shaderProgram);
	gl::CheckGLErrors("after glUseProgram " + std::to_string(_shaderProgram));
}
/* Get a uniform from the shader. The shader needs to be bound to device.
*/
GLint Shader::getUniform(const std::string &varName)
{
	return glGetUniformLocation(_shaderProgram, &varName[0]);
}
/*	Queries the shader for a sampler uniform and binds it to the slot. Shaderprogram needs to be bound on device.
varName		<<	Variable name defined in the shader program
sampleSlot	<<	Sampler slot
return		>>	If the variable was found in the shader program
*/
bool Shader::bindSampler(const std::string &varName, int sampleSlot)
{
	GLint loc = glGetUniformLocation(_shaderProgram, &varName[0]);
	if (loc != -1)
	{
		glUniform1i(loc, sampleSlot); //Assigns sampling slot
		return true;
	}
	return false;
}

void Shader::printListOfUniforms()
{
    std::cout << "Printing program uniforms: " << std::endl;
    int total = -1;
    glGetProgramiv( _shaderProgram, GL_ACTIVE_UNIFORMS, &total );
    for(int i=0; i<total; ++i)
	{
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform( _shaderProgram, GLuint(i), sizeof(name)-1,
            &name_len, &num, &type, name );
        name[name_len] = 0;
        std::cout << name << std::endl;
        // GLuint location = glGetUniformLocation( shaderProgram, name );
    }
}

Shader::~Shader()
{
	if (_shaderProgram != 0)
	{
		glUseProgram(0);
		glDeleteProgram(_shaderProgram);
	}
}
