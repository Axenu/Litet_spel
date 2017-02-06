#include "GridDataStructure.h"
#pragma warning(disable:4996)

Grid::Grid()
{
	_gotTheTreasure = false;

	loadingBmpPicture((char*)"roomtest.bmp");

	_exit = glm::vec2(getData(exiting).x,getData(exiting).z);
}

Grid::~Grid()
{
	for (int i = 0; i < _heightLength; i++)
	{
		delete[] _twodArray[i];
	}

	delete[] _twodArray;
}
void Grid::buildgridarray()
{
	//building the 2D array
	_twodArray = new gridValues*[_heightLength];
	for (int i = 0; i < _heightLength; i++)
	{
		_twodArray[i] = new gridValues[_widthLength];
	}
}


glm::vec3 Grid::getData(gridType Data)
{
	for (int i = 0; i < _heightLength; i++)
	{
		for (int j = 0; j < _widthLength; j++)
		{
			if (Data == exiting)
			{
				if (_twodArray[i][j].type == exiting)
				{
					return glm::vec3(i, 0, j);
				}
			}
			if (Data == guard)
			{
				if (_twodArray[i][j].type == guard)
				{
					return glm::vec3(i, 0, j);
				}
			}
		}

	}
	return glm::vec3(0, 0, 0);
}

void Grid::print2darraydata()
{



	for (int i = 0;i < _heightLength;i++)
	{
		for (int j = 0;j < _widthLength;j++)
		{
			std::cout << _twodArray[i][j].type;
		}
		std::cout << "" << std::endl;

	}

}

void Grid::loadingBmpPicture(char* filename)
{
	//2
	//		_heightLength = 10;
	//		_widthLength = 15;
	//		buildgridarray();
	//		_twodArray[9][12].xz = glm::vec2(1,2);
	//f�rsta �r heightlength andra �r widthlenght



	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

//	std::cout<< std::endl;
//	std::cout<< "  Name: " << filename << std::endl;
//	std::cout << " Width: " << width << std::endl;
//	std::cout << "Height: " << height << std::endl;
	_heightLength = height;
	_widthLength = width;


	buildgridarray();
	//		_twodArray[9][12].xz = glm::vec2(1,2);
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
			}
			else if (data[j] == 255 && data[j + 1] == 255 && data[j + 2] == 0)
			{
				this->_lootLocations.push_back(glm::vec3(j, 0.f, i));
				_twodArray[height - 1 - i][realj].type = loot;
			}
			else
			{
				std::cout << "error" << std::endl;
				std::cout<<i<<","<<realj<<std::endl;
			}
			realj++;
			//	cout << _twodArray[i][j].type;
		}
		//	cout << "" << endl;
	}
	delete[] data;
	fclose(f);
//	print2darraydata();
}

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
	int currentX = (int)glm::floor(position->x / GRIDSPACE);
	int currentZ = (int)glm::floor(position->z / GRIDSPACE);

	//std::cout << position->x << ", " << position->z << std::endl;

	/*glm::vec3 playerToCorner = glm::vec3((float)currentX - position->x, 0.f, (float)currentZ - position->z);
	float len = playerToCorner.x * playerToCorner.x + playerToCorner.y * playerToCorner.y + playerToCorner.z * playerToCorner.z;
	if (len > 0.3f * 0.3f)
	{
	glm::vec3 dist = playerToCorner;
	dist.x /= len;
	dist.y /= len;
	dist.z /= len;
	position->x = (float)currentX + dist.x * 0.15;
	position->z = (float)currentZ + dist.z * 0.15;
	}*/
	if (currentX <= 0 || currentZ <= 0 || currentX >= _widthLength || currentZ >= _heightLength)
	{
		position->x += velocity.x;
		position->z += velocity.y;
		return;
	}
	if (signbit(velocity.x) == false)// -->
	{
		if (_twodArray[currentZ][currentX + 1].type != wall)
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
		if (_twodArray[currentZ][currentX - 1].type != wall)
		{
			position->x += velocity.x;
		}
		else if (position->x - currentX > 0.3f)
		{
			position->x += velocity.x;
		}
	}

	if (signbit(velocity.y) == false)
	{
		if (_twodArray[currentZ + 1][currentX].type != wall)
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
		if (_twodArray[currentZ - 1][currentX].type != wall)
		{
			position->z += velocity.y;
		}
		else if (position->z - currentZ > 0.3f)
		{
			position->z += velocity.y;
		}
	}
}

