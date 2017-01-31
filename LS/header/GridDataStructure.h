#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define GRIDSPACE 1.f
#define ROOFHEIGHT 1.f
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
	float _gridSpace;
	float _roofHeight;
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray;
	bool _gotTheTreasure;
	glm::vec2 exit;

public:
	Grid();
	void buildgridarray();
	gridValues getData(gridType Data);
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(char* filename);
	Mesh generateMesh();
	bool wallCollission(glm::vec3 position);
	void checkifPlayerWon(glm::vec3 playerpos);
};




#endif
