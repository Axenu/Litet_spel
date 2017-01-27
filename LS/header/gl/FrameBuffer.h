#pragma once
#include"GLFunctions.h"
#include"FBOData.h"

namespace gl {

	/* Frame buffer function collection. Abstract object where implementations will need to load and assign the frame buffer object.

	Contract:
	Implementation must assign the frame buffer value on construction
	*/
	class FrameBuffer{
	protected:

		/* Width of the render target
		*/
		unsigned int _width;
		/* Height of the render target
		*/
		unsigned int _height;
		/* Frame buffer
		*/
		FBOData _frameBuffer;

		/* Destroys the all bound buffers. Called from destructor and resize.
		*/
		virtual void destroyBuffer();
	public:
		/* Sets the viewport
		*/
		virtual void setViewport() const;
		/* Sets the buffer as active draw frame buffer.
		*/
		virtual void bind() const;
		/* Sets the buffer as active draw frame buffer. Also sets viewport
		*/
		virtual void bindDraw() const;

		/* Construct a frame buffer. The frame buffer binding must be assigned from implementation
		width	<<	Width of the frame buffer
		height	<<	Height of the frame buffer
		*/
		FrameBuffer(unsigned int width, unsigned int height);
		virtual ~FrameBuffer();

		/* Clears the buffers
		*/
		virtual void clearBuffers() const = 0;

		/* Bind render textures in sampler slots. Render texture will be assigned in order starting in sampler 0.
		*/
		virtual void bindTextures() const = 0;
		/* Bind  render textures to samplers. Render textures will be assigned in order starting with the specifed sampler.
		samplerSlot	<<	Sampler to bind the shadow depth map to.
		*/
		virtual void bindTextures(GLenum samplerSlot) const = 0;

		/* Resize the frame buffer
		*/
		virtual void resize(unsigned int width, unsigned int height) = 0;
		/* Get the width of the render target
		*/
		unsigned int width() const;
		/* Get the height of the render target
		*/
		unsigned int height() const;

	};
}