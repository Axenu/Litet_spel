#pragma once
#include <vector>
#include <string>
#include "KeyFrame.h"

class Animation
{
private:
public:
	const std::string _name;
	const float _duration;
	const std::vector<std::vector<KeyFrame>> _keys;

	Animation(const std::string &name, float duration, std::vector<std::vector<KeyFrame>> &keys);
	~Animation();
	/* Finds the next KeyFrame index for a bone and the specific time.
	*/
	int goToFrame(int bone, float elapsedTime);
};

