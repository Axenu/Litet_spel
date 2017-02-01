#include"gl/GLFunctions.h"

#include<iostream>
#include<fstream>
#include<algorithm>

namespace gl {

	/*	Checks gl errors
	*/
	bool CheckGLErrors(int print)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR && print) {
			std::cout << "OpenGL Error: " + std::to_string(err) << std::endl;
			#ifndef __APPLE__
			std::string s((const char*)glewGetErrorString(err));
			std::cout << s << std::endl;
			#endif
			return true;
		}
		return false;
	}
	/*	Checks gl errors. Prints the log text before printing opengl error string.
	*/
	bool CheckGLErrors(const std::string& logText)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << logText << std::endl;
			std::cout << "OpenGL Error: " + std::to_string(err) << std::endl;
			#ifndef __APPLE__
			std::string s((const char*)glewGetErrorString(err));
			std::cout << s << std::endl;
			#endif
			return true;
		}
		return false;
	}
	/*Generates a buffer offset pointer*/
	void* bufferOffset(int i) {
		return (char *)nullptr + (i);
	}

#pragma region Generate VAO and load vertex & index buffers

	/* Generates a simple VAO object containing vertices constructed by a single vec3 element.
	vertexPositions	<<	Pointer to the vertex vec3 elements.
	vertexCount		<<	The vertex count.
	indices			<<	Pointer to the index data that will be sent to the gpu
	indexByteSize	<<	The byte size of each index, 2 byte for int16 and 4 byte for int32 (16 & 32 bit integers)
	indexCount		<<	The index count
	*/
	VAData generateVAO_Simple(const void* vertexPositions, int vertexCount, const void* indices, int indexByteSize, int indexCount) {

		//Vertices is the size of one vec3
		int vertexSize = 12;

		GLuint gVAO;
		// Vertex Array Object (VAO) 
		glGenVertexArrays(1, &gVAO);
		// bind == enable
		glBindVertexArray(gVAO);

		// Generate two buffer ids to be used as VBO and IBO
		std::vector<GLuint> gBuffer(2);
		glGenBuffers(2, &gBuffer[0]);


		// Bind the first buffer ID as an ARRAY_BUFFER, making it a vertex buffer object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, gBuffer[0]);
		// Sends the data to the GPU, depending on what the driver wants to do...
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertexPositions, GL_STATIC_DRAW);

		//Set attribute pointer
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, NULL);

		//Bind the second buffer as an GL_ELEMENT_ARRAY_BUFFER, making it a index buffer object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gBuffer[1]);
		//Send the indices to the gpu.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize * indexCount, indices, GL_STATIC_DRAW);
		//Unbind VOA
		glBindVertexArray(0);
		CheckGLErrors("Error generating and buffering VAO");
		return VAData(gVAO, gBuffer);
	}

	/*	Generate a vertex array object (VAO) and assigning a vertex and index buffer with the specified data.
	The Vertices will be assigned in AoS (Array of Structures) meaning each vertex struct are placed after eachother in the stream.

	vertexData		<<	Pointer to the vertex data that will be sent to the gpu
	attributes		<<	Array of vertex attribute definitions
	vertexCount		<<	The vertex count
	indices			<<	Pointer to the index data that will be sent to the gpu
	indexByteSize	<<	The byte size of each index, 2 byte for int16 and 4 byte for int32 (16 & 32 bit integers)
	indexCount		<<	The index count
	*/
	VAData generateVAO_AoS(const void* vertexData, std::vector<VertexAttribute> &vertexAttri, int vertexCount, const void* indices, int indexByteSize, int indexCount) {

		//Calculate the byte size/stride of each vertex from the vertex attributes
		int vertexSize = 0;
		for (unsigned int i = 0; i < vertexAttri.size(); i++)
			//Count the total size of each vertex
			vertexSize += vertexAttri[i].elementSize * vertexAttri[i].elementCount;


		GLuint gVAO;
		// Vertex Array Object (VAO) 
		glGenVertexArrays(1, &gVAO);
		// bind == enable
		glBindVertexArray(gVAO);


		// Generate two buffer ids to be used as VBO and IBO
		std::vector<GLuint> gBuffer(2);
		glGenBuffers(2, &gBuffer[0]);


		// Bind the first buffer ID as an ARRAY_BUFFER, making it a vertex buffer object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, gBuffer[0]);
		// Sends the data to the GPU, depending on what the driver wants to do...
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertexData, GL_STATIC_DRAW);

		//Set attribute pointers
		int offset = 0;
		for (unsigned int i = 0; i < vertexAttri.size(); i++) {

			//Enables the attribute "slot" in the VAO for each attribute
			glEnableVertexAttribArray(vertexAttri[i].attributeIndex);
			// specify that: the vertex attribute "vertexPos", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
			//               starts at offset 0 of the gVertexBuffer (it is implicitly bound!)
			glVertexAttribPointer(vertexAttri[i].attributeIndex, vertexAttri[i].elementCount, vertexAttri[i].elementType, vertexAttri[i].noormalize, vertexSize, bufferOffset(offset));
			offset += vertexAttri[i].elementCount *  vertexAttri[i].elementSize;
		}

		//Bind the second buffer as an GL_ELEMENT_ARRAY_BUFFER, making it a index buffer object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gBuffer[1]);
		//Send the indices to the gpu.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize * indexCount, indices, GL_STATIC_DRAW);
		//Unbind VOA
		glBindVertexArray(0);
		CheckGLErrors("Error generating and buffering VAO");
		return VAData(gVAO, gBuffer);
	}


	VAData generateVAO_SoA(const std::vector<const void*> &vertexData, std::vector<VertexAttribute> &vertexAttri, int vertexCount, const void* indices, int indexByteSize, int indexCount) {
		//http://ogldev.atspace.co.uk/www/tutorial32/tutorial32.html


		GLuint gVAO;
		// Vertex Array Object (VAO) 
		glGenVertexArrays(1, &gVAO);
		// bind == enable
		glBindVertexArray(gVAO);


		// Generate buffer ids to be used as VBO and IBO.
		std::vector<GLuint> gBuffers(vertexData.size() + 1); //+1 for index buffer
		glGenBuffers(gBuffers.size(), &gBuffers[0]);

		for (unsigned int i = 0; i < vertexData.size(); i++) {

			// Bind the first buffer ID as an ARRAY_BUFFER, making it a vertex buffer object (VBO)
			glBindBuffer(GL_ARRAY_BUFFER, gBuffers[i]);
			// Sends the data to the GPU, depending on what the driver wants to do...
			glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexAttri[i].elementCount * vertexAttri[i].elementSize, vertexData[i], GL_STATIC_DRAW);


			//Enables the attribute "slot" in the VAO for each attribute
			glEnableVertexAttribArray(vertexAttri[i].attributeIndex);
			// Specify the data array attribute. Describing what the data represents and layout identifier for opengl code 
			glVertexAttribPointer(vertexAttri[i].attributeIndex, vertexAttri[i].elementCount, vertexAttri[i].elementType, vertexAttri[i].noormalize, 0, 0);
		}

		//Bind the last buffer as an GL_ELEMENT_ARRAY_BUFFER, making it a index buffer object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gBuffers[vertexData.size()]);
		//Send the indices to the gpu.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize * indexCount, indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		CheckGLErrors("Error generating and buffering VAO");
		return VAData(gVAO, gBuffers);
	}

