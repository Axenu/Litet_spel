#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/* Seed the randomizer with a value */
void seed(unsigned int seed);
/* Get a random unsigned int less then max param 
*/
unsigned int getRand(unsigned int max);
/* Get a random int between the params
*/
int getRand(int min, int max);
/* Get a random float between the params
*/
float getRand(float min, float max);
/* Get a random positive vector with the excluded upper boundary 
*/
glm::ivec2 randIVec2(unsigned int maxX, unsigned int maxY);

/* Get a random vector3 with each component receiving a random value between the limits.
*/
glm::vec3 randVec3(float min, float max);
float lerp(float from, float to, float amount);
/* Linearly interpolate between the two vectors. Amount is the 0-1 influence value between the points.
*/
glm::vec3 lerp(glm::vec3 from, glm::vec3 to, float amount);

template
<
	typename T, //real type
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
int sign(T value) {
	return (value > 0) - (value < 0);
}
int clampInt(int value, int min, int max);