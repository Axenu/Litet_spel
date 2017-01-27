#include"gl/FrameBuffer.h"

namespace gl {

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	: _width(width), _height(height){
		//Frame buffer be assigned on implementation
	}

	FrameBuffer::~FrameBuffer()
	{
		destroyBuffer();
	}
	/* Destroys the all bound buffers.
	*/
	void FrameBuffer::destroyBuffer(){
		//Clear
		_frameBuffer = FBOData();
	}

	/* Sets the viewport
	*/
	void FrameBuffer::setViewport() const {
		glViewport(0, 0, _width, _height);
	}
	/* Sets the buffer as active draw frame buffer.
	*/
	void FrameBuffer::bind() const {
		//Bind the frame buffer for drawing.
		_frameBuffer.bind();
	}
	/* Enables the buffer for rendering. Sets viewport and clears the buffers.
	*/
	void FrameBuffer::bindDraw() const {
		setViewport();
		bind();
		clearBuffers();
	}
	/* Get the width of the render target
	*/
	unsigned int FrameBuffer::width() const{
		return _width;
	}
	/* Get the height of the render target
	*/
	unsigned int FrameBuffer::height() const {
		return _height;
	}

}