#pragma once
#pragma once
#include "gl/glInclude.h"
#include<vector>

namespace gl {

	/* Vertex array gl buffered data. Contains the gl buffer links created for the data.
	* Combination of VBO, IBO and VAO objects. May contain links to any combination of named buffers/object.
	*/
	class VAData {
	private:
		/* Vertex Array Object
		*/
		GLuint _VAO;
		/* GL Buffers owned in this
		*/
		std::vector<GLuint> _glBuffer;
		/* Deletes buffers on graphics card if necesary
		*/
		void destroy();
	public:
		VAData();
		VAData(GLuint VAO);
		VAData(std::vector<GLuint> &buffers);
		VAData(GLuint VAO, std::vector<GLuint> &buffers);
		/* Destructor
		*/
		virtual ~VAData();
		/* Move the data
		*/
		VAData(VAData &&move);
		/* Move the data
		*/
		VAData& operator=(VAData &&move);

		/* Cant copy this data
		*/
		VAData(const VAData &copy) = delete;
		/* Cant copy this data
		*/
		VAData& operator=(const VAData &copy) = delete;

		/*	Binds the VAO object
		*/
		void bindVAO() const;
		/* Unbinds any VAO and set to default: 0
		*/
		void clearBinding() const;
	};

}
