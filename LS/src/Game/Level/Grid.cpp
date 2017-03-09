#include "Game/Level/Grid.h"
#include "math/MathFunctions.h"
#include "math/GridTraveler.h"
#pragma warning(disable:4996)

Grid::Grid(const std::string &level)
{
	loadingBmpPicture(level.c_str());
}

Grid::~Grid()
{
	for (int j = 0; j < _heightLength; j++)
	{
		delete[] _twodArray[j];
	}
	delete[] _twodArray;
}


#pragma region MCode


bool Grid::getDist(glm::vec3 pos, glm::vec3 ray, float viewDist, gridType gridType)
{
	float objectDist = 0.0f;
	return getDist(pos, ray, viewDist, objectDist, gridType);
}

bool Grid::getDist(glm::vec3 pos, glm::vec3 ray, float viewDist, float &objectDist, gridType gridType)
{
	GridTraveler trav(GRIDSPACE, getSquare(pos), pos, ray);
	float dist = 0.f;
	do
	{
		if ((*this)[trav.getSquare()] == gridType)
		{
			objectDist = dist;
			return true;
		}

		dist += trav.goNext();

	} while (dist < viewDist);

	return false;
}

bool Grid::getDist(glm::vec3 pos, glm::vec3 ray, float viewDist, glm::vec3 playerPos, gridType gridType)
{
	GridTraveler trav(GRIDSPACE, getSquare(pos), pos, ray);
	float dist = 0.f;
	float height = 0.0f;
	float objectDist = 0.0f;
	do
	{
		if ((*this)[trav.getSquare()] == gridType)
		{
			height = std::max(getHeight(trav.getSquare().y, trav.getSquare().x), height);
		}

		dist += trav.goNext();

	} while (dist < viewDist);

	if (height < playerPos.y * 0.8f)
	{
		return false;
	}

	return true;
}

#pragma region Grid Square

bool Grid::isInside(glm::ivec2 sq) const
{
	return sq.x >= 0 && sq.x < _widthLength && sq.y >= 0 && sq.y < _heightLength;
}
AARect Grid::getSquareRect(glm::ivec2 square) const {
	return AARect(square, glm::vec2(square.x + GRIDSPACE, square.y + GRIDSPACE));
}
glm::ivec2 Grid::getRandomSquare()
{
	return randIVec2(_widthLength, _heightLength);
}

glm::ivec2 Grid::getSquare(const glm::vec3 &pos) const
{
	return glm::ivec2(glm::floor(pos.x / GRIDSPACE), glm::floor(pos.z / GRIDSPACE));
}
float Grid::getGridHeight(const glm::vec3 & pos) const
{
	glm::ivec2 gridPos = getSquare(pos);
	if (isInside(gridPos))
	{
		return _twodArray[gridPos.y][gridPos.x].height;
	}
	else
	{
		return 0.0f;
	}
}
GridSquare Grid::operator[](glm::vec3 vec) const
{
	glm::ivec2 sq = getSquare(vec);
	return isInside(sq) ? GridSquare(sq, _twodArray[sq.y][sq.x].type) : GridSquare();
}

gridType Grid::operator[](const glm::ivec2 &sq) const
{
	return isInside(sq) ? _twodArray[sq.y][sq.x].type : gridType::wall;
}

glm::vec3 Grid::getCenter(glm::ivec2 sq) const
{
	return glm::vec3((sq.x + 0.5f) * GRIDSPACE, 0.f, (sq.y + 0.5f) * GRIDSPACE);
}

#pragma endregion
#pragma endregion

void Grid::addObject(GameObject * object, gridType gridType)
{

	AABB oAABB = object->getAABB();
	glm::vec3 min = oAABB.getMin();
	glm::vec3 max = oAABB.getMax();
	//get aabb height
	float height = max.y;
	//Round edges
	min += 0.5f;
	max -= 0.5f;
	glm::ivec2 start = getSquare(min);
	glm::ivec2 end = getSquare(max);

	for (int i = start.y; i <= end.y; i++)
	{
		for (int j = start.x; j <= end.x; j++)
		{
			_twodArray[i][j].type = gridType;
			_twodArray[i][j].height = height;
		}
	}
}


