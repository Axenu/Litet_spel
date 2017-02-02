#include "gui/TexturedRectangle.h"
#include "gl/Texture.h"

namespace gui
{
    TexturedRectangle::TexturedRectangle(float width, float height, std::string path) : Element()
    {
        _size.x = width;
        _size.y = height;
        _shader = new Shader("2DTexture");
    	_colorUniform = _shader->getUniform("color");
        _positionUniform = _shader->getUniform("position");
        _sizeUniform = _shader->getUniform("size");
        _textureUniform = _shader->getUniform("_texture");
        _texture = Texture::createTexture(path);
        _color = glm::vec4(1,1,1,1);

        _vertexCount = 4;
        _indexCount = 6;

        static const GLfloat vertices[] = {
            1, 1, 1, 1,
            0, 1, 0, 1,
            1, 0, 1, 0,
            0, 0, 0, 0
        };

        static const GLuint indices[] = {
            0,1,2,
            1,3,2
        };

        std::vector<gl::VertexAttribute> attri;
    	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 2, sizeof(float))); //Pos attribute
    	attri.push_back(gl::VertexAttribute(1, GL_FLOAT, 2, sizeof(float))); //UV attribute

    	std::vector<const void*> vertexData;
    	vertexData.push_back(&vertices[0]);    //Get position array start pointer
    	vertexData.push_back(&vertices[2]);	   //Get UV array start pointer
        _VA = gl::generateVAO_AoS(&vertices[0], attri, _vertexCount * 4, &indices[0], sizeof(indices[0]), _indexCount); // Create VAO
    	gl::CheckGLErrors("generate label VA");

    }
    TexturedRectangle::~TexturedRectangle()
    {

    }
    void TexturedRectangle::render()
    {
        _shader->bind();
        glUniform4fv(_colorUniform, 1, &_color[0]);
        glUniform3fv(_positionUniform, 1, &_position[0]);
        glUniform2fv(_sizeUniform, 1, &_size[0]);
        glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, _texture);
        _VA.bindVAO();
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, gl::bufferOffset(0));
    }
    void TexturedRectangle::update(float dt)
    {

    }
}
