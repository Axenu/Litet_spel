#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class KeyFrame
{
public:
	/* Determines if the frame is custom generated animation key. False if it belongs to an loaded Animation */
	bool _custom;
	glm::mat4x4 _transform;
	float _time;

	KeyFrame(float time, const glm::mat4 &transform, bool custom = false);
	KeyFrame(float time, glm::vec3 pos, glm::vec3 scale, glm::quat rotation);
	~KeyFrame();


	/* Lerps from the keyframe to the next one
	*/
	glm::mat4 lerpTo(const KeyFrame &to, float eT);
};

