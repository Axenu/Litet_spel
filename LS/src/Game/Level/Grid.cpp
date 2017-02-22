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


float Grid::getWallDist(glm::vec3 pos, glm::vec3 ray, float guardViewDist)
{
	GridTraveler trav(GRIDSPACE, getSquare(pos), pos, ray);
	float dist = 0.f;
	do
	{
		dist += trav.goNext();
	} while ((*this)[trav.getSquare()] != gridType::wall && dist < guardViewDist);
	return dist;
}

float Grid::getObjectDist(glm::vec3 guardPos, glm::vec3 ray, float guardViewDist, glm::vec3 playerPos)
{

	GridTraveler trav(GRIDSPACE, getSquare(guardPos), guardPos, ray);
	float dist = 0.f;
	while (dist < guardViewDist)
	{
		dist += trav.goNext();
		if (!isInside(trav.getSquare()))
			return dist;

		//Check if each square is 'viewable'?
		float heightDifference = abs(playerPos.y - getHeight(trav.getSquare().y, trav.getSquare().x));
		heightDifference = 100.0f * (heightDifference / playerPos.y);

		if (heightDifference < 75.0f)
		{
			return 0.0f;
		}
	}
	return dist;
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

bool Grid::testForClimb(glm::vec3 & pos, glm::vec3 &dir, float &heightDiff)
{
	glm::ivec2 moveVec;
	if (abs(dir.x) > abs(dir.z))
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
			heightDiff = _twodArray[iPos.y][iPos.x].height - pos.y;
			heightDiff = abs(heightDiff);
			if (heightDiff > WalkHeight)
			{
				pos = getCenter(iPos);
				pos.y = _twodArray[iPos.y][iPos.x].height;
				return true;
			}
		}
	}
	return false;
}

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

glm::vec3 Grid::wallCollission(glm::vec3 position, glm::vec3 velocity)
{
	//calculate current grid position
	int currentX = (int)glm::floor(position.x / GRIDSPACE);
	int currentZ = (int)glm::floor(position.z / GRIDSPACE);

	if (currentX <= 0 || currentZ <= 0 || currentX > _widthLength || currentZ > _heightLength)
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
			if (abs(heightDiff) < WalkHeight)
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
			if (abs(heightDiff) < WalkHeight)
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
			if (abs(heightDiff) < WalkHeight)
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
			if (abs(heightDiff) < WalkHeight)
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


int Grid::getHeight()
{
	return _heightLength;
}

int Grid::getWidth()
{
	return _widthLength;
}

std::vector<glm::vec3>* Grid::getLootLocations()
{
	return &_lootLocations;
}

std::vector<glm::vec3>* Grid::getGuardLocations()
{
	return &_guardLocations;
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

gridType Grid::gettype(int height, int width)
{
	return _twodArray[height][width].type;
}

void Grid::addLight(glm::vec3 lightPos, glm::vec3 diff, float dist)
{
	lightValues light;
	light.pos = lightPos;
	light.diffuse = diff;
	light.dist = dist;
	_light.push_back(light);
}


std::shared_ptr<Path> Grid::generatePath(glm::ivec2 startPosition, glm::ivec2 goalPosition)
{
	int maxValue = _heightLength * _widthLength - 1;
	int oldMaxValue = 0;
	int value = 0;
	//-1 outforskadmark, ej gåbart
	// -2 vägg
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			setvalue(j, i, -1);
		}
	}
	if(isInside(startPosition)) 
		setvalue(startPosition.y, startPosition.x, 0);

	while (maxValue != 0)
	{
		oldMaxValue = maxValue;
		for (int j = 0; j < _heightLength; j++)
		{
			for (int i = 0; i < _widthLength; i++)
			{
				if (gettype(j, i) == nothing || gettype(j, i) == guard)
				{
					if (getvalue(j, i) == value)
					{
						if (j <= 0)
						{

						}
						else if (j > 0 && getvalue(j - 1, i) == -1 && (gettype(j - 1, i) == nothing || gettype(j - 1, i) == guard))
						{
							setvalue(j - 1, i, value + 1);
							maxValue--;
						}

						if (j >= _heightLength - 1)
						{

						}
						else if (j < _heightLength && getvalue(j + 1, i) == -1 && (gettype(j + 1, i) == nothing || gettype(j + 1, i) == guard))
						{
							setvalue(j + 1, i, value + 1);
							maxValue--;
						}

						if (i <= 0)
						{

						}
						else if (i > 0 && getvalue(j, i - 1) == -1 && (gettype(j, i - 1) == nothing || gettype(j, i - 1) == guard))
						{
							setvalue(j, i - 1, value + 1);
							maxValue--;
						}

						if (i >= _widthLength - 1)
						{

						}
						else if (i < _widthLength && getvalue(j, i + 1) == -1 && (gettype(j, i + 1) == nothing || gettype(j, i + 1) == guard))
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
	}
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (gettype(j, i) == wall)
			{
				setvalue(j, i, -2);
			}
		}
	}

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

	for (int i = 0; i < path.size(); i++)
	{
		path[i] += glm::vec3(GRIDSPACE / 2.f, 0.f, GRIDSPACE / 2.f);
	}
	return std::shared_ptr<Path>(new Path(path));
}

float Grid::getHeight(int height, int width)
{
	return _twodArray[height][width].height;
}

float Grid::calcLightOnPosition(glm::vec3 playerPos)
{
	float wallDist = 0.0f;
	glm::vec3 posColor(0.0f);

	for (unsigned int i = 0; i < _light.size(); i++)
	{
		glm::vec3 lightRay = playerPos - _light[i].pos;

		if (glm::length(lightRay) < _light[i].dist)
		{
			wallDist = getWallDist(_light[i].pos, lightRay, _light[i].dist);

			if (wallDist > glm::length(lightRay) || wallDist == 0.0f)
			{
				lightRay = glm::normalize(lightRay);
				float diff = glm::max(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), lightRay), 0.0f);
				float distance = glm::length(lightRay);
				float att = glm::max(1.0f - (distance / _light[i].dist), 0.0f);

				posColor += _light[i].diffuse * diff * att;
			}
		}
	}

	return (posColor.x + posColor.y + posColor.z + 0.5f);
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
			else if (glm::vec3(data[j], data[j + 1], data[j + 2]) == glm::vec3(0, 255, 0))
			{
				_twodArray[height - 1 - i][realj].type = guard;
				glm::vec3 tmpVec((realj), 1.3f, (height - 1 - i));
				this->_guardLocations.push_back(tmpVec);
			}
			else if (data[j] == 255 && data[j + 1] == 255 && data[j + 2] == 0)
			{
				glm::vec3 tmpVec((realj * GRIDSPACE + 0.5f * GRIDSPACE), 0.0f, (((height - 1 - i) * GRIDSPACE) + 0.5f * GRIDSPACE));
				this->_lootLocations.push_back(tmpVec);
				_twodArray[height - 1 - i][realj].type = loot;
			}
			else if (data[j] == 100 && data[j + 1] == 100 && data[j + 2] == 100)
			{
				glm::vec3 tmpVec((realj * GRIDSPACE + 0.5f * GRIDSPACE), 0.0f, (((height - 1 - i) * GRIDSPACE) + 0.5f * GRIDSPACE));
				_twodArray[height - 1 - i][realj].type = object;
			}
			else
			{
				std::cout << "error" << std::endl;
				std::cout << i << "," << realj << std::endl;
			}
			realj++;
		}
	}
	delete[] data;
	fclose(f);
}

