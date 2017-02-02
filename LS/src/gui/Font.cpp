#include "gui/Font.h"
#include "gl/GLFunctions.h"

namespace gui {
	Font::Font()
	{

	}

	Font::Font(std::string path)
	{

		std::ifstream fontFile;
		fontFile.open(path + ".fnt");
		if (!fontFile.is_open()) {
			std::cout << "Font file: " + path + " could not be found";
			return;
		}
		std::string Line;
		std::string Read, Key, Value;
		std::size_t i;
		std::string textureFileName = "";

		float x = 0, y = 0;
		float width = 0, height = 0;
		float xOffset = 0, yOffset = 0;

		while(!fontFile.eof())
		{
			std::stringstream lineStream;
			std::getline( fontFile, Line );
			lineStream << Line;

			//read the line's type
			lineStream >> Read;
			if( Read == "common" ) {
				while(!lineStream.eof())
				{
					std::stringstream Converter;
					lineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "scaleW" )
					{
						Converter >> _textureWidth;
					}
					else if( Key == "scaleH" )
					{
						Converter >> _textureHeight;
						_fontHeight /= _textureHeight;
						_fontSize /= _textureHeight;
						_baseline /= _textureHeight;
					}
					else if( Key == "base" )
					{
						Converter >> _baseline;
					}
					else if( Key == "file" )
					{
						Converter >> textureFileName;
					}
					else if( Key == "lineHeight" )
					{
						Converter >> _fontHeight;
					}
				}
			} else if( Read == "info" ) {
				while( !lineStream.eof() )
				{
					std::stringstream Converter;
					lineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "size" )
					{
						Converter >> _fontSize;
					}
				}
			} else if (Read == "char") {
				int charID = 0;
				glyph g;
				while( !lineStream.eof() )
				{
					std::stringstream Converter;
					lineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//Assign the correct value
					Converter << Value;
					if( Key == "id" )
					{
						Converter >> charID;
					}
					else if( Key == "x" )
					{
						Converter >> x;
					}
					else if( Key == "y" )
					{
						Converter >> y;
					}
					else if( Key == "width" )
					{
						Converter >> width;
					}
					else if( Key == "height" )
					{
						Converter >> height;
					}
					else if( Key == "xoffset" )
					{
						Converter >> xOffset;
					}
					else if( Key == "yoffset" )
					{
						Converter >> yOffset;
					}
					else if( Key == "xadvance" )
					{
						Converter >> g.xAdvance;
					}
				}
				x /= _textureWidth;
				y /= _textureHeight;
				y = 1 - y;
				xOffset /= _textureWidth;
				yOffset /= _textureHeight;
				g.xAdvance /= (_textureWidth);
				width /= _textureWidth;
				height /= _textureHeight;
				yOffset = _baseline - height - yOffset;
				g.x1 = xOffset;
				g.x2 = xOffset + width;
				g.y1 = yOffset;
				g.y2 = yOffset + height;
				g.u1 = x;
				g.u2 = x + width;
				g.v1 = y - height;
				g.v2 = y;
				_glyphs.insert(std::pair<int,glyph>(charID,g));
			}
		}
		_fontTexture = Texture::createTexture(path + ".png");
	}

	float Font::getFontSize()
	{
	    return _fontSize;
	}
	float Font::getFontHeight()
	{
		return _baseline;
	}
	GLuint Font::getFontTexture()
	{
		return _fontTexture;
	}

	glyph* Font::getGlyph(int g)
	{
		return &_glyphs[g];
	}

	Font::~Font()
	{

	}
}