int Grid::getHeight()
{
	return _heightLength;
}

int Grid::getWidth()
{
	return _widthLength;
}


float Grid::getGridSpace()
{
	return GRIDSPACE;
}

int Grid::getvalue(int height, int width)
{
	return _twodArray[height][width].value;
}

void Grid::setvalue(int height, int width, int value)
{
	_twodArray[height][width].value=value;
}

gridType Grid::getTypeNC(int height, int width)
{
	return _twodArray[height][width].type;
}

std::shared_ptr<Path> Grid::generatePath(glm::ivec2 startPosition, glm::ivec2 goalPosition, int maxRange)
{
	if (startPosition == goalPosition)
	{
		std::vector<glm::vec3> emptyPath;
		return std::shared_ptr<Path>(new Path(emptyPath));
	}
	//print2darraydata();
	int maxValue = maxRange * maxRange;
	int oldMaxValue = 0;
	int value = 0;
	//-1 outforskadmark, ej gåbart
	// -2 vägg
	int minX = clampInt(startPosition.x - maxRange, 0, _widthLength);
	int maxX = clampInt(startPosition.x + maxRange, 0, _widthLength);
	int minY = clampInt(startPosition.y - maxRange, 0, _heightLength);
	int maxY = clampInt(startPosition.y + maxRange, 0, _heightLength);

	if(isInside(startPosition))
		setvalue(startPosition.y, startPosition.x, 0);

	for (int j = minY; j < maxY; j++)
	{
		for (int i = minX; i < maxX; i++)
		{
			if (getTypeNC(j, i) == wall)
			{
				setvalue(j, i, -2);
			}
		}
	}

	std::vector<glm::ivec2> currentPositions;
	currentPositions.push_back(startPosition);

	while (currentPositions.size() != 0)
	{
		std::vector<glm::ivec2> nextPositions;
		for (int i = 0; i < currentPositions.size(); i++)
		{
			if (currentPositions[i].y - 1 > minY && getvalue(currentPositions[i].y - 1, currentPositions[i].x) == -1)
			{
				nextPositions.push_back(glm::ivec2(currentPositions[i].x, currentPositions[i].y - 1));
				setvalue(currentPositions[i].y - 1, currentPositions[i].x, value + 1);
			}
			if (currentPositions[i].y + 1 < maxY && getvalue(currentPositions[i].y + 1, currentPositions[i].x) == -1)
			{
				nextPositions.push_back(glm::ivec2(currentPositions[i].x, currentPositions[i].y + 1));
				setvalue(currentPositions[i].y + 1, currentPositions[i].x, value + 1);
			}
			if (currentPositions[i].x - 1 > minX && getvalue(currentPositions[i].y, currentPositions[i].x - 1) == -1)
			{
				nextPositions.push_back(glm::ivec2(currentPositions[i].x - 1, currentPositions[i].y));
				setvalue(currentPositions[i].y, currentPositions[i].x - 1, value + 1);
			}
			if (currentPositions[i].x + 1 < maxX && getvalue(currentPositions[i].y, currentPositions[i].x + 1) == -1)
			{
				nextPositions.push_back(glm::ivec2(currentPositions[i].x + 1, currentPositions[i].y));
				setvalue(currentPositions[i].y, currentPositions[i].x + 1, value + 1);
			}
		}
		currentPositions = nextPositions;
		value++;
	}
	/*while (maxValue != 0)
	{
		oldMaxValue = maxValue;
		for (int j = minY; j < maxY; j++)
		{
			for (int i = minX; i < maxX; i++)
			{
				if (getTypeNC(j, i) == nothing)
				{
					if (getvalue(j, i) == value)
					{
						if (j > 0 && getvalue(j - 1, i) == -1 && getTypeNC(j - 1, i) == nothing)
						{
							setvalue(j - 1, i, value + 1);
							maxValue--;
						}

						if (j < _heightLength && getvalue(j + 1, i) == -1 && getTypeNC(j + 1, i) == nothing)
						{
							setvalue(j + 1, i, value + 1);
							maxValue--;
						}

						if (i > 0 && getvalue(j, i - 1) == -1 && getTypeNC(j, i - 1) == nothing)
						{
							setvalue(j, i - 1, value + 1);
							maxValue--;
						}
						if (i < _widthLength && getvalue(j, i + 1) == -1 && getTypeNC(j, i + 1) == nothing)
						{
							setvalue(j, i + 1, value + 1);
							maxValue--;
						}
					}
				}
			}
		}
		if (oldMaxValue == maxValue)
		{
			break;
		}
		value++;
	}*/

	for (int j = minY; j < maxY; j++)
	{
		for (int i = minX; i < maxX; i++)
		{
			if (getTypeNC(j, i) == wall)
			{
				setvalue(j, i, -2);
			}
		}
	}
	//print2darraydata();
	std::vector<glm::vec3> path;

	glm::ivec2 currentPos = goalPosition;
	if (getvalue(goalPosition.y, goalPosition.x) > 0)
	{
		path.push_back(glm::vec3(goalPosition.x, 0.f, goalPosition.y));
	}

	int currentValue = getvalue(goalPosition.y, goalPosition.x);
	while (currentValue > 1)
	{
		if (getvalue(currentPos.y - 1, currentPos.x)<currentValue && getvalue(currentPos.y - 1, currentPos.x) >= 0)
		{
			currentPos.y -= 1;
			path.push_back(glm::vec3(currentPos.x, 0.f, currentPos.y));
		}
		else if (getvalue(currentPos.y + 1, currentPos.x)<currentValue && getvalue(currentPos.y + 1, currentPos.x) >= 0)
		{
			currentPos.y += 1;
			path.push_back(glm::vec3(currentPos.x, 0.f, currentPos.y));
		}
		else if (getvalue(currentPos.y, currentPos.x - 1)<currentValue && getvalue(currentPos.y, currentPos.x - 1) >= 0)
		{
			currentPos.x -= 1;
			path.push_back(glm::vec3(currentPos.x, 0.f, currentPos.y));
		}
		else if (getvalue(currentPos.y, currentPos.x + 1)<currentValue && getvalue(currentPos.y, currentPos.x + 1) >= 0)
		{
			currentPos.x += 1;
			path.push_back(glm::vec3(currentPos.x, 0.f, currentPos.y));
		}
		currentValue--;
	}

	//reset used squares
	for (int j = clampInt(minY - 1, 0, _heightLength); j < clampInt(maxY + 1, 0, _heightLength); j++)
	{
		for (int i = clampInt(minX - 1, 0, _widthLength); i < clampInt(maxX + 1, 0, _widthLength); i++)
		{
			setvalue(j, i, -1);
		}
	}

	for (unsigned int i = 0; i < path.size(); i++)
	{
		path[i] += glm::vec3(GRIDSPACE / 2.f, 0.f, GRIDSPACE / 2.f);
	}
	return std::shared_ptr<Path>(new Path(path));
}

