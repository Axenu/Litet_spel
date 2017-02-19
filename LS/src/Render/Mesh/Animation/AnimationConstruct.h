#pragma once
#include <vector>
#include <memory>
#include "Render/Mesh/Animation/Bone.h"
#include "Render/Mesh/Animation/Animation.h"
#include "Render/Mesh/Animation/ChannelKeys.h"

struct AnimSize {
	size_t _numFloats;
	size_t _numChannels;
	size_t _numNodeChannels;
	size_t _numBones;
};
struct AnimationConstruct {
	size_t _dataInd;
	float* _data;
	std::vector<std::vector<std::vector<ChannelKey>>> _keys;
public:

	AnimationConstruct(AnimSize size);
	~AnimationConstruct();

	std::shared_ptr<Animation> generateAnim(const char* name, float duration);

	void insert(int ch, int nodeCh, float time, const glm::quat &frame);
	void insert(int ch, int nodeCh, float time, const glm::vec3 &frame);
};