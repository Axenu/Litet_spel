#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define GRIDSPACE 1.f
#define ROOFHEIGHT 10.f
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include "Mesh.h"

enum gridType
{
	// color values (0, 0, 0) = nothing, (255, 255, 255) = wall, (0, 255, 0) = loot, (0, 0, 255) = door (255, 0,0) = exiting
	nothing = 0, wall = 1, tresure = 2, loot = 3, door = 4, exiting = 5, guard = 6
};

struct gridValues {
	glm::vec2 xz;
	gridType type;
};

class Grid {

private:
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray;

public:
	Grid();
	void buildgridarray();
	gridValues getData(gridType Data);
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(char* filename);
	void generateMesh(std::vector<glm::vec3> *position, std::vector<glm::vec3> *normal);
	bool wallCollission(glm::vec3 position);
};




#endif
