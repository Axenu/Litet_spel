#pragma once

#include"glInclude.h"
#include<vector>
#include<string>

#include"TexData.h"
#include"FBOData.h"
#include"SamplerObject.h"
#include"VAData.h"

namespace gl {


	/* Checks for gl problem
	print <<	True if error message should be printed to log (Default: true)
	*/
	bool CheckGLErrors(int print = true);
	/*	Checks gl errors. Prints the log text before printing opengl error string.
	*/
	bool CheckGLErrors(const std::string& logText);
	/*Generates a buffer offset pointer*/
	void* bufferOffset(int i);


#pragma region Generate VAO and load vertex & index buffers
	/*Struct used to define vertex attributes when generating a VAO*/
	struct VertexAttribute {
		/*	Index for to attribute in the layout array.*/
		GLuint attributeIndex;
		/*Type of elements that creates the attribute*/
		GLenum elementType;
		/*The number of elements of the specified count that belongs to this attribute*/
		GLint elementCount;
		/*Byte size of a single element*/
		int elementSize;
		/*If a value is converted from int to float it will be converted into the [-1,1] range for signed and [0,1] for unsigned values (usefull for color conversion)*/
		GLboolean noormalize;

		VertexAttribute() {}

		/*Vertex attribute is used to identify the bytes of the vertices
			attributeIndex	<<	Layout index of the attribute in the shader.
			elementType		<<	Value type the attribute is constructed of (GL_FLOAT etc)
			elementCount	<<	How many elements of the specified type that belongs to the attribute (3 for a vec3 as it is made up of 3 GL_FLOAT etc)
			elementSize		<<	The byte size of the element type (4 for a GL_FLOAT etc)
			*/
		VertexAttribute(GLuint attributeIndex, GLenum elementType, GLint elementCount, int elementSize)
			: attributeIndex(attributeIndex), elementType(elementType), elementCount(elementCount), elementSize(elementSize), noormalize(GL_FALSE) {}
		/*Vertex attribute is used to identify the bytes of the vertices
			attributeIndex	<<	Layout index of the attribute in the shader.
			elementType		<<	Value type the attribute is constructed of (GL_FLOAT etc)
			elementCount	<<	How many elements of the specified type that belongs to the attribute (3 for a vec3 as it is made up of 3 GL_FLOAT etc)
			elementSize		<<	The byte size of the element type (4 for a GL_FLOAT etc)
			noormalize		<<	If convert integer values to floats normalize them to the [-1,1] range for signed and [0,1] for unsigned values (useful for color conversion)
		*/
		VertexAttribute(GLuint attributeIndex, GLenum elementType, GLint elementCount, int elementSize, GLboolean noormalize)
			: attributeIndex(attributeIndex), elementType(elementType), elementCount(elementCount), elementSize(elementSize), noormalize(noormalize) {}
	};

	/* Generates a simple VAO object containing vertices constructed by a single vec3 element.
	vertexPositions	<<	Pointer to the vertex vec3 elements.
	vertexCount		<<	The vertex count.
	indices			<<	Pointer to the index data that will be sent to the gpu
	indexByteSize	<<	The byte size of each index, 2 byte for int16 and 4 byte for int32 (16 & 32 bit integers)
	indexCount		<<	The index count
	*/
	VAData generateVAO_Simple(const void* vertexPositions, int vertexCount, const void* indices, int indexByteSize, int indexCount);

	/*	Generate a VAO by loading and assigning the vertex and index buffer with the specified data.
	 * The Vertices will be assigned in AoS (Array of Structures) meaning each vertex struct are placed after eachother in the stream.

	vertexData		<<	Pointer to the vertex data that will be sent to the gpu
	attributes		<<	Array of vertex attribute definitions
	vertexCount		<<	The vertex count
	indices			<<	Pointer to the index data that will be sent to the gpu
	indexByteSize	<<	The byte size of each index (Indices can either be constructed of int16 or int32)
	indexCount		<<	The index count
	*/
	VAData generateVAO_AoS(const void* vertexData, std::vector<VertexAttribute> &vertexAttri, int vertexCount, const void* indices, int indexByteSize, int indexCount);
	/* Generate a VAO by loading and assigning the vertex and index buffer with the specified data.
	 * The Vertices will be assigned in SoA (Structure of Arrays), where the vertex data is split into multiple arrays.
	vertexData		<<	List of vertex data arrays that will be sent to the gpu
	attributes		<<	Array of vertex attribute definitions
	vertexCount		<<	The vertex count
	indices			<<	Pointer to the index data that will be sent to the gpu
	indexByteSize	<<	The byte size of each index (Indices can either be constructed of int16 or int32)
	indexCount		<<	The index count
	*/
	VAData generateVAO_SoA(const std::vector< const void*> &vertexData, std::vector<VertexAttribute> &vertexAttri, int vertexCount, const void* indices, int indexByteSize, int indexCount);

#pragma endregion

#pragma region Load Shader