float Grid::getHeight(int height, int width)
{
	return _twodArray[height][width].height;
}





#pragma region Build/Load

void Grid::buildgridarray()
{
	//building the 2D array
	_twodArray = new gridValues*[_heightLength];
	for (int j = 0; j < _heightLength; j++)
	{
		_twodArray[j] = new gridValues[_widthLength];
	}
}

void Grid::loadingBmpPicture(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		throw "Argument Exception";
	}

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	_heightLength = height;
	_widthLength = width;


	buildgridarray();

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;
	int realj = 0;
	for (int i = 0; i < height; i++)
	{
		realj = 0;
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;
			_twodArray[height - 1 - i][realj].height = 0.0f;
			if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(255, 255, 255))
			{
				_twodArray[height - 1 - i][realj].type = wall;
			}
			else if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(0, 0, 0))
			{
				_twodArray[height - 1 - i][realj].type = nothing;
			}
			else if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(255, 0, 0))
			{
				_twodArray[height - 1 - i][realj].type = exiting;
			}
			else if (data[j] == 100 && data[j + 1] == 100 && data[j + 2] == 100)
			{
				glm::vec3 tmpVec((realj * GRIDSPACE + 0.5f * GRIDSPACE), 0.0f, (((height - 1 - i) * GRIDSPACE) + 0.5f * GRIDSPACE));
				_twodArray[height - 1 - i][realj].type = object;
			}
			else if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(0, 255, 0))
			{
				_twodArray[height - 1 - i][realj].type = window;
			}
			else if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(0, 0, 255))
			{
				_twodArray[height - 1 - i][realj].type = door;
			}
			_twodArray[height - 1 - i][realj].value = -1;
			realj++;
		}
	}
	delete[] data;
	fclose(f);
}

