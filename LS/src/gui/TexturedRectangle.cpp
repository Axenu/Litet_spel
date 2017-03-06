#include "gui/TexturedRectangle.h"
#include "gl/Texture.h"

namespace gui
{
    TexturedRectangle::TexturedRectangle(float width, float height, std::string path) : Element()
    {
        _size.x = width;
        _size.y = height;
        _scale.x = width;
        _scale.y = height;
        _shader = Factory::getInstance().getShader("2DTexture");
    	_colorUniform = _shader->getUniform("color");
        _positionZUniform = _shader->getUniform("positionZ");
        _modelMatrixUniform = _shader->getUniform("modelMatrix");
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
    void TexturedRectangle::onRender(float dt)
    {
        _shader->bind();
        glUniform4fv(_colorUniform, 1, &_color[0]);
        glUniform1f(_positionZUniform, _position.z);
        glUniformMatrix3fv(_modelMatrixUniform, 1, false, (GLfloat*)&_modelMatrix[0]);
        glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, _texture);
        _VA.bindVAO();
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, gl::bufferOffset(0));
    }
    void TexturedRectangle::onUpdate(float dt)
    {

    }
    //setters
    void TexturedRectangle::setColor(glm::vec4& color)
    {
        _color = color;
    }
    void TexturedRectangle::setOpacity(float opacity)
    {
        _color.a = opacity;
    }
}
