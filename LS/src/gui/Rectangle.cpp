#include "gui/Rectangle.h"

namespace gui
{
    Rectangle::Rectangle(float width, float height) 
		: Element(), _shader("2DSimple")
    {
        _size.x = width;
        _size.y = height;
    	_colorUniform = _shader.getUniform("color");
        _positionUniform = _shader.getUniform("position");
        _sizeUniform = _shader.getUniform("size");

        _vertexCount = 4;
        _indexCount = 6;

        static const GLfloat vertices[] = {
            1, 1,
            0, 1,
            1, 0,
            0, 0
        };

        static const GLuint indices[] = {
            0,1,2,
            1,3,2
        };

        std::vector<gl::VertexAttribute> attri;
    	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 2, sizeof(float))); //Pos attribute

    	std::vector<const void*> vertexData;
    	vertexData.push_back(&vertices[0]);   //Get position array start pointer

    	_VA = gl::generateVAO_AoS(&vertices[0], attri, _vertexCount, &indices[0], sizeof(indices[0]), _indexCount); // Create VAO
    	gl::CheckGLErrors("generate label VA");

    }
    Rectangle::~Rectangle()
    {

    }
    void Rectangle::render()
    {
        _shader.bind();
        glUniform4fv(_colorUniform, 1, &_color[0]);
        glUniform3fv(_positionUniform, 1, &_position[0]);
        glUniform2fv(_sizeUniform, 1, &_size[0]);
        _VA.bindVAO();
    	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, gl::bufferOffset(0));
    }
    void Rectangle::update(float dt)
    {

    }

    //getters
    glm::vec4& Rectangle::getColor()
    {
        return _color;
    }
    //setters
    void Rectangle::setColor(glm::vec4& color)
    {
        _color = color;
    }
}
