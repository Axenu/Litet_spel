#include"gl/VAData.h"

namespace gl {

	VAData::VAData()
		: _VAO(0), _glBuffer() {
	}
	VAData::VAData(GLuint VAO)
		: _VAO(VAO), _glBuffer() {
	}
	VAData::VAData(std::vector<GLuint> &buffers)
		: _VAO(0), _glBuffer(std::move(buffers)) {
	}
	VAData::VAData(GLuint VAO, std::vector<GLuint> &buffers)
		: _VAO(VAO), _glBuffer(std::move(buffers)) {
	}
	/* Destructor
	*/
	VAData::~VAData() {

	}
	void VAData::destroy() {
		//Delete VAO
		if (_VAO != 0) {
			clearBinding();
			glDeleteVertexArrays(1, &_VAO);
			_VAO = 0;
		}
		//Delete vertex buffers
		if (_glBuffer.size() > 0) {
			glDeleteBuffers(_glBuffer.size(), &_glBuffer[0]);
			_glBuffer.clear();
		}
	}
	/* Move the data
	*/
	VAData::VAData(VAData &&move)
		: _VAO(move._VAO), _glBuffer(std::move(move._glBuffer)) {
		move._VAO = 0;
	}
	/* Move the data
	*/
	VAData& VAData::operator=(VAData &&move) {
		if (this == &move)
			return *this;
		destroy();
		_VAO = move._VAO;
		_glBuffer = std::move(move._glBuffer);
		move._VAO = 0;
		return *this;
	}

	/*	Binds the VAO object
	*/
	void VAData::bindVAO() const {
		glBindVertexArray(_VAO);
	}
	/* Unbinds the VAO
	*/
	void VAData::clearBinding() const {
		glBindVertexArray(0);
	}

}