#pragma endregion

#pragma region Load & Debug Shader

	bool debugShader(GLuint shader) {
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) // the last compilation failed!
		{
			GLint msgLength = 0; // length of error message
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgLength);
			GLchar* msg = new GLchar[msgLength];
			glGetShaderInfoLog(shader, msgLength, NULL, msg);
			// print the error msg into the VS debug window,
			std::cout << msg;
			glDeleteShader(shader); // remove the shader as it has failed....
			delete[] msg;
			return false;
		}
		return true;
	}
	bool debugShaderProgram(GLuint shaderProgram) {

		GLint success = 0;
		glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) // the last compilation failed!
		{
			GLint msgLength = 0; // length of error message
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &msgLength);
			GLchar* msg = new GLchar[msgLength];
			glGetProgramInfoLog(shaderProgram, msgLength, NULL, msg);
			// print the error msg into the VS debug window,
			std::cout << "Shader program failed to link" << std::endl;//We print this since it can return empty string
			std::cout << msg << std::endl;
			glDeleteShader(shaderProgram); // remove the shader as it has failed....
			delete[] msg;
			return false;
		}
		return true;
	}

	/*Load a single shader part from a text file and returns the id.
		fileName	<<		The file to the shader text
		shaderType	<<		Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc)
		shaderID	>>		Id to the shader
		return		>>		If the single shader loaded successfully, errors will be logged */
	bool loadShader(std::string fileName, GLenum shaderType, GLuint &shaderID) {

		// Load .glsl file

		std::ifstream shaderFile;
		shaderFile.open(fileName);
		// Validate the file was opened
		if (!shaderFile.is_open()) {
			std::cout << "Shader file: " + fileName + " could not be found";
			return false;
		}
		// Put stream into a string
		std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		return loadShaderString(shaderText, shaderType, shaderID);
	}
	/*Load a single shader part from a string
	shader_str	<<		The shader string
	shaderType	<<		Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc)
	shaderID	>>		Id to the shader
	return		>>		If the single shader loaded successfully, errors will be logged */
	bool loadShaderString(std::string shader_str, GLenum shaderType, GLuint &shaderID) {

		// Create a shader
		shaderID = glCreateShader(shaderType);

		// Load the shader string to the gpu

		// make a double pointer (only valid here)
		const char* shaderTextPtr = shader_str.c_str();
		// ask GL to load this
		glShaderSource(shaderID, 1, &shaderTextPtr, nullptr);
		// ask GL to compile it
		glCompileShader(shaderID);
		//Debug
		return debugShader(shaderID);
	}

	/*Load a shader program with vertex and fragment shader
		vertexShader	<<		Directory to the vertex shader
		fragmentShader	<<		Directory to the fragmentShader
		shaderProgramID	>>		Reference for the shader program id
		return			>>		If the shader program loaded successfully, errors will be logged */
	bool loadShaderProgram(const std::string &vertexShader, const std::string &fragmentShader, GLuint &shaderProgramID) {

		GLuint vs, fs;
		if (!loadShader(vertexShader, GL_VERTEX_SHADER, vs))
			return false;
		if (!loadShader(fragmentShader, GL_FRAGMENT_SHADER, fs))
			return false;

		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, fs);
		glAttachShader(shaderProgramID, vs);
		glLinkProgram(shaderProgramID);

		return true;
		//return debugShaderProgram(shaderProgramID);
	}
	/*Load a shader program from a specified vertex and fragment shader code strings
	vertexShader	<<		Vertex shader code
	fragmentShader	<<		Fragment shader code
	shaderProgramID	>>		Reference for the shader program id created
	return			>>		If the shader program loaded successfully, errors will be logged */
	bool loadShaderProgramString(const std::string &vertexShader, const std::string &fragmentShader, GLuint &shaderProgramID) {

		GLuint vs, fs;
		if (!loadShaderString(vertexShader, GL_VERTEX_SHADER, vs))
			return false;
		if (!loadShaderString(fragmentShader, GL_FRAGMENT_SHADER, fs))
			return false;

		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, fs);
		glAttachShader(shaderProgramID, vs);
		glLinkProgram(shaderProgramID);

		return true;
		//return debugShaderProgram(shaderProgramID);
	}

