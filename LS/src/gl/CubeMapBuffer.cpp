#include "gl/CubeMapBuffer.h"

namespace gl {

    CubeMapBuffer::CubeMapBuffer()
		: CubeMapBuffer(512, 512)
	{
	}

	CubeMapBuffer::CubeMapBuffer(unsigned int width, unsigned int height)
		: FrameBuffer(width, height)
	{
		generateBuffers();
	}


	CubeMapBuffer::~CubeMapBuffer()
	{

    }
	void CubeMapBuffer::generateBuffers()
    {

        GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);


        GLuint depthCubemap;
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (GLuint i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        _renderTargets = std::vector<TexData>(1);
        _renderTargets[0] = TexData(depthCubemap, GL_TEXTURE_CUBE_MAP);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        _frameBuffer = FBOData(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CheckGLErrors("Error generating CubeMap buffer.");

		//Generate frame buffer
		// if (!gl::generateDeferredBuffers(_setup, _width, _height, _frameBuffer, _renderTargets))
		// 	throw new std::invalid_argument("Deferred frame buffer failed to be generated: Error");
	}
	/* Destroys the all bound buffers.
	*/
	void CubeMapBuffer::destroyBuffers()
    {

		//Clear textures
		_renderTargets.clear();
		FrameBuffer::destroyBuffer();
	}

	/* Clears the buffers
	*/
	void CubeMapBuffer::clearBuffers() const
    {
		glClear(GL_DEPTH_BUFFER_BIT);
	}


	/* Bind textures in sampler slots. Render targets will be assigned in order from GL_TEXTURE0. Depth buffer is assigned to the following slot.
	*/
	void CubeMapBuffer::bindTextures() const
    {
        _renderTargets[0].bind(GL_TEXTURE0);
	}
	/* Bind  render textures to samplers. Starting from the specifed sampler slot.
	Render targets will be assigned in order with the depth buffer being assigned to the last slot.
	samplerSlot	<<	Initial sampler slot render targets are bound to.
	*/
	void CubeMapBuffer::bindTextures(GLenum samplerSlot) const
    {
		_renderTargets[0].bind(samplerSlot);
	}

	/* Fetch a texture used by the buffer. Indexed as defined by use, first is depthmap.
	index	<<	Index of the texture.
	return	>>	Texture at the index.
	*/
	const TexData& CubeMapBuffer::operator[](unsigned int index) const
    {
		return _renderTargets[index];
	}

	/* Get number of render targets. Including depth buffer.
	*/
	size_t CubeMapBuffer::size() const
    {
		return _renderTargets.size();
	}

	/* Resize the frame buffer
	*/
	void  CubeMapBuffer::resize(unsigned int screenWidth, unsigned int screenHeight)
    {
		destroyBuffers();
		_width = screenWidth;
		_height = screenHeight;
		generateBuffers();
	}


}
