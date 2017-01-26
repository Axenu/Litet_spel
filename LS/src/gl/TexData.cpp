#include "gl/TexData.h"

namespace gl {


	TexData::TexData()
	:	_gTexID(0), _targetTexType(0){

	}

	TexData::TexData(const GLuint gTexID, const GLenum targetTexType)
		: _gTexID(gTexID), _targetTexType(targetTexType)
	{
	}

	TexData::~TexData()
	{
		destroy();
	}
	/* Destroy the texture
	*/
	void TexData::destroy(){
		if (_gTexID != 0)
			glDeleteTextures(1, &_gTexID);
		_gTexID = 0;
	}
	/* Move constructor
	*/
	TexData::TexData(TexData &&move)
		: _gTexID(move._gTexID), _targetTexType(move._targetTexType){
		move._gTexID = 0;
	}
	/* Move operator
	*/
	TexData& TexData::operator=(TexData &&move){
		if (this != &move) {
			destroy();
			_gTexID = move._gTexID;
			_targetTexType = move._targetTexType;
			move._gTexID = 0;
		}
		return *this;
	}

	void TexData::bind() const{
		glBindTexture(_targetTexType, _gTexID);
	}
	void TexData::bind(const GLenum samplerSlot) const{
		glActiveTexture(samplerSlot);
		glBindTexture(_targetTexType, _gTexID);
	}


	/* Extract the texture id. Nullifying the container.
	*/
	GLuint TexData::extract(){
		GLuint texID = _gTexID;
		_gTexID = 0;
		return texID;
	}
	/* Fetch the graphics texture type.
	*/
	GLenum TexData::texType() const {
		return _targetTexType;
	}
	/* Get the texture ID.
	*/
	GLuint TexData::texID() const  {
		return _gTexID;
	}

}