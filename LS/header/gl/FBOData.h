#pragma once

#define GLEW_STATIC
#include<GL\glew.h>
#include<memory>

namespace gl {

	/*	Container keeping a reference to a frame buffer object loaded on the graphics device

	*/
	class FBOData
	{
	private:
		/*	The sampler reference
		*/
		GLuint _gFBO;
		/* Destroy the GL bound data
		*/
		void destroy();

	public:
		FBOData();
		/*	Construct a frame buffer object from the device link
		*/
		FBOData(GLuint gFBO);
		/* Destructor
		*/
		virtual ~FBOData();
		/* Move the data
		*/
		FBOData(FBOData &&move);
		/* Move the data
		*/
		FBOData& operator=(FBOData &&move);

		/* Cant copy this data
		*/
		FBOData(const FBOData &copy) = delete;
		/* Cant copy this data
		*/
		FBOData& operator=(const FBOData &copy) = delete;

		/*	Binds the frame buffer
		*/
		void bind() const;
		/* Unbinds current frame buffer and sets buffer 0
		*/
		void clearBinding() const;
		/* Get the FBO id
		*/
		GLuint getID() const;
	};

}
#pragma once
