#include "AnimationConstruct.h"

AnimationConstruct::AnimationConstruct(AnimSize size)
	: _dataInd(0), _data(new float[size._numFloats]), _keys(size._numBones)
{	
	for (unsigned int i = 0; i < size._numBones; i++) 
	{
		_keys[i] = std::vector<std::vector<ChannelKey>>(size._numNodeChannels);
	}
}

AnimationConstruct::~AnimationConstruct() 
{
	delete[] _data;
}


Animation* AnimationConstruct::generateAnim(const char* name, float duration) {
	std::unique_ptr<float> ptr(_data);
	_data = nullptr;
	std::vector<Channel> channels(_keys.size());
	for (unsigned int i = 0; i < _keys.size(); i++)
		channels[i] = Channel(_keys[i]);
	return new Animation(name, duration, channels, ptr);
}

void AnimationConstruct::insert(int ch, int nodeCh, float time, const glm::quat &value) {
	_keys[ch][nodeCh].push_back(ChannelKey(time, _data + _dataInd));
	_data[_dataInd] = value.x;
	_data[_dataInd + 1] = value.y;
	_data[_dataInd + 2] = value.z;
	_data[_dataInd + 3] = value.w;
	_dataInd += 4;
}
void AnimationConstruct::insert(int ch, int nodeCh, float time, const glm::vec3 &value) {
	_keys[ch][nodeCh].push_back(ChannelKey(time, _data + _dataInd));
	_data[_dataInd] = value.x;
	_data[_dataInd + 1] = value.y;
	_data[_dataInd + 2] = value.z;
	_dataInd += 3;
}