	/*Load a single shader part from a text file and returns the id.
	fileName	<<		The file to the shader text
	shaderType	<<		Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc)
	shaderID	>>		Id to the shader
	return		>>		If the single shader loaded successfully, errors will be logged */
	bool loadShader(std::string fileName, GLenum shaderType, GLuint &shaderID);

	/*Load a single shader part from a string
	shader_str	<<		The shader string
	shaderType	<<		Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc)
	shaderID	>>		Id to the shader
	return		>>		If the single shader loaded successfully, errors will be logged */
	bool loadShaderString(std::string shader_str, GLenum shaderType, GLuint &shaderID);

	/*Load a shader program with vertex and fragment shader
	vertexShader	<<		Directory to the vertex shader
	fragmentShader	<<		Directory to the fragmentShader
	shaderProgramID	>>		Reference for the shader program id
	return			>>		If the shader program loaded successfully, errors will be logged */
	bool loadShaderProgram(const std::string &vertexShader, const std::string &fragmentShader, GLuint &shaderProgramID);
	/*Load a shader program from a specified vertex and fragment shader code strings
	vertexShader	<<		Vertex shader code
	fragmentShader	<<		Fragment shader code
	shaderProgramID	>>		Reference for the shader program id created
	return			>>		If the shader program loaded successfully, errors will be logged */
	bool loadShaderProgramString(const std::string &vertexShader, const std::string &fragmentShader, GLuint &shaderProgramID);

#pragma endregion

#pragma region Generate and load frame buffers
	struct DeferredTextureInfo {
		/* Data format of the deferred texture. GL_RGB, GL_RGBA, etc. */
		GLenum _internalFormat;
		DeferredTextureInfo() 
		:	_internalFormat(GL_RGB){}
		DeferredTextureInfo(GLenum format)
			: _internalFormat(format) {}
	};
	/* Generates a frame buffer containing a set of render targets used for deferred rendering.
		colRenderTargets	<<	Determines the number of deferred textures generated and the 
		width				<<	Screen width.
		height				<<	Screen height.
		frameBuffer			<<	Generated frame buffer.
		renderTextures		<<	Generated render target textures. First item is the depth buffer, the following textures are the render targets in order of render slot.
	*/
	bool generateDeferredBuffers(const std::vector<DeferredTextureInfo> &colRenderTargets, unsigned int width, unsigned int height, FBOData &frameBuffer, std::vector<TexData> &renderTextures);

	/* Generate a depth buffer used for shadow mapping.
	width		<<	Shadow map width.
	height		<<	Shadow map height.
	frameBuffer	<<	Generated frame buffer.
	shadowMap	<<	Shadow map texture reference.
	*/
	bool generateShadowMap(unsigned int width, unsigned int height, FBOData &frameBuffer, TexData &shadowMap);


	/* Generate a FBO with the depth texture attached to it
	tex			<<	Texture attached
	frameBuffer	<<	Generated frame buffer.
	*/
	bool generateDepthFBO(const TexData &tex, FBOData &frameBuffer);
#pragma endregion


#pragma region Generate Textures

	/*	Binds a texture of type GL_TEXTURE_2D to device from the specified data
	width			<<	Texture width
	height			<<	Texture height
	data			<<	Pointer to the data array
	samplingParam	<<	Sampling method for the texture. Default::GL_LINEAR
	edgeParam		<<	Wrapping parameter. Default::GL_REPEAT
	format			<<	Image format of pixel 'data' param. Default::GL_RGBA
	dataType		<<	Data type of the pixel 'data' param. Default::GL_UNSIGNED_BYTE
	return			>>	The texture reference with the data bound to the device.
	*/
	TexData bind2DTexture(unsigned int width, unsigned int height, void* data, GLint samplingParam = GL_LINEAR, GLenum edgeParam = GL_REPEAT, GLint format = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE);


#pragma endregion

	/* Generate a sampler object. Used to define sampler state of a sampler regadrless of the texture sampling state.
	filterParam	<<	The filtering parameter. GL_NEAREST, GL_LINEAR etc.
	edgeParam	<<	The edge wrap parameter. GL_REPEAT, GL_CLAMP_TO_EDGE etc.
	*/
	SamplerObject generateSamplerObject(GLenum filterParam, GLenum edgeParam);
	/* Generate a sampler object. Used to define sampler state of a sampler regadrless of the texture sampling state.
	minFilter	<<	The minification filtering parameter. GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST etc...
	magFilter	<<	The magnification filtering parameter. GL_NEAREST, GL_LINEAR etc.
	edgeParam	<<	The edge wrap parameter. GL_REPEAT, GL_CLAMP_TO_EDGE etc.
	*/
	SamplerObject generateSamplerObject(GLenum minFilter, GLenum magFilter, GLenum edgeParam);

}