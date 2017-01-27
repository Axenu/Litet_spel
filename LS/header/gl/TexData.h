#pragma once

#include "gl/glInclude.h"
#include<memory>

namespace gl {

	/*	Container keeping a reference to a image loaded to the graphics device

	*/
	class TexData
	{
	protected:
		/*	The texture data reference ID on the device
		*/
		GLuint _gTexID;
		/*	Texture target type on the device. GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP etc.
		*/
		GLenum _targetTexType;


	public:
		TexData();
		/*
			Creates a texture object from a loaded image
			gTexID			<<	the GLuint id on the device
			targetTexType	<<	The texture type (GL_TEXTURE_2D (default), GL_TEXTURE_CUBE_MAP etc)
		*/
		TexData(const GLuint gTexID, const GLenum targetTexType = GL_TEXTURE_2D);
		/* Destructor
		*/
		virtual ~TexData();
		/* Move the texture data
		*/
		TexData(TexData &&move);
		/* Move the texture data
		*/
		TexData& operator=(TexData &&move);

		/* Cant copy this data
		*/
		TexData(const TexData &copy) = delete;
		/* Cant copy this data
		*/
		TexData& operator=(const TexData &copy) = delete;

		/*	Binds the texture on the device
		*/
		void bind() const;
		/*	Binds the texture to a sampler
			samplerSlot	<<	Sampler enum index (GL_TEXTURE0 etc)
		*/
		void bind(const GLenum samplerSlot) const;
		/* Destroy the texture
		*/
		void destroy();
		/* Extract the texture id. Nullifying the container. Caller must make sure that the texture is destroyed (on gpu) when finished.
		*/
		virtual GLuint extract();

		/* Fetch the graphics texture type.
		*/
		GLenum texType() const;
		/* Get the texture ID.
		*/
		GLuint texID() const;
	};

}
