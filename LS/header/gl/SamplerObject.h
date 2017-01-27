#pragma once

#include"glInclude.h"
#include<memory>

namespace gl {

	/*	Container keeping a reference to a image loaded to the graphics device

	*/
	class SamplerObject
	{
	private:
		/*	The sampler reference
		*/
		GLuint _gSampler;
		/* Destroy the GL bound data
		*/
		void destroy();

	public:
		SamplerObject();
		/*	Construct a sampler object from the sampler object bound to the device
		*/
		SamplerObject(GLuint gSampler);
		/* Destructor
		*/
		virtual ~SamplerObject();
		/* Move the texture data
		*/
		SamplerObject(SamplerObject &&move);
		/* Move the texture data
		*/
		SamplerObject& operator=(SamplerObject &&move);

		/* Cant copy this data
		*/
		SamplerObject(const SamplerObject &copy) = delete;
		/* Cant copy this data
		*/
		SamplerObject& operator=(const SamplerObject &copy) = delete;

		/*	Binds the sampler parameters to the sampler slot.
		*/
		void bind(GLuint samplerSlot);
		/* Unbinds the sampler slot
		*/
		void clearBinding(GLuint samplerSlot);
	};

}
