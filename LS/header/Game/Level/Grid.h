#pragma once

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
#include "math/intersectionFunctions.h"
#include "math/AARect.h"
#include "Game/Level/Path.h"
#include <memory>

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
	std::vector<glm::vec3> _lootLocations;
	std::vector<glm::vec3> _guardLocations;
	std::vector<lightValues> _light;

	void buildgridarray();
	void print2darraydata();
	void loadingBmpPicture(const char* filename);
public:
	Grid(const std::string& level);
	~Grid();
	void generateMesh(Mesh* meshes);

	/* Get height size of grid in squares */
	int getHeight();
	/* Get width size of grid in squares */
	int getWidth();
	/* Get the size of a grid square */
	float getGridSpace();
	int getvalue(int height,int width);
	void setvalue(int height, int width, int value);
	/* Get grid square type not checking if inside */
	gridType getTypeNC(int height, int width);

#pragma region Mfuncs
	/* Get distance to closest wall */
	float getWallDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist);
	/* Get distance to closest blocking object...?*/
	float getObjectDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist, glm::vec3 playerPos);
	/* Verify a grid square is represented in the grid
	*/
	bool isInside(glm::ivec2 vec) const;
	/* Get the grid square associated with the position value
	*/
	glm::ivec2 getSquare(const glm::vec3 &pos) const;
	/* Get the rectangle representing a square */
	AARect getSquareRect(glm::ivec2 square) const;
	/* Get a random square in the grid
	*/
	glm::ivec2 getRandomSquare();
	/* Get the center position of the specified square. If outside wall is returned. */
	glm::vec3 getCenter(glm::ivec2 vec) const;
	GridSquare operator[](glm::vec3 vec) const;
	/* Get the grid type at a square, checking if valid square and returning wall on fail.
	*/
	gridType operator[](const glm::ivec2 &sq) const;
#pragma endregion
	float getGridHeight(const glm::vec3 &pos) const;
	/* Adding object to grid*/
	void addObject(GameObject* object, gridType gridType);
	/* Generate a walkable path */
	std::shared_ptr<Path> generatePath(glm::ivec2 startPosition, glm::ivec2 goalPosition);

	float getHeight(int height, int width);

	//These funcs should not be in grid: /Mattias

	bool testForClimb(glm::vec3 &pos, glm::vec3 &dir, float &heightDiff);
	glm::vec3 wallCollission(glm::vec3 position, glm::vec3 velocity);
	void addLight(glm::vec3 lightPos, glm::vec3 diff, float dist);
	float calcLightOnPosition(glm::vec3 playerPos);
	std::vector<glm::vec3> * getLootLocations();
	std::vector<glm::vec3> *  getGuardLocations();
};
