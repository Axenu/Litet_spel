#include "Shader.h"
#include"gl\GLFunctions.h"

Shader::Shader() {
	shaderProgram = -1;
}

Shader::Shader(std::string file) : Shader(file, file) {
}

Shader::Shader(std::string vert, std::string frag) {
	/* Load shader, prints gl errors. If false is returned an error occured.
	*/
	if (!gl::loadShaderProgram("shaders/" + vert + ".vert", "shaders/" + frag + ".frag", shaderProgram)) {
		std::cout << "Shader failed to load" << std::endl;
	}
	name = vert;
	
}

Shader::Shader(std::string *vertexShader, std::string *fragmentShader) {
	GLuint vertShader, fragShader;
	gl::loadShaderString(*vertexShader, GL_VERTEX_SHADER, vertShader);
	gl::loadShaderString(*fragmentShader, GL_FRAGMENT_SHADER, fragShader);
	name = "Material shader";
	
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	
	glBindFragDataLocation(shaderProgram, 0, "Frag_Data");
	
	glLinkProgram(shaderProgram);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
	
	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::cout << "Shader program failed to link!" << std::endl;
		
		GLint infoLogLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete[] infoLog;
	}
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
    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}