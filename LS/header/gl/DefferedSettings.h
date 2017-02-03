#pragma once
#include"GLFunctions.h"

namespace gl {

	/* Settings used to initialize the deffered buffer.
	*/
	struct DefferredSettings{
		unsigned int _windowWidth, _windowHeight;
		/*
		*/
		std::vector<DeferredTextureInfo> _textureSetup;

		DefferredSettings(unsigned int windowWidth, unsigned int windowHeight, unsigned int colorBufferCount)
			: _windowWidth(windowWidth), _windowHeight(windowHeight), _textureSetup(colorBufferCount){}
		DefferredSettings(unsigned int windowWidth, unsigned int windowHeight, std::vector<DeferredTextureInfo> &textureSetup)
			: _windowWidth(windowWidth), _windowHeight(windowHeight), _textureSetup(std::move(textureSetup)) {}
	};
}