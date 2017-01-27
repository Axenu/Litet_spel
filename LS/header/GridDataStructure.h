#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define ROOFHEIGHT 10.0f
#include "gl/glInclude.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>

enum Enum
{
	nothing = 0,wall = 1, tresure = 2, loot = 3, door = 4, guard = 5
};

struct gridValues {
	glm::vec2 xz;
	Enum enumet;
	glm::vec3 color;
};

class Grid {

private:
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray;

public:
	Grid();
	void buildgridarray();
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(char* filename);
	std::vector<glm::vec3> generateMesh();
};




#endif
