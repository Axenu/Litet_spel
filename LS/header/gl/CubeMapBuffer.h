#pragma once

#include <iostream>
#include "GLFunctions.h"
#include "FrameBuffer.h"
#include "DefferedSettings.h"
#include "Config.h"

namespace gl{


	/* Deferred frame buffer. Container and generater of a deferred frame buffer.
	*/
	class CubeMapBuffer : public FrameBuffer
	{
	private:
		/* The render target textures.
		0 is the depth buffer. [1, size) is the color texture assigned in framebuffer slot [0, size - 1)
		*/
		std::vector<TexData> _renderTargets;

		/* Generate new buffers
		*/
		virtual void generateBuffers();
	protected:
		/* Destroys the all bound buffers. Called from destructor and resize.
		*/
		virtual void destroyBuffers();

	public:
        CubeMapBuffer();
		CubeMapBuffer(unsigned int screenWidth, unsigned int screenHeight);
		~CubeMapBuffer();

		/* Clears the buffers
		*/
		virtual void clearBuffers() const;

		/* Bind textures in sampler slots. Render targets will be assigned in order from GL_TEXTURE0. Depth buffer is assigned to the following slot.
		*/
		virtual void bindTextures() const;
		/* Bind  render textures to samplers. Starting from the specifed sampler slot.
		Render targets will be assigned in order with the depth buffer being assigned to the last slot.
		samplerSlot	<<	Initial sampler slot render targets are bound to.
		*/
		virtual void bindTextures(GLenum samplerSlot) const;

		/* Resize the frame buffer
		*/
		virtual void resize(unsigned int screenWidth, unsigned int screenHeight);

		/* Get number of render targets. Including depth buffer.
		*/
		size_t size() const;

		/* Fetch a texture used by the buffer. Indexed as used, last is depthmap.
		index	<<	Index of the texture.
		return	>>	Texture at the index.
		*/
		const TexData& operator[](unsigned int index) const;

	};

}
