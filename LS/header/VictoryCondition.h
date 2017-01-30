#pragma once
#ifndef VICTORYCONDITION_H
#define VICTORYCONDITION_H
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include "GridDataStructure.h"


class Victory {
private:
	bool _gotTheTreasure;
	glm::vec2 exit;


public:
	Victory();
	~Victory();
	void checkifPlayerWon(glm::vec2 playerpos );



};



#endif // !VICTORYCONDITION_H
