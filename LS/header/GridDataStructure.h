#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define ROOFHEIGHT 10.0f

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum Enum
{
	nothing = 0,wall=1, tresure=2, loot=3, door=4, guard=5
};

struct gridValues {
	glm::vec2 xz;
	Enum enumet;
	glm::vec3 color;
};

class Grid {

private:
	float heightLength;
	float widthLength;
	gridValues** twodArray;

public:
	Grid();
	void buildgridarray();
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(char* filename);
	vector<glm::vec3> generateMesh();
};




#endif