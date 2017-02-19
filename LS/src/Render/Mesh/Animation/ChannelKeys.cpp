#include "Render/Mesh/Animation/ChannelKeys.h"

#pragma region Channel Key

ChannelKey::ChannelKey() {}
ChannelKey::ChannelKey(float time, float* value)
	: _time(time), _value(value)
{
}
ChannelKey::~ChannelKey() {}
glm::vec3 ChannelKey::asVec3() const
{
	return glm::make_vec3(_value);
}
glm::quat ChannelKey::asQuat() const
{
	return glm::make_quat(_value);
}
void ChannelKey::set(glm::vec3 value) {
	_value[0] = value.x;
	_value[1] = value.y;
	_value[2] = value.z;
}
void ChannelKey::set(glm::quat value) {
	_value[0] = value.x;
	_value[1] = value.y;
	_value[2] = value.z;
	_value[3] = value.w;
}
/* Set the values of the key.
*/
void ChannelKey::set(float time, glm::vec3 value) {
	_time = time;
	_value[0] = value.x;
	_value[1] = value.y;
	_value[2] = value.z;
}
/* Set the values of the key.
*/
void ChannelKey::set(float time, glm::quat value) {
	_time = time;
	_value[0] = value.x;
	_value[1] = value.y;
	_value[2] = value.z;
	_value[3] = value.w;
}

#pragma endregion

#pragma region Memory Key

ChannelMemoryKey::ChannelMemoryKey() {	}
ChannelMemoryKey::ChannelMemoryKey(unsigned int size)
	: ChannelKey(0.f, new float[size]), _size(size)
{

}
ChannelMemoryKey::ChannelMemoryKey(float time, glm::vec3 data)
	: ChannelKey(time, new float[3]), _size(3)
{
	ChannelKey::set(time, data);
}

ChannelMemoryKey::~ChannelMemoryKey()
{
	delete[] _value;
}
void ChannelMemoryKey::operator = (const ChannelKey &key) {
	set(key, _size);
}
void ChannelMemoryKey::operator= (const ChannelMemoryKey &key) {
	if (this == &key)
		return;
	_size = key._size;
	float* tmp = _value;
	_value = new float[_size];
	set(key, _size);
	delete[] tmp;
}
/* Overwrite a number of values from the specific key.
*/
void ChannelMemoryKey::set(const ChannelKey& key, unsigned int copyCount) {
	_time = key._time;
	//Copy value
	memcpy(_value, key._value, sizeof(float) * (copyCount = copyCount < _size ? copyCount : _size));
}

#pragma endregion