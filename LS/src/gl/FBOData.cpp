#include"gl/FBOData.h"

namespace gl {


	FBOData::FBOData()
		: _gFBO(0) {}

	FBOData::FBOData(GLuint gFBO)
		: _gFBO(gFBO)
	{}

	FBOData::~FBOData()
	{
		destroy();
	}
	/* Destroy the GL bound data
	*/
	void FBOData::destroy() {
		if (_gFBO != 0) {
			//Make sure frame buffer is not active then delete
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &_gFBO);
		}
	}
	/* Move constructor
	*/
	FBOData::FBOData(FBOData &&move)
		: _gFBO(move._gFBO) {
		move._gFBO = 0;
	}
	/* Move operator
	*/
	FBOData& FBOData::operator=(FBOData &&move) {
		if (this == &move)
			return *this;
		//Clean current:
		destroy();
		this->_gFBO = move._gFBO;
		move._gFBO = 0;
		return *this;
	}

	/*	Binds the frame buffer
	*/
	void FBOData::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, _gFBO);
	}
	/* Unbinds current frame buffer and sets buffer 0
	*/
	void FBOData::clearBinding() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	/* Get the FBO id
	*/
	GLuint FBOData::getID() const {
		return _gFBO;
	}

}
