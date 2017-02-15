#include "Render/Mesh/Animation/Animation.h"



Animation::Animation(const std::string &name, float duration, std::vector<std::vector<KeyFrame>> &keys)
	: _name(name), _duration(duration), _keys(std::move(keys))
{
}


Animation::~Animation()
{
}

int Animation::goToFrame(int bone, float elapsedTime) {
	for (unsigned int i = 0; i < _keys[bone].size(); i++) {
		if (elapsedTime < _keys[bone][i]._time)
			return i;
	}
	return -1;
}