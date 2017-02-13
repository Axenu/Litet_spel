#include"Game/Setting.h"



Setting::Setting(unsigned int winWidth, unsigned int winHeight, unsigned int colBuffers, float near, float far, float fov)
: _renderSetting(winWidth, winHeight, colBuffers), _nearPlane(near), _farPlane(far), _fov(fov) {
}
Setting::Setting()
: _renderSetting(0, 0, 0){
}