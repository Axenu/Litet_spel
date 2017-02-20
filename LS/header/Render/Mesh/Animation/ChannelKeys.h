#pragma once
/* Mattias F 2017*/
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>


/* Key in a animated channel holding reference to a set of floats and the relative time of influence.
*/
class ChannelKey {
public:

	/* Animation time the key value influences it's channel */
	float _time;
	/* Referenced key value (if not an memory key) */
	float* _value;

	ChannelKey();
	ChannelKey(float time, float* value);
	~ChannelKey();
	glm::vec3 asVec3() const;
	glm::quat asQuat() const;

	/* Set the value of the key */
	void set(glm::vec3 value);
	/* Set the value of the key */
	void set(glm::quat value);
	/* Set the values of the key. */
	void set(float time, glm::vec3 value);
	/* Set the values of the key. */
	void set(float time, glm::quat value);
};
/* Channel key owning it's memory pointer. Note that keys need to match when use and size is determined by the larger structure.
*/
class ChannelMemoryKey
	: public ChannelKey {
public:

	/* Number of values in the key
	*/
	unsigned int _size;

	ChannelMemoryKey();
	ChannelMemoryKey(unsigned int size);
	ChannelMemoryKey(float time, glm::vec3 data);
	~ChannelMemoryKey();

	ChannelMemoryKey& operator= (const ChannelKey &key);
	ChannelMemoryKey& operator= (const ChannelMemoryKey &key);

	/* Overwrite a number of values from the specific key.
	*/
	void set(const ChannelKey& key, unsigned int copyCount);
	using ChannelKey::set; //Make parent set functions visible
};