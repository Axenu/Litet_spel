#include "GridDataStructure.h"
#pragma warning(disable:4996)




Grid::Grid(const std::string &level)
{
	loadingBmpPicture(level.c_str());
}

//här raderar du








int Grid::IsInVector(glm::ivec2 pos, std::vector<gridNode>* vector)
{
	for (unsigned int i = 0; i < vector->size(); i++)
	{
		if (vector->at(i).position == pos)
		{
			return i;
		}
	}
	return -1;
}

bool Grid::removeGridCell(glm::ivec2 pos, std::vector<gridNode>* vector)
{
	//checks surrounding grid cells  
	int test = IsInVector(glm::ivec2(pos.x, pos.y), vector);
	if (test >= 0)
	{
		if (vector->at(test).needsCheck == true)
		{
			return false;
		}
	}
	test = IsInVector(glm::ivec2(pos.x, pos.y - 1), vector);
	if (test >= 0)    // ^  
	{
		if (vector->at(test).needsCheck == true)
		{
			return false;
		}
	}
	test = IsInVector(glm::ivec2(pos.x + 1, pos.y), vector);
	if (test >= 0)    // >  
	{
		if (vector->at(test).needsCheck == true)
		{
			return false;
		}
	}
	test = IsInVector(glm::ivec2(pos.x, pos.y + 1), vector);
	if (test >= 0)    // v  
	{
		if (vector->at(test).needsCheck == true)
		{
			return false;
		}
	}
	test = IsInVector(glm::ivec2(pos.x - 1, pos.y), vector);
	if (test >= 0)    // <  
	{
		if (vector->at(test).needsCheck == true)
		{
			return false;
		}
	}
	return true;
}
/*
Grid::Grid()
{
	loadingBmpPicture((char*)"Resources/Demo1.bmp");
}
*/
Grid::~Grid()
{
	for (int j = 0; j < _heightLength; j++)
	{
		delete[] _twodArray[j];
	}
	delete[] _twodArray;
}

void Grid::buildgridarray()
{
	//building the 2D array
	_twodArray = new gridValues*[_heightLength];
	for (int j = 0; j < _heightLength; j++)
	{
		_twodArray[j] = new gridValues[_widthLength];
	}
}

glm::vec3 Grid::getData(gridType Data)
{
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (Data == exiting)
			{
				if (_twodArray[j][i].type == exiting)
				{
					return glm::vec3(i, 0, j);
				}
			}
			if (Data == guard)
			{
				if (_twodArray[j][i].type == guard)
				{
					return  glm::vec3(i ,0.25 , j);
				}
			}
		}
	}
	return glm::vec3(0, 0, 0);
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

void Grid::loadingBmpPicture(const char* filename)
{
	//
	//_heightLength = 10;
	//_widthLength = 15;
	//buildgridarray();
	//_twodArray[9][12].xz = glm::vec2(1,2);
	//f�rsta �r heightlength andra �r widthlenght
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
	{
		throw "Argument Exception";
	}

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*) &info[18];
	int height = *(int*) &info[22];

	//	std::cout<< std::endl;
	//	std::cout<< "  Name: " << filename << std::endl;
	//	std::cout << " Width: " << width << std::endl;
	//	std::cout << "Height: " << height << std::endl;
	_heightLength = height;
	_widthLength = width;


	buildgridarray();
	//	_twodArray[9][12].xz = glm::vec2(1,2);
	//f�rsta �r heightlength andra �r widthlenght
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
			//	 glm::vec3(data[j], data[j + 1], data[j + 2]);
			//	cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << endl;

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
				std::cout<<i<<","<<realj<<std::endl;
			}
//			_twodArray[height - 1 - i][realj].xz = glm::vec2(height - 1 - i, realj);
			realj++;
			//	cout << _twodArray[i][j].type;
		}
		//	cout << "" << endl;
	}
	delete[] data;
	fclose(f);
//	print2darraydata();
}

#pragma region Grid Square

bool Grid::isInside(glm::ivec2 sq) const {
	return sq.x >= 0 && sq.x < _widthLength && sq.y >= 0 && sq.y < _heightLength;
}

glm::ivec2 Grid::getSquare(const glm::vec3 &pos) const {
	return glm::ivec2(glm::floor(pos.x / GRIDSPACE), glm::floor(pos.z / GRIDSPACE));
}
GridSquare Grid::operator[](glm::vec3 vec) const {
	glm::ivec2 sq = getSquare(vec);
	return isInside(sq) ? GridSquare(sq, _twodArray[sq.y][sq.x].type) : GridSquare();
}

gridType Grid::operator[](const glm::ivec2 &sq) const {
	return isInside(sq) ? _twodArray[sq.y][sq.x].type : gridType::nothing;
}

glm::vec3 Grid::getCenter(glm::ivec2 sq) const {
	return glm::vec3((sq.x + 0.5f) * GRIDSPACE, 0.f, (sq.y + 0.5f) * GRIDSPACE);
}
#pragma endregion

