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
#include "Game/Level/GridSquare.h"
#include "Render/Mesh/Mesh.h"
#include "Event/Events.h"

struct gridValues {
	glm::vec2 xz;
	gridType type;
	int value;
};

struct gridNode {
	bool needsCheck = true;
	glm::ivec2 position;
};

class Grid {

private:
	float _gridSpace;
	float _roofHeight;
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray; //f�rsta �r heightlength andra �r widthlenght
	glm::vec3 pointxy[4];
	std::vector<glm::vec3> _lootLocations;
	std::vector<glm::vec3> _guardLocations;
	int IsInVector(glm::ivec2 pos, std::vector<gridNode> *vector);
	bool removeGridCell(glm::ivec2 pos, std::vector<gridNode> *vector);
public:
	Grid(const std::string& level);
	void buildgridarray();
	glm::vec3 getData(gridType Data);
	~Grid();
	void print2darraydata();
	void loadingBmpPicture(const char* filename);
	Mesh generateMesh();
	void wallCollission(glm::vec3 *position, glm::vec3 velocity);
	void Creategetheightandwidthpoint12(glm::vec3 guardposition);
	glm::vec3 getheightandwidthpoint12(int i);
	int getHeight();
	int getWidth();
	std::vector<glm::vec3> * getLootLocations();
	std::vector<glm::vec3> *  getGuardLocations();
	glm::vec3 getLastValueOfGuardLocationsAndremovesit();
	void buildGridArrayForGuards();
	bool isAccessible(glm::ivec2 start, glm::ivec2 end);
	/* Get the size of a grid square */
	float getGridSpace();
	int getvalue(int height,int width);
	void setvalue(int height, int width, int value);
	gridType gettype(int height, int width);
	bool isInside(glm::ivec2 vec) const;
	gridType returnGridType(int width,int height);
	glm::ivec2 getSquare(const glm::vec3 &pos) const;
	GridSquare operator[](glm::vec3 vec) const;
	gridType operator[](const glm::ivec2 &sq) const;
	/* Get the center position of the specified square */
	glm::vec3 getCenter(glm::ivec2 vec) const;
};

#endif
