#include "gui/Label.h"

namespace gui
{
    Label::Label(Font* font, std::string text)
		: Element(), _shader("2DTexture")
    {
        _font = font;
        _textSize.y = font->getFontHeight();
    	_colorUniform = _shader.getUniform("color");
        _positionUniform = _shader.getUniform("position");
        _sizeUniform = _shader.getUniform("size");
        _modelMatrixUniform = _shader.getUniform("modelMatrix");
        _color = glm::vec4(1,1,1,1);

        setText(text);

    }
    Label::~Label()
    {
        // delete _font;
    }
    void Label::onRender()
    {
        _shader.bind();
        glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, _font->getFontTexture());
        glUniform4fv(_colorUniform, 1, &_color[0]);
        glUniform3fv(_positionUniform, 1, &_positionGlobal[0]);
        glUniform2fv(_sizeUniform, 1, &_size[0]);
        glUniformMatrix3fv(_modelMatrixUniform, 1, false, (GLfloat*)&_modelMatrix[0]);
        _VA.bindVAO();
    	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, gl::bufferOffset(0));
        _VA.clearBinding();
    }
    void Label::onUpdate(float dt)
    {
        std::string tempText = "";
        for (StringComponent *sc : _sComponents)
        {
            tempText += sc->getString();
        }
        if (_text != tempText)
        {
            _text = tempText;
            updateText();
        }
    }

    void Label::addStringComponent(StringComponent* sc)
    {
        _sComponents.push_back(sc);
        updateText();
    }
    void Label::updateText()
    {
        // _text = text;
        std::vector<GLfloat> vertices;
    	std::vector<GLuint> indices;

    	float x = 0.0f;
    	glyph* g;

    	//custom anchorpoint?
    	for (int i = 0; i < (int)_text.length(); i++)
        {
            g = _font->getGlyph((int)_text[i]);
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
    	}
        _textSize.x = x;
        _vertexCount = vertices.size() / 4;
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
    void Label::setText(std::string &text)
    {
        // std::string tempText = "";
        // for (StringComponent *sc : _sComponents)
        // {
        //     tempText += sc->getString();
        // }
        // std::cout << tempText << std::endl;
        _text = text;
        std::vector<GLfloat> vertices;
    	std::vector<GLuint> indices;

    	float x = 0.0f;
    	glyph* g;

    	//custom anchorpoint?
    	for (int i = 0; i < (int)text.length(); i++)
        {
            g = _font->getGlyph((int)text[i]);
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
    	}
        _textSize.x = x;
        _vertexCount = vertices.size() / 4;
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

    //getters
    float Label::getTextWidth()
    {
        return _textSize.x;
    }
    float Label::getTextHeight()
    {
        return _textSize.y;
    }
    glm::vec2& Label::getTextSize()
    {
        return _textSize;
    }










}
