#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <IL/il.h>
#include "gl/glInclude.h"
#include "gl/Texture.h"

namespace gui
{
	struct glyph
	{
		float xAdvance;
		float x1, x2, y1, y2;
		float u1, u2, v1, v2;
	};

	class Font
	{

	private:
	    float _fontSize;
		float _fontHeight;
		float _baseline;
		float _textureWidth;
		float _textureHeight;

	    std::map<int, glyph> _glyphs;
		GLuint _fontTexture;

	    void loadCharacters();

	public:


	    Font();
		Font(std::string path);
	    ~Font();

		float getFontSize();
		float getFontHeight();
		GLuint getFontTexture();
		glyph* getGlyph(int g);
	};
}
