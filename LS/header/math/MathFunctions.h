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



