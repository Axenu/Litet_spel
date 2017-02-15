#include "Render/Mesh/Animation/KeyFrame.h"



KeyFrame::KeyFrame(float time, glm::vec3 pos, glm::vec3 scale, glm::quat rotation)
	: _time(time), _transform(), _custom(false)
{
}
KeyFrame::KeyFrame(float time, const glm::mat4 &transform, bool custom)
	: _time(time), _transform(transform), _custom(custom) 
{
}

glm::mat4 KeyFrame::lerpTo(const KeyFrame &to, float eT) {
	float lerpAmount = (_time - eT) / (to._time - eT);
	return glm::mat4();
}

KeyFrame::~KeyFrame()
{
}