bool Grid::checkifPlayerWon(glm::vec3 * playerpos,bool buttonpressed)
{
	_gotTheTreasure = true;
	glm::vec2 fixedPlayerPos;
	_exit.x = glm::floor(_exit.x / GRIDSPACE);
	_exit.y = glm::floor(_exit.y / GRIDSPACE);
	fixedPlayerPos.x = glm::floor(playerpos->z / GRIDSPACE);
	fixedPlayerPos.y = glm::floor(playerpos->x / GRIDSPACE);
	if (fixedPlayerPos == _exit && _gotTheTreasure == true)
	{
		return victory.victory(buttonpressed);
	}
	return false;
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

void Grid::Creategetheightandwidthpoint12(glm::vec3 guardposition)
{
	//0 = x1,1=x2,2=y1,3=y2

	int i = (int)guardposition.x;//height
	int j = (int)guardposition.z; //width
	//first wall upwards
	for (i;i > -1;i--)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[0] = glm::vec3(i + 1, 0, j);
			break;
		}
	}
	i = (int)guardposition.x;
	j = (int)guardposition.z;

	//second point downwards
	for (i;i < _heightLength;i++)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[1] = glm::vec3(i - 1, 0, j );
			break;
		}
	}
	i = (int)guardposition.x;
	j = (int)guardposition.z;
	//thirdwall left
	for (j;j > -1;j--)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[2] = glm::vec3(i, 0, j + 1);
			break;
		}
	}
	i = (int)guardposition.x;
	j = (int)guardposition.z;

	//sfourthwall right
	for (j;j < _widthLength;j++)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[3] = glm::vec3(i , 0, j - 1);
			break;
		}
	}

/*	for (i; i < _heightLength; i++)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[0] = glm::vec3(_twodArray[j][i].xz.x, 0, _twodArray[j][i].xz.y - 1);
			break;
		}
	}

	i = guardposition.x;
	j = guardposition.z;
	for (j ; j > -1; j--)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[1] = glm::vec3(_twodArray[j][i].xz.x, 0, _twodArray[j][i].xz.y + 1);
			break;
		}
	}


	 i = guardposition.x;
	 j = guardposition.z;
	for (i; i < _widthLength; i++)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[2] = glm::vec3(_twodArray[j][i].xz.x - 1, 0, _twodArray[j][i].xz.y );
			break;
		}
	}
	i = guardposition.x;
	j = guardposition.z;
	for (i; i > -1; i--)
	{
		if (_twodArray[j][i].type == wall)
		{
			pointxy[3] = glm::vec3(_twodArray[j][i].xz.x + 1 , 0, _twodArray[j][i].xz.y);
			break;
		}
	}


		//{
	/*	if (_twodArray[i][j].type == wall)
		{
			pointxy[0] = glm::vec3(_twodArray[i][j].xz.y,0,_twodArray[i][j].xz.x);
			break;
		}
	}
	i = guardposition.x;
	 j = guardposition.z;
	for (i; i > -1; i--)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[1] = glm::vec3(_twodArray[i][j].xz.y +1, 0, _twodArray[i][j].xz.x);
			break;
		}
	}
	 i = guardposition.x;
	 j = guardposition.z;
	for (j; j < _heightLength; j++)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[2] = glm::vec3(_twodArray[i][j].xz.y, 0, _twodArray[i][j].xz.x);
			break;
		}
	}
	i = guardposition.x;
	j = guardposition.z;
	for (j; j > -1; j--)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[3] = glm::vec3(_twodArray[i][j].xz.y, 0, _twodArray[i][j].xz.x + 1);
			break;
		}
	}*/


}

gridType Grid::returnGridType(int width, int height)
{
	return _twodArray[width][height].type;
}
