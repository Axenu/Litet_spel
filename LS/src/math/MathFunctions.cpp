#include "math/MathFunctions.h"


void seed(unsigned int value)
{;
	srand(value);
}
unsigned int getRand(unsigned int max) {
	return rand() % max;
}

glm::ivec2 randIVec2(unsigned int maxX, unsigned int maxY)
{
	return glm::ivec2(getRand(maxX), getRand(maxY));
}