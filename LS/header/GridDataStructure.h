#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;
enum Enum
{
	nothing = 0,wall=1, tresure=2, loot=3, door=4, guard=5
};
struct gridValues {
	glm::vec3 xyz;
	Enum enumet;
};

class Grid{
private:
float heightLength;
float widthLength;
gridValues** twodArray;
public:
Grid();
void buildgridarray();
~Grid();
void print2darraydata();
};




#endif