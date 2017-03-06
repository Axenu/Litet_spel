#pragma once
#include <glm/vec2.hpp>

/* Seed the randomizer with a value */
void seed(unsigned int seed);
/* Get a random unsigned int less then max param 
*/
unsigned int getRand(unsigned int max);
/* Get a random positive vector with the excluded upper boundary 
*/
glm::ivec2 randIVec2(unsigned int maxX, unsigned int maxY);



template
<
	typename T, //real type
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
int sign(T value) {
	return (value > 0) - (value < 0);
}
int clampInt(int value, int min, int max);