#pragma endregion

#pragma region Bind Texture

	/*	Binds a texture of type GL_TEXTURE_2D to device from the specified data
	width			<<	Texture width
	height			<<	Texture height
	data			<<	Pointer to the data array
	samplingParam	<<	Sampling method for the texture. Default::GL_LINEAR
	edgeParam		<<	Wrapping parameter. Default::GL_REPEAT
	format			<<	Image pixel format. Default::GL_RGBA
	dataType		<<	Data type of the each channel/pixel. Default::GL_UNSIGNED_BYTE
	return			>>	The texture reference with the data bound to the device.
	*/
	TexData bind2DTexture(unsigned int width, unsigned int height, void* data, GLint samplingParam, GLenum edgeParam, GLint format, GLenum dataType) {

		//Generate a single texture id
		GLuint textureID;
		glGenTextures(1, &textureID); //Can generate an array of texture ids!

		//Binds the texture and all further changes will affect this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Send the image to the bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, dataType, data);

		//Filtering and wrap params:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edgeParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edgeParam);
		CheckGLErrors("Error generating a texture.");
		return TexData(textureID, GL_TEXTURE_2D);
	}

#pragma endregion

#pragma region Frame buffer

	/* Debug frame buffer
	*/
	bool debugFrameBuffer() {
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// Always check that our framebuffer is ok
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Deferred frame buffer failed to be generated: Error";
			switch (status) {
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cout << "GL_FRAMEBUFFER_UNDEFINED";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
				return false;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
				return false;
			default:
				std::cout << "Unkown error";
				return false;
			}
		}
		return true;
	}

#pragma region Shadow map


	/* Generate a depth buffer used for shadow mapping.
	width		<<	Shadow map width.
	height		<<	Shadow map height.
	frameBuffer	<<	Generated frame buffer.
	shadowMap	<<	Shadow map texture reference.
	*/
	bool generateShadowMap(unsigned int width, unsigned int height, FBOData &frameBuffer, TexData &shadowMap) {

		// Generate the depth texture
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		shadowMap = TexData(depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		//Bind it to frame buffer.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//Clamp to edge
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Generate FBO
		return generateDepthFBO(shadowMap, frameBuffer);
	}

	/* Generate a FBO with the depth texture attached to it
	tex			<<	Texture attached
	frameBuffer	<<	Generated frame buffer.
	*/
	bool generateDepthFBO(const TexData &tex, FBOData &frameBuffer) {

		//Generate and bind FBO
		GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//Bind texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex.texID(), 0);
		CheckGLErrors();

		glDrawBuffer(GL_NONE); //No Color buffer

		// Validate frame buffer
		if (!debugFrameBuffer()) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteBuffers(1, &fbo);
			return false;
		}

		//Unbind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		frameBuffer = FBOData(fbo);
		return true;
	}

#pragma endregion


#pragma region Deferred frame buffer

	void generateDeferredTexture(GLuint renderTarget, GLenum attachment, unsigned int width, unsigned int height) {
		/* Initiate the generated texture:
		*/
		glBindTexture(GL_TEXTURE_2D, renderTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// No filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		/*	Bind render targets to the frame buffer:
		*/
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, renderTarget, 0);
		CheckGLErrors();
	}

	/* Generates a frame buffer containing a set of render targets used for deferred rendering.
	colRenderTargets	<<	How many render targets generated using rgb colour.
	width				<<	Screen width.
	height				<<	Screen height.
	frameBuffer			<<	Generated frame buffer.
	renderTargets		<<	Generated render target textures. First item is the depth buffer, the following textures are the render targets in order of render slot.
	*/
	bool generateDeferredBuffers(unsigned int colRenderTargets, unsigned int width, unsigned int height, FBOData &frameBuffer, std::vector<TexData> &renderTextures) {

		unsigned int renderTargetCount = colRenderTargets + 1;
		renderTextures = std::vector<TexData>(renderTargetCount);

		/*Generate and bind framebuffer. The frame buffer will pack our rendertargets in a single object.
		*/
		GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Generate the textures we're going to render to
		std::vector<GLuint> renderTargets = std::vector<GLuint>(renderTargetCount);
		glGenTextures(renderTargetCount, &renderTargets[0]);


		// Generate and assign render buffers:
		std::vector<GLenum> DrawBuffers(colRenderTargets);
		for (unsigned int i = 1; i < renderTargetCount; i++) { //Begin at 1 since 0 is depthbuffer
			/* Generate texture:
			*/
			GLenum attachment = GL_COLOR_ATTACHMENT0 + i - 1;
			generateDeferredTexture(renderTargets[i], attachment, width, height);
			// Set render target textures as our colour attachement #i
			DrawBuffers[i - 1] = attachment;
			//Assign render targets to the texture array.
			renderTextures[i] = TexData(renderTargets[i], GL_TEXTURE_2D);
		}
		//Assigns layout index to our assigned buffers.
		glDrawBuffers(colRenderTargets, &DrawBuffers[0]);
		/* Generate and bind depth buffer to frame buffer
		*/
		glBindTexture(GL_TEXTURE_2D, renderTargets[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderTargets[0], 0);
		//Assign depthbuffer the first index of the texture array:
		renderTextures[0] = TexData(renderTargets[0], GL_TEXTURE_2D);
		CheckGLErrors("Error generating deferred buffer.");

		// Always check that our framebuffer is ok
		if (!debugFrameBuffer()) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteBuffers(1, &fbo);
			return false;
		}
		frameBuffer = FBOData(fbo);
		//Unbind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

#pragma endregion


#pragma endregion


	/* Generate a sampler object. Used to define sampler state of a sampler regadrless of the texture sampling state.
	filterParam	<<	The filtering parameter. GL_NEAREST, GL_LINEAR etc.
	edgeParam	<<	The edge wrap parameter. GL_REPEAT, GL_CLAMP_TO_EDGE etc.
	*/
	SamplerObject generateSamplerObject(GLenum filterParam, GLenum edgeParam) {
		GLuint sampler;
		glGenSamplers(1, &sampler);
		SamplerObject obj(sampler);
		//Assign parameters
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, filterParam);
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, filterParam);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, edgeParam);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, edgeParam);

		CheckGLErrors();
		return obj;
	}
	/* Generate a sampler object. Used to define sampler state of a sampler regadrless of the texture sampling state.
	minFilter	<<	The minification filtering parameter. GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST etc...
	magFilter	<<	The magnification filtering parameter. GL_NEAREST, GL_LINEAR etc.
	edgeParam	<<	The edge wrap parameter. GL_REPEAT, GL_CLAMP_TO_EDGE etc.
	*/
	SamplerObject generateSamplerObject(GLenum minFilter, GLenum magFilter, GLenum edgeParam) {
		GLuint sampler;
		glGenSamplers(1, &sampler);
		SamplerObject obj(sampler);
		//Assign parameters
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter);
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, edgeParam);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, edgeParam);

		CheckGLErrors();
		return obj;
	}


}