Mesh Grid::generateMesh()
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<GLuint> indices;
	GLint k = 0;

	// Position
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, 0.f));
	position.push_back(glm::vec3(0.f, 0.f, 0.f));
	position.push_back(glm::vec3(0.f, 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f, 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, 0.f));
	// Normals
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	normal.push_back(glm::vec3(0.f, 1.f, 0.f));
	// Index
	indices.push_back(k);
	indices.push_back(k + 1);
	indices.push_back(k + 2);
	indices.push_back(k + 3);
	indices.push_back(k + 4);
	indices.push_back(k + 5);
	k += 6;

	// Roof

	// Position
	position.push_back(glm::vec3(0.f, ROOFHEIGHT, 0.f));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, 0.f));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f, ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f, ROOFHEIGHT, 0.f));
	// Normals
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	normal.push_back(glm::vec3(0.f, -1.f, 0.f));
	// Index
	indices.push_back(k);
	indices.push_back(k + 1);
	indices.push_back(k + 2);
	indices.push_back(k + 3);
	indices.push_back(k + 4);
	indices.push_back(k + 5);
	k += 6;
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (_twodArray[j][i].type != wall)
			{
				if (j != 0)
				{
					if (_twodArray[j - 1][i].type == wall)
					{
						// Positions
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position.push_back(glm::vec3(i      * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position.push_back(glm::vec3(i      * GRIDSPACE, 0.f, j * GRIDSPACE));
						position.push_back(glm::vec3(i      * GRIDSPACE, 0.f, j * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, j * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						// Normals
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						normal.push_back(glm::vec3(0.f, 0.f, 1.f));
						// Index
						indices.push_back(k);
						indices.push_back(k + 1);
						indices.push_back(k + 2);
						indices.push_back(k + 3);
						indices.push_back(k + 4);
						indices.push_back(k + 5);
						k += 6;
					}
				}
				if (j != _heightLength - 1)
				{
					if (_twodArray[j + 1][i].type == wall)
					{
						// Positions
						position.push_back(glm::vec3(i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i      * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						// Normals
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						normal.push_back(glm::vec3(0.f, 0.f, -1.f));
						// Index
						indices.push_back(k);
						indices.push_back(k + 1);
						indices.push_back(k + 2);
						indices.push_back(k + 3);
						indices.push_back(k + 4);
						indices.push_back(k + 5);
						k += 6;
					}
				}
				if (i != 0)
				{
					if (_twodArray[j][i + 1].type == wall)
					{
						// Positions
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						// Normals
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
						// Index
						indices.push_back(k);
						indices.push_back(k + 1);
						indices.push_back(k + 2);
						indices.push_back(k + 3);
						indices.push_back(k + 4);
						indices.push_back(k + 5);
						k += 6;
					}
				}
				if (i != _widthLength - 1)
				{
					if (_twodArray[j][i - 1].type == wall)
					{
						// Positions
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT, j      * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f, j      * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT, j      * GRIDSPACE));
						// Normals
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						normal.push_back(glm::vec3(1.f, 0.f, 0.f));
						// Index
						indices.push_back(k);
						indices.push_back(k + 1);
						indices.push_back(k + 2);
						indices.push_back(k + 3);
						indices.push_back(k + 4);
						indices.push_back(k + 5);
						k += 6;
					}
				}
			}
		}
	}
	Mesh mesh;
	mesh.setMesh(position, normal, indices);
	return mesh;
}

void Grid::print2darraydata()
{
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			std::cout << _twodArray[j][i].type;
		}
		std::cout << "" << std::endl;
	}
}

#pragma endregion