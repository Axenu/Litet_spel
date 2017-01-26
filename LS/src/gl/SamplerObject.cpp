#include"gl/SamplerObject.h"

namespace gl {


	SamplerObject::SamplerObject()
		: _gSampler(0){}

	SamplerObject::SamplerObject(GLuint gSampler)
		: _gSampler(gSampler)
	{}

	SamplerObject::~SamplerObject()
	{
		destroy();
	}
	/* Destroy the GL bound data
	*/
	void SamplerObject::destroy() {
		if (_gSampler != 0)
			glDeleteSamplers(1, &_gSampler);
	}
	/* Move constructor
	*/
	SamplerObject::SamplerObject(SamplerObject &&move)
		: _gSampler(move._gSampler){
		move._gSampler = 0;
	}
	/* Move operator
	*/
	SamplerObject& SamplerObject::operator=(SamplerObject &&move){
		if (this == &move)
			return *this;
		//Clean current:
		destroy();
		this->_gSampler = move._gSampler;
		move._gSampler = 0;
		return *this;
	}

	void SamplerObject::bind(GLuint samplerSlot){
		glBindSampler(samplerSlot, _gSampler);
	}
	/* Unbinds the sampler slot
	*/
	void SamplerObject::clearBinding(GLuint samplerSlot){
		glBindSampler(samplerSlot, 0);
	}

}