void Grid::generateMesh(Mesh* meshes, float windowMin, float windowMax, float windowDepth)
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<GLuint> indices;
	GLint k = 0;

	//Floor
	std::vector<glm::vec3> floorPosition;
	std::vector<glm::vec3> floorNormal;
	std::vector<GLuint> floorIndices;

	addQuad(floorPosition, floorNormal, floorIndices, k, glm::vec3(0.f, 0.f, _heightLength * GRIDSPACE), glm::vec3(_widthLength * GRIDSPACE, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

	meshes[0].setMesh(floorPosition, floorNormal, floorIndices);
	k = 0;
	///////


	//Roof
	std::vector<glm::vec3> roofPosition;
	std::vector<glm::vec3> roofNormal;
	std::vector<GLuint> roofIndices;

	addQuad(roofPosition, roofNormal, roofIndices, k, glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, _heightLength * GRIDSPACE), glm::vec3(0.f, ROOFHEIGHT, 0.f), glm::vec3(0.f, 0.f, -1.f));

	meshes[1].setMesh(roofPosition, roofNormal, roofIndices);
	k = 0;
	//////

	//walls
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (_twodArray[j][i].type != wall && _twodArray[j][i].type != window)
			{
				if (j != 0)
				{
					if (_twodArray[j - 1][i].type == wall)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 0.f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j - 1][i].type == window)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 0.f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, windowMin, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, windowMax, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (ROOFHEIGHT > 2.3f && _twodArray[j - 1][i].type == door)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 2.3f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
				}
				if (j != _heightLength - 1)
				{
					if (_twodArray[j + 1][i].type == wall)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j + 1][i].type == window)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, windowMin, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, windowMax, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j + 1][i].type == door)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 2.3f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
				}
				if (i != 0)
				{
					if (_twodArray[j][i + 1].type == wall)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 0.f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j][i + 1].type == window)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 0.f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, windowMin, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, windowMax, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j][i + 1].type == door)
					{
						addQuad(position, normal, indices, k, glm::vec3((i + 1) * GRIDSPACE, 2.3f, j * GRIDSPACE), glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
				}
				if (i != _widthLength - 1)
				{
					if (_twodArray[j][i - 1].type == wall)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j][i - 1].type == window)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, windowMin, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, windowMax, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
					else if (_twodArray[j][i - 1].type == door)
					{
						addQuad(position, normal, indices, k, glm::vec3(i * GRIDSPACE, 2.3f, (j + 1) * GRIDSPACE), glm::vec3(i * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE), glm::vec3(0.f, 1.f, 0.f));
					}
				}
			}
		}
	}

	// Change all window types to walls such that the collision will treath the windows as walls
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (_twodArray[j][i].type == window)
			{
				_twodArray[j][i].type = wall;
				_windowData.push_back(glm::ivec2(i, j));
			}
			else if (_twodArray[j][i].type == door)
			{
				_twodArray[j][i].type = nothing;
			}
		}
	}
	meshes[2].setMesh(position, normal, indices);
}

