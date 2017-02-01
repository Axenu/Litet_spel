#include "gui/Label.h"

namespace gui
{
    Label::Label(Font* font, std::string text) : Element()
    {
        _font = font;
        _shader = new Shader("2DTexture");
    	_colorUniform = _shader->getUniform("color");
        _positionUniform = _shader->getUniform("position");
        _sizeUniform = _shader->getUniform("size");
        _color = glm::vec4(1,1,1,1);

        setText(text);

    }
    Label::~Label()
    {

    }
    void Label::render()
    {
        _shader->bind();
        glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, _font->getFontTexture());
        glUniform4fv(_colorUniform, 1, &_color[0]);
        glUniform3fv(_positionUniform, 1, &_position[0]);
        glUniform2fv(_sizeUniform, 1, &_size[0]);
        _VA.bindVAO();
    	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, gl::bufferOffset(0));
    }
    void Label::update(float dt)
    {

    }

    void Label::setText(std::string &text)
    {
        _text = text;
        std::vector<GLfloat> vertices;
    	std::vector<GLuint> indices;

    	float x = 0.0f;
    	glyph* g = _font->getGlyph((int)text[0]);

    	//custom anchorpoint?
    	for (int i = 0; i < (int)text.length(); i++)
        {
    		//v0 |_
    		vertices.push_back(x + g->x1); //x1
    		vertices.push_back(g->y1); //y1

    		vertices.push_back(g->u1); //u1
    		vertices.push_back(g->v1); //v1

    		//v1 |-
    		vertices.push_back(x + g->x1); //x1
    		vertices.push_back(g->y2); //y2

    		vertices.push_back(g->u1); //u
    		vertices.push_back(g->v2); //v2
    		//v2-|
    		vertices.push_back(x + g->x2); //x2
    		vertices.push_back(g->y2); //y2

    		vertices.push_back(g->u2); //u
    		vertices.push_back(g->v2); //v

    		//v3 _|
    		vertices.push_back(x + g->x2); //x2
    		vertices.push_back(g->y1); //y1

    		vertices.push_back(g->u2); //u2
    		vertices.push_back(g->v1); //v

    		//indices
    		indices.push_back(i*4+0);
    		indices.push_back(i*4+2);
    		indices.push_back(i*4+1);
    		indices.push_back(i*4+0);
    		indices.push_back(i*4+3);
    		indices.push_back(i*4+2);

    		x += g->xAdvance;
    		g = _font->getGlyph((int)text[i+1]);
    	}
        _vertexCount = vertices.size();
        _indexCount = indices.size();

        std::vector<gl::VertexAttribute> attri;
    	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 2, sizeof(float))); //Pos attribute
    	attri.push_back(gl::VertexAttribute(1, GL_FLOAT, 2, sizeof(float))); //UV attribute

    	std::vector<const void*> vertexData;
    	vertexData.push_back(&vertices[0]);   //Get position array start pointer
    	vertexData.push_back(&vertices[2]);		//Get UV array start pointer

    	_VA = gl::generateVAO_AoS(&vertices[0], attri, _vertexCount, &indices[0], sizeof(indices[0]), _indexCount); // Create VAO
    	gl::CheckGLErrors("generate label VA");
    }










}
