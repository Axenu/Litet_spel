#pragma once

namespace gl {

	/* Settings used to initialize the deffered buffer.
	*/
	struct DefferredSettings{
		unsigned int _windowWidth, _windowHeight, _colorBufferCount;

		DefferredSettings(unsigned int windowWidth, unsigned int windowHeight, unsigned int colorBufferCount)
			: _windowWidth(windowWidth), _windowHeight(windowHeight), _colorBufferCount(colorBufferCount){}
	};
}