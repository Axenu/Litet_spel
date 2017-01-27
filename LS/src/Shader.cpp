#include "Shader.h"
#include"gl\GLFunctions.h"

Shader::Shader() {
	shaderProgram = -1;
}

Shader::Shader(const std::string &file) : Shader(file, file) {
}

Shader::Shader(const std::string &vert, const std::string &frag) {
	/* Load shader, prints gl errors. If false is returned an error occured.
	*/
	if (!gl::loadShaderProgram("shaders/" + vert + ".vert", "shaders/" + frag + ".frag", shaderProgram)) {
		std::cout << "Shader failed to load: " + vert + ", " + frag << std::endl;
		//throw new std::exception("Shader load fail");
	}
	name = vert;
	
}

Shader::Shader(std::string *vertexShader, std::string *fragmentShader) {
	if (gl::loadShaderProgramString(*vertexShader, *fragmentShader, shaderProgram)) {
		std::cout << "Shader failed to load from strings\n\nVertex:\n" + *vertexShader + "\n\nFragment:\n" + *fragmentShader << std::endl;
		//throw new std::exception("Shader load fail");
	}
	name = "Material shader";
}

/* Move the Shader data
*/
Shader::Shader(Shader &&move)
 : shaderProgram(move.shaderProgram), name(move.name){
	move.shaderProgram = 0;
}
/* Move the Shader data
*/
Shader& Shader::operator=(Shader &&move) {
	if (this != &move)
		return *this;
	this->name = move.name;
	this->shaderProgram = move.shaderProgram;
	move.shaderProgram = 0;
	return *this;
}

void Shader::bind() {
	glUseProgram(this->shaderProgram);
}
/* Get a uniform from the shader. The shader needs to be bound to device.
*/
GLint Shader::getUniform(const std::string &varName) {
	return glGetUniformLocation(shaderProgram, &varName[0]);
}
/*	Queries the shader for a sampler uniform and binds it to the slot. Shaderprogram needs to be bound on device.
varName		<<	Variable name defined in the shader program
sampleSlot	<<	Sampler slot
return		>>	If the variable was found in the shader program
*/
bool Shader::bindSampler(const std::string &varName, int sampleSlot) {
	GLint loc = glGetUniformLocation(shaderProgram, &varName[0]);
	if (loc != -1) {
		glUniform1i(loc, sampleSlot); //Assigns sampling slot
		return true;
	}
	return false;
}

void Shader::printListOfUniforms() {
    std::cout << "Printing program uniforms: " << std::endl;
    int total = -1;
    glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORMS, &total ); 
    for(int i=0; i<total; ++i)  {
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform( shaderProgram, GLuint(i), sizeof(name)-1,
            &name_len, &num, &type, name );
        name[name_len] = 0;
        std::cout << name << std::endl;
        // GLuint location = glGetUniformLocation( shaderProgram, name );
    }
}

GLint Shader::createShader(std::string path, GLenum shaderType) const {
    std::fstream fin;
    GLuint shaderID = glCreateShader(shaderType);
    
    // Vertex Shader
    fin.open(path);
    if(!fin.is_open()) {
        fin.close();
        std::cout << "File not found: '" << path << "'" << std::endl;
        return 0;
    }
    
    std::string source((std::istreambuf_iterator<GLchar>(fin)), std::istreambuf_iterator<GLchar>());
    fin.close();
    
    const GLchar *shaderSource = source.c_str();
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    
    glCompileShader(shaderID);
    
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        std::cout << "Shader failed to compile: '" << path << "'" << std::endl;
        std::cout << source << std::endl;
        
        GLint infoLogLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<GLchar> errorLog(infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, &errorLog[0]);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        return 0;
    }
    return shaderID;
}

GLint Shader::createShaderFromString(std::string *shader, GLenum shaderType) const {
	GLuint shaderID = glCreateShader(shaderType);
    
    const GLchar *shaderSource = shader->c_str();
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    
    glCompileShader(shaderID);
    
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        std::cout << "Shader failed to compile: '" << shader << "'" << std::endl;
        
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        return 0;
    }
    return shaderID;
}

Shader::~Shader() {
	if (shaderProgram != 0) {
		glUseProgram(0);
		glDeleteProgram(shaderProgram);
	}
}