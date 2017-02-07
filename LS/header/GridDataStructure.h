#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define GRIDSPACE 1.f
#define ROOFHEIGHT 2.f
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include "Mesh.h"
#include "Event/Events.h"
enum gridType
{
	// color values (0, 0, 0) = nothing, (255, 255, 255) = wall, (0, 255, 0) = loot, (0, 0, 255) = door (255, 0,0) = exiting
	nothing = 0, wall = 1, tresure = 2, loot = 3, door = 4, exiting = 5, guard = 6
};

struct gridValues {
	glm::vec2 xz;
	gridType type;
	int value;
};

class Grid {

private:
	float _gridSpace;
	float _roofHeight;
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray; //f�rsta �r heightlength andra �r widthlenght
	bool _gotTheTreasure;
	glm::vec2 _exit;
	glm::vec3 pointxy[4];
	std::vector<glm::vec3> _guardPositions;
	std::vector<glm::vec3> _exitposition;
	std::vector<glm::vec3> _lootLocations;
	bool outsidethebox;
	bool doyouwanttoleave = false, yousure = false;
public:
	Grid();
	void buildgridarray();
	glm::vec3 getData(gridType Data);
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(char* filename);
	Mesh generateMesh();
	void wallCollission(glm::vec3 *position, glm::vec3 velocity);
	bool checkifPlayerWon(glm::vec3 * playerpos, bool buttonpressed);
	void Creategetheightandwidthpoint12(glm::vec3 guardposition);
	gridType returnGridType(int width,int height);
	glm::vec3 getheightandwidthpoint12(int i);
	int getHeight();
	int getWidth();
	std::vector<glm::vec3> * getLootLocations();
	PossibleVictoryEvent victory;

};




#endif
