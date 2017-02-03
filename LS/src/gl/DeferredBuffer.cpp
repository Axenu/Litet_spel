#include "gl/DeferredBuffer.h"

namespace gl {

	DeferredBuffer::DeferredBuffer(unsigned int screenWidth, unsigned int screenHeight, std::vector<DeferredTextureInfo> &setup)
		: FrameBuffer(screenWidth, screenHeight), _setup(std::move(setup))
	{
		generateBuffers();
	}
	/* Generate a deffered buffer from the specified settings.
	*/
	DeferredBuffer::DeferredBuffer(DefferredSettings& settings)
	 : DeferredBuffer(settings._windowWidth, settings._windowHeight, settings._textureSetup){}


	DeferredBuffer::~DeferredBuffer()
	{	}
	void DeferredBuffer::generateBuffers() {

		//Generate frame buffer
		if (!gl::generateDeferredBuffers(_setup, _width, _height, _frameBuffer, _renderTargets))
			throw new std::invalid_argument("Deferred frame buffer failed to be generated: Error");
	}
	/* Destroys the all bound buffers.
	*/
	void DeferredBuffer::destroyBuffers(){

		//Clear textures
		_renderTargets.clear();
		FrameBuffer::destroyBuffer();
	}

	/* Clears the buffers
	*/
	void DeferredBuffer::clearBuffers() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	/* Bind textures in sampler slots. Render targets will be assigned in order from GL_TEXTURE0. Depth buffer is assigned to the following slot.
	*/
	void DeferredBuffer::bindTextures() const{
		unsigned int i = 0;
		for (; i < _renderTargets.size() - 1; i++)
			_renderTargets[i + 1].bind(GL_TEXTURE0 + i);
		//Bind depth
		_renderTargets[0].bind(GL_TEXTURE0 + i);
	}
	/* Bind  render textures to samplers. Starting from the specifed sampler slot.
	Render targets will be assigned in order with the depth buffer being assigned to the last slot.
	samplerSlot	<<	Initial sampler slot render targets are bound to.
	*/
	void DeferredBuffer::bindTextures(GLenum samplerSlot) const{
		unsigned int i = 0;
		for (; i < _renderTargets.size() - 1; i++)
			_renderTargets[i + 1].bind(samplerSlot + i);
		//Bind depth
		_renderTargets[0].bind(samplerSlot + i);
	}



	/* Fetch a texture used by the buffer. Indexed as defined by use, first is depthmap.
	index	<<	Index of the texture.
	return	>>	Texture at the index.
	*/
	const TexData& DeferredBuffer::operator[](unsigned int index) const {
		return _renderTargets[index];
	}

	/* Get number of render targets. Including depth buffer.
	*/
	size_t DeferredBuffer::size() const {
		return _renderTargets.size();
	}

	/* Resize the frame buffer
	*/
	void  DeferredBuffer::resize(unsigned int screenWidth, unsigned int screenHeight){
		destroyBuffers();
		_width = screenWidth;
		_height = screenHeight;
		generateBuffers();
	}


}