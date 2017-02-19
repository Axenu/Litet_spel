#include "Render/Mesh/Animation/KeyFrame.h"


/* Lerp the keys as vec3 with the specific amount */
glm::vec3 lerpVec3(const ChannelKey& from, const ChannelKey& to, float amount) {
	return from.asVec3() * (1.f - amount) + to.asVec3() * amount;
}
/* Slerp the keys as quaternions with the specific amount */
glm::quat slerp(const ChannelKey& from, const ChannelKey& to, float amount) {
	return glm::slerp(from.asQuat(), to.asQuat(), amount);
}