void Grid::addQuad(std::vector<glm::vec3>& positionList, std::vector<glm::vec3>& normalList, std::vector<GLuint>& indicesList, GLint &k, glm::vec3 quadMin, glm::vec3 quadMax, glm::vec3 upVector)
{
	glm::vec3 minMax = quadMax - quadMin;
	float len = glm::dot(upVector, minMax);
	glm::vec3 topLeft(quadMin + len * upVector);
	glm::vec3 bottomRight(quadMax - len * upVector);

	positionList.push_back(quadMax);
	positionList.push_back(topLeft);
	positionList.push_back(quadMin);

	positionList.push_back(quadMax);
	positionList.push_back(quadMin);
	positionList.push_back(bottomRight);

	glm::vec3 normal(glm::normalize(glm::cross(topLeft - quadMax, bottomRight - quadMax)));

	normalList.push_back(normal);
	normalList.push_back(normal);
	normalList.push_back(normal);
	normalList.push_back(normal);
	normalList.push_back(normal);
	normalList.push_back(normal);

	indicesList.push_back(k);
	indicesList.push_back(k + 1);
	indicesList.push_back(k + 2);
	indicesList.push_back(k + 3);
	indicesList.push_back(k + 4);
	indicesList.push_back(k + 5);
	k += 6;

	return;
}

std::vector<glm::ivec2> Grid::getWindowData()
{
	return _windowData;
}

void Grid::print2darraydata()
{
	for (int j = 0; j < _heightLength; j++)
	{
		std::cout << "(";
		for (int i = 0; i < _widthLength; i++)
		{
			if (i == _widthLength - 1)
				std::cout << _twodArray[j][i].value << ")";
			else
				std::cout << _twodArray[j][i].value << ",";
		}
		std::cout << "" << std::endl;
	}
	std::cout << std::endl;
}

#pragma endregion

#pragma region ToBeMoved/Improved
/* Move */
bool Grid::testForClimb(glm::vec3 & pos, glm::vec3 &dir, float &heightDiff, float charHeight)
{
	glm::ivec2 moveVec;

	if (std::abs(dir.x) > std::abs(dir.z))
	{
		if (dir.x > 0.0f)
		{
			moveVec = glm::ivec2(1, 0);
		}
		else
		{
			moveVec = glm::ivec2(-1, 0);
		}
	}
	else
	{
		if (dir.z > 0.0f)
		{
			moveVec = glm::ivec2(0, 1);
		}
		else
		{
			moveVec = glm::ivec2(0, -1);
		}
	}
	glm::ivec2 iPos = getSquare(pos);
	iPos += moveVec;
	if (isInside(iPos))
	{
		if (_twodArray[iPos.y][iPos.x].type != gridType::wall)
		{
			if ((_twodArray[iPos.y][iPos.x].height + charHeight) < ROOFHEIGHT)
			{
				heightDiff = _twodArray[iPos.y][iPos.x].height - pos.y;
				heightDiff = std::abs(heightDiff);
				if (heightDiff > WalkHeight)
				{
					pos = getCenter(iPos);
					pos.y = _twodArray[iPos.y][iPos.x].height;
					return true;
				}
			}
		}
	}
	return false;
}

