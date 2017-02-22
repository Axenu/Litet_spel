#pragma once

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
#include "Game/Objects/GameObject.h"
#include "math/intersectionFunctions.h"
#include "math/AARect.h"
#include "Game/Level/Path.h"
#include <memory>

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

	void buildgridarray();
	void print2darraydata();
	void loadingBmpPicture(const char* filename);

	bool isWalkable(glm::ivec2 square);
public:
	Grid(const std::string& level);
	~Grid();
	glm::vec3 getData(gridType Data);
	Mesh generateMesh();
	glm::vec3 wallCollission(glm::vec3 position, glm::vec3 velocity);
	void Creategetheightandwidthpoint12(glm::vec3 guardposition);
	glm::vec3 getheightandwidthpoint12(int i);
	int getHeight();
	int getWidth();
	float getHeight(int height, int width);
	std::vector<glm::vec3> * getLootLocations();
	std::vector<glm::vec3> *  getGuardLocations();
	glm::vec3 getLastValueOfGuardLocationsAndremovesit();
	/* Get the size of a grid square */
	float getGridSpace();
	int getvalue(int height,int width);
	void setvalue(int height, int width, int value);
	gridType gettype(int height, int width);

#pragma region Mfuncs
	/* Verify a grid square is represented in the grid
	*/
	bool isInside(glm::ivec2 vec) const;
	gridType returnGridType(int width,int height);
	/* Get the grid square associated with the position value
	*/
	glm::ivec2 getSquare(const glm::vec3 &pos) const;
	/* Get the rectangle representing a square */
	AARect getSquareRect(glm::ivec2 square) const;
	/* Get a random square in the grid
	*/
	glm::ivec2 getRandomSquare();
	GridSquare operator[](glm::vec3 vec) const;
	gridType operator[](const glm::ivec2 &sq) const;
#pragma endregion
	/* Get the center position of the specified square */
	glm::vec3 getCenter(glm::ivec2 vec) const;
	void getRightQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getLeftQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getFrontQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	void getBackQuad(glm::vec3* triangle, unsigned short int xOffset, unsigned short int zOffset);
	/* Adding object to grid*/
	void addObject(GameObject* object, gridType gridType);
	std::shared_ptr<Path> generatePath(glm::ivec2 startPosition, glm::ivec2 goalPosition);
	float getWallDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist);
	float getObjectDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist, glm::vec3 playerPos);
};
