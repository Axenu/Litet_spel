#pragma once
#include"gl/DefferedSettings.h"


class Setting {
public:
	/* Deferred settings
	*/
	gl::DefferredSettings _renderSetting;
	/* Camera settings
	*/
	float _nearPlane, _farPlane, _fov;

	Setting();
	Setting(unsigned int winWidth, unsigned int winHeight, unsigned int colBuffers, float near, float far, float fov);

	unsigned int Width() const { return _renderSetting._windowWidth; }
	unsigned int Height() const { return _renderSetting._windowHeight; }
};