/* Improvable */
glm::vec3 Grid::wallCollission(glm::vec3 position, glm::vec3 velocity)
{
	//calculate current grid position
	int currentX = (int)glm::floor(position.x / GRIDSPACE);
	int currentZ = (int)glm::floor(position.z / GRIDSPACE);

	if (currentX <= 0 || currentZ <= 0 || currentX > _widthLength - 2 || currentZ > _heightLength - 2)
	{
		position.x += velocity.x;
		position.z += velocity.z;
		return position;
	}

	//Determine which direction the player is moving, stop the player 0.3 units before the wall if there is a wall to the right or left
	if (signbit(velocity.x) == false)
	{
		if (_twodArray[currentZ][currentX + 1].type == wall)
		{
			if (position.x - currentX < 0.7f)
			{
				position.x += velocity.x;
			}
		}
		else
		{
			float heightDiff = _twodArray[currentZ][currentX + 1].height - position.y;
			//std::cout << heightDiff << std::endl;
			if (std::abs(heightDiff) < WalkHeight)
			{
				position.x += velocity.x;
			}
			else if (position.x - currentX < 0.7f)
			{
				position.x += velocity.x;
			}
		}
	}
	else
	{
		if (_twodArray[currentZ][currentX - 1].type == wall)
		{
			if (position.x - currentX > 0.3f)
			{
				position.x += velocity.x;
			}
		}
		else
		{
			float heightDiff = _twodArray[currentZ][currentX - 1].height - position.y;
			//std::cout << heightDiff << std::endl;
			if (std::abs(heightDiff) < WalkHeight)
			{
				position.x += velocity.x;
			}
			else if (position.x - currentX > 0.3f)
			{
				position.x += velocity.x;
			}
		}
	}

	//Determine which direction the player is moving, stop the player 0.3 units before the wall if there is a wall to the forward or backward
	if (signbit(velocity.z) == false)
	{
		if (_twodArray[currentZ + 1][currentX].type == wall)
		{
			if (position.z - currentZ < 0.7f)
			{
				position.z += velocity.z;
			}
		}
		else
		{
			float heightDiff = _twodArray[currentZ + 1][currentX].height - position.y;
			//std::cout << heightDiff << std::endl;
			if (std::abs(heightDiff) < WalkHeight)
			{
				position.z += velocity.z;
			}
			else if (position.z - currentZ < 0.7f)
			{
				position.z += velocity.z;
			}
		}
	}
	else
	{
		if (_twodArray[currentZ - 1][currentX].type == wall)
		{
			if (position.z - currentZ > 0.3f)
			{
				position.z += velocity.z;
			}
		}
		else
		{
			float heightDiff = _twodArray[currentZ - 1][currentX].height - position.y;
			//std::cout << heightDiff << std::endl;
			if (std::abs(heightDiff) < WalkHeight)
			{
				position.z += velocity.z;
			}
			else if (position.z - currentZ > 0.3f)
			{
				position.z += velocity.z;
			}
		}
	}
	//Check if there's a wall in the north east square, if true, move the player 0.3 units away from the corner in the direction he was moving
	if (_twodArray[currentZ - 1][currentX - 1].type == wall && _twodArray[currentZ - 1][currentX - 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX - position.x, 0.f, (float)currentZ - position.z);
		float len = sqrtf(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position.x = (float)currentX - dist.x * 0.3f;
			position.z = (float)currentZ - dist.z * 0.3f;
		}
	}
	//north west square
	if (_twodArray[currentZ - 1][currentX + 1].type == wall && _twodArray[currentZ - 1][currentX + 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX + 1 - position.x, 0.f, (float)currentZ - position.z);
		float len = sqrtf(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position.x = (float)currentX + 1 - dist.x * 0.3f;
			position.z = (float)currentZ - dist.z * 0.3f;
		}
	}
	//south east square
	if (_twodArray[currentZ + 1][currentX - 1].type == wall && _twodArray[currentZ + 1][currentX - 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX - position.x, 0.f, (float)currentZ + 1 - position.z);
		float len = sqrtf(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position.x = (float)currentX - dist.x * 0.3f;
			position.z = (float)currentZ + 1 - dist.z * 0.3f;
		}
	}
	//south west square
	if (_twodArray[currentZ + 1][currentX + 1].type == wall && _twodArray[currentZ + 1][currentX + 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX + 1 - position.x, 0.f, (float)currentZ + 1 - position.z);
		float len = sqrtf(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position.x = (float)currentX + 1 - dist.x * 0.3f;
			position.z = (float)currentZ + 1 - dist.z * 0.3f;
		}
	}
	return position;
}


bool Grid::wallCollissionForGrenade(glm::vec3 position, glm::vec3 velocity)
{
	//calculate current grid position
	int currentX = (int)glm::floor(position.x / GRIDSPACE);
	int currentZ = (int)glm::floor(position.z / GRIDSPACE);

	if (currentX < 0 || currentZ < 0 || currentX > _widthLength - 1 || currentZ > _heightLength - 1)
	{
		return false;
	}

	if (_twodArray[currentZ][currentX].type == wall)
	{
		return true;
	}
	else if (_twodArray[currentZ][currentX].type == object && _twodArray[currentZ][currentX].height > position.y)
	{
		return true;
	}
	else
	{
		if (position.y < 0.15f || position.y > ROOFHEIGHT - 0.15f)
		{
			return true;
		}
	}
	return false;
}

/* Move */

#pragma endregion
