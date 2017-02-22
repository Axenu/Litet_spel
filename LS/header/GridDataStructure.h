#ifndef GRIDDATASTRUCTURE
#define GRIDDATASTRUCTURE

#define GRIDSPACE 1.f
#define ROOFHEIGHT 3.f
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include "Game/Level/GridSquare.h"
#include "Render/Mesh/Mesh.h"
#include "Event/Events.h"
#include "Game/Objects/GameObject.h"
#include "intersectionFunctions.h"

#define WalkHeight 0.25f

struct gridValues {
	glm::vec2 xz;
	gridType type;
	float height;
	int value;
};

struct gridNode {
	bool needsCheck = true;
	glm::ivec2 position;
};

struct lightValues {
	glm::vec3 pos;
	glm::vec3 diffuse;
	float dist;
};

class Grid {

private:
	float _gridSpace;
	float _roofHeight;
	int _heightLength;
	int _widthLength;
	gridValues** _twodArray;
	glm::vec3 pointxy[4];
	std::vector<glm::vec3> _lootLocations;
	std::vector<glm::vec3> _guardLocations;
	std::vector<lightValues> _light;

	void buildgridarray();
	void print2darraydata();
	void loadingBmpPicture(const char* filename);
public:
	Grid(const std::string& level);
	~Grid();
	glm::vec3 getData(gridType Data);
	Mesh generateMesh();
	void wallCollission(glm::vec3 *position, glm::vec3 velocity);
	void Creategetheightandwidthpoint12(glm::vec3 guardposition);
	float calcLightOnPosition(glm::vec3 playerPos);
	glm::vec3 getheightandwidthpoint12(int i);
	int getHeight();
	int getWidth();
	std::vector<glm::vec3> * getLootLocations();
	std::vector<glm::vec3> *  getGuardLocations();
	glm::vec3 getLastValueOfGuardLocationsAndremovesit();
	/* Get the size of a grid square */
	float getGridSpace();
	int getvalue(int height,int width);
	void setvalue(int height, int width, int value);
	gridType gettype(int height, int width);
	bool isInside(glm::ivec2 vec) const;
	gridType returnGridType(int width,int height);
	glm::ivec2 getSquare(const glm::vec3 &pos) const;
	float getGridHeight(const glm::vec3 &pos) const;
	bool testForClimb(glm::vec3 &pos, glm::vec3 &dir, float &heightDiff);
	GridSquare operator[](glm::vec3 vec) const;
	gridType operator[](const glm::ivec2 &sq) const;
	/* Get the center position of the specified square */
	glm::vec3 getCenter(glm::ivec2 vec) const;
	void getRightQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getLeftQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getFrontQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getBackQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	/* Adding object to grid*/
	void addObject(GameObject* object, gridType gridType);
	std::vector<glm::ivec2> generatePath(glm::ivec2 startPosition, glm::ivec2 goalPosition);
	float getWallDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist);
	float getObjectDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist, glm::vec3 playerPos, glm::vec3 playerEyePos);
	void addLight(glm::vec3 lightPos, glm::vec3 diff, float dist);
};

#endif
