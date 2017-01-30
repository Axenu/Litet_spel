#pragma once
#ifndef VICTORYCONDITION_H
#define VICTORYCONDITION_H
#include "gl/glInclude.h"
#include <glm/glm.hpp>
#include <vector>
#include "GridDataStructure.h"


class Victory {
private:
	bool _gotTheTreasure;
	bool _atTheExit;
	glm::vec2 exit;


public:
	Victory();
	~Victory();
	void checkifPlayerWon(glm::vec3 playerpos );



};



#endif // !VICTORYCONDITION_H
