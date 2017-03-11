#include "math/MathFunctions.h"
#include <algorithm>


void seed(unsigned int value)
{;
	srand(value);
}
unsigned int getRand(unsigned int max) {
	return rand() % max;
}
/* Get a random int between the params
*/
int getRand(int min, int max)
{
	return rand() % (max - min) + min;
}
/* Get a random float between the params
*/
float getRand(float min, float max)
{
	//Second rand for a random decimal part.
	return std::fmod((float)rand() + (float)rand() / RAND_MAX, max - min) + min;
}

glm::ivec2 randIVec2(unsigned int maxX, unsigned int maxY)
{
	return glm::ivec2(getRand(maxX), getRand(maxY));
}

/* Get a random vector3 with each component receiving a random value between the limits.
*/
glm::vec3 randVec3(float min, float max) 
{
	return glm::vec3(getRand(min, max), getRand(min, max), getRand(min, max));
}

float lerp(float from, float to, float amount)
{
	return from + (to - from) * amount;
}

glm::vec3 lerp(glm::vec3 from, glm::vec3 to, float amount)
{
	return from + (to - from) * amount;
}
int clampInt(int value, int min, int max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}
