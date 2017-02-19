#include "Render/Mesh/Animation/Animation.h"



Animation::Animation(const std::string &name, float duration, std::vector<Channel> &channels, std::unique_ptr<float>& data)
	:  _channels(std::move(channels)), _data(std::move(data)), _name(name), _duration(duration)
{
}


Animation::~Animation()
{
}


const Channel& Animation::operator[](unsigned int bone) {
	return _channels[bone];
}