Mesh Grid::generateMesh()
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<GLuint> indices;
	GLint k = 0;
	// Floor

	// Position
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, 0.f                      ));
	position.push_back(glm::vec3(0.f                     , 0.f, 0.f                      ));
	position.push_back(glm::vec3(0.f                     , 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f                     , 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, 0.f, 0.f                      ));
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
	position.push_back(glm::vec3(0.f                     , ROOFHEIGHT, 0.f                      ));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, 0.f                      ));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(_widthLength * GRIDSPACE, ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f                     , ROOFHEIGHT, _heightLength * GRIDSPACE));
	position.push_back(glm::vec3(0.f                     , ROOFHEIGHT, 0.f                      ));
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
						position.push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position.push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
						position.push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , j * GRIDSPACE));
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
						position.push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3( i      * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
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
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
						position.push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
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
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
						position.push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
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

void Grid::wallCollission(glm::vec3 *position, glm::vec3 velocity)
{
	//calculate current grid position
	int currentX = (int)glm::floor(position->x / GRIDSPACE);
	int currentZ = (int)glm::floor(position->z / GRIDSPACE);

	if (currentX <= 0 || currentZ <= 0 || currentX > _widthLength || currentZ > _heightLength)
	{
		position->x += velocity.x;
		position->z += velocity.y;
		return;
	}

	//Determine which direction the player is moving, stop the player 0.3 units before the wall if there is a wall to the right or left
	if (signbit(velocity.x) == false)
	{
		if (_twodArray[currentZ][currentX + 1].type != wall && _twodArray[currentZ][currentX + 1].type != object)
		{
			position->x += velocity.x;
		}
		else if (position->x - currentX < 0.7f)
		{
			position->x += velocity.x;
		}
	}
	else
	{
		if (_twodArray[currentZ][currentX - 1].type != wall && _twodArray[currentZ][currentX - 1].type != object)
		{
			position->x += velocity.x;
		}
		else if (position->x - currentX > 0.3f)
		{
			position->x += velocity.x;
		}
	}

	//Determine which direction the player is moving, stop the player 0.3 units before the wall if there is a wall to the forward or backward
	if (signbit(velocity.y) == false)
	{
		if (_twodArray[currentZ + 1][currentX].type != wall && _twodArray[currentZ + 1][currentX].type != object)
		{
			position->z += velocity.y;
		}
		else if (position->z - currentZ < 0.7f)
		{
			position->z += velocity.y;
		}
	}
	else
	{
		if (_twodArray[currentZ - 1][currentX].type != wall && _twodArray[currentZ - 1][currentX].type != object)
		{
			position->z += velocity.y;
		}
		else if (position->z - currentZ > 0.3f)
		{
			position->z += velocity.y;
		}
	}
	//Check if there's a wall in the north east square, if true, move the player 0.3 units away from the corner in the direction he was moving
	if (_twodArray[currentZ - 1][currentX - 1].type == wall && _twodArray[currentZ - 1][currentX - 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX - position->x, 0.f, (float)currentZ - position->z);
		float len = sqrt(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position->x = (float)currentX - dist.x * 0.3f;
			position->z = (float)currentZ - dist.z * 0.3f;
		}
	}
	//north west square
	if (_twodArray[currentZ - 1][currentX + 1].type == wall && _twodArray[currentZ - 1][currentX + 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX + 1 - position->x, 0.f, (float)currentZ - position->z);
		float len = sqrt(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position->x = (float)currentX + 1 - dist.x * 0.3f;
			position->z = (float)currentZ - dist.z * 0.3f;
		}
	}
	//south east square
	if (_twodArray[currentZ + 1][currentX - 1].type == wall && _twodArray[currentZ + 1][currentX - 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX - position->x, 0.f, (float)currentZ + 1 - position->z);
		float len = sqrt(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position->x = (float)currentX - dist.x * 0.3f;
			position->z = (float)currentZ + 1 - dist.z * 0.3f;
		}
	}
	//south west square
	if (_twodArray[currentZ + 1][currentX + 1].type == wall && _twodArray[currentZ + 1][currentX + 1].type != object)
	{
		glm::vec3 playerToCorner = glm::vec3((float)currentX + 1 - position->x, 0.f, (float)currentZ + 1 - position->z);
		float len = sqrt(playerToCorner.x * playerToCorner.x + playerToCorner.z * playerToCorner.z);
		if (len < 0.3f && len != 0)
		{
			glm::vec3 dist = playerToCorner;
			dist.x /= len;
			dist.z /= len;
			position->x = (float)currentX + 1 - dist.x * 0.3f;
			position->z = (float)currentZ + 1 - dist.z * 0.3f;
		}
	}
}

glm::vec3 Grid::getheightandwidthpoint12(int i)
{
	return pointxy[i];
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
void Grid::buildGridArrayForGuards()
{
	
}

std::vector<glm::vec3>* Grid::getGuardLocations()
{
	return &_guardLocations;
}

glm::vec3 Grid::getLastValueOfGuardLocationsAndremovesit()
{
	glm::vec3 temp;
	temp = _guardLocations[_guardLocations.size() - 1];
	_guardLocations.pop_back();

	return temp;
}

bool Grid::isAccessible(glm::ivec2 start, glm::ivec2 end)
{
	glm::ivec2 currentPos = start;
	std::vector<gridNode> nodes;
	gridNode root;
	root.needsCheck = false;
	root.position = start;
	nodes.push_back(root);
	int index;
	while (currentPos != end)
	{
		int currentIndex = IsInVector(glm::ivec2(currentPos.x, currentPos.y), &nodes);
		nodes[currentIndex].needsCheck = false;
		//checks surrounding grid cells  
		if (_twodArray[currentPos.y - 1][currentPos.x].type != wall && _twodArray[currentPos.y - 1][currentPos.x].type != exiting)    // ^  
		{
			index = IsInVector(glm::ivec2(currentPos.x, currentPos.y - 1), &nodes);
			if (index == -1)
			{
				gridNode tmp;
				tmp.needsCheck = true;
				tmp.position = glm::ivec2(currentPos.x, currentPos.y - 1);
				nodes.push_back(tmp);
			}
			else
			{
				if (removeGridCell(glm::ivec2(currentPos.x, currentPos.y - 1), &nodes))
				{
					nodes.erase(nodes.begin() + index);
				}
			}
		}
		if (_twodArray[currentPos.y][currentPos.x + 1].type != wall && _twodArray[currentPos.y][currentPos.x + 1].type != exiting)    // >  
		{
			index = IsInVector(glm::ivec2(currentPos.x + 1, currentPos.y), &nodes);
			if (index == -1)
			{
				gridNode tmp;
				tmp.needsCheck = true;
				tmp.position = glm::ivec2(currentPos.x + 1, currentPos.y);
				nodes.push_back(tmp);
			}
			else
			{
				if (removeGridCell(glm::ivec2(currentPos.x + 1, currentPos.y), &nodes))
				{
					nodes.erase(nodes.begin() + index);
				}
			}
		}
		if (_twodArray[currentPos.y + 1][currentPos.x].type != wall && _twodArray[currentPos.y + 1][currentPos.x].type != exiting)    // v  
		{
			index = IsInVector(glm::ivec2(currentPos.x, currentPos.y + 1), &nodes);
			if (index == -1)
			{
				gridNode tmp;
				tmp.needsCheck = true;
				tmp.position = glm::ivec2(currentPos.x, currentPos.y + 1);
				nodes.push_back(tmp);
			}
			else
			{
				if (removeGridCell(glm::ivec2(currentPos.x, currentPos.y + 1), &nodes))
				{
					nodes.erase(nodes.begin() + index);
				}
			}
		}
		if (_twodArray[currentPos.y][currentPos.x - 1].type != wall && _twodArray[currentPos.y][currentPos.x - 1].type != exiting)    // <  
		{
			index = IsInVector(glm::ivec2(currentPos.x - 1, currentPos.y), &nodes);
			if (index == -1)
			{
				gridNode tmp;
				tmp.needsCheck = true;
				tmp.position = glm::ivec2(currentPos.x - 1, currentPos.y);
				nodes.push_back(tmp);
			}
			else
			{
				if (removeGridCell(glm::ivec2(currentPos.x - 1, currentPos.y), &nodes))
				{
					nodes.erase(nodes.begin() + index);
				}
			}
		}
		currentIndex = IsInVector(glm::ivec2(currentPos.x, currentPos.y), &nodes);
		if (nodes.size() == 0 || nodes.size() == currentIndex + 1)
		{
			return false;
		}
		currentPos = nodes[currentIndex + 1].position;

	}
	std::cout << nodes.size() << std::endl;
	return true;
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

void Grid::Creategetheightandwidthpoint12(glm::vec3 guardposition)
{
	//0 = x1,1=x2,2=y1,3=y2
	//gridet är zx men alla andra värden i världen är xz
	int i = (int)guardposition.x; //width
	int j = (int)guardposition.z; //height
	//first wall upwards
	for (j; j > -1; j--)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[0] = glm::vec3(i, 0, j + 1);
			break;
		}
	}
	j = (int)guardposition.z;

	//second point downwards
	for (j; j < _heightLength; j++)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[1] = glm::vec3(i, 0, j - 1);
			break;
		}
	}
	j = (int)guardposition.z;
	//thirdwall left

	for (i; i > -1; i--)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[2] = glm::vec3(i + 1, 0, j);
			break;
		}
	}
	i = (int)guardposition.x;

	//fourthwall right
	for (i; i < _widthLength; i++)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[3] = glm::vec3(i - 1, 0, j);
			break;
		}
	}
}

gridType Grid::returnGridType(int width, int height)
{
	return _twodArray[height][width].type;
}

