#include "GridDataStructure.h"
#pragma warning(disable:4996)

Grid::Grid()
{
	_gotTheTreasure = false;

	loadingBmpPicture("roomtest.bmp");

	_exit = getData(exiting).xz;

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

gridValues Grid::getData(gridType Data)
{
	for (int i = 0; i < _heightLength; i++)
	{
		for (int j = 0; j < _widthLength; j++)
		{
			if (Data == exiting)
			{
				if (_twodArray[i][j].type == exiting)
				{
					return _twodArray[i][j];
				}
			}
			if (Data == guard)
			{
				if (_twodArray[i][j].type == guard)
				{
					return _twodArray[i][j];
				}
			}
		}

	}
}

void Grid::print2darraydata()
{
	for (int i = 0; i < _heightLength; i++)
	{
		for (int j = 0; j < _widthLength; j++)
		{
	//		std::cout << _twodArray[i][j].color.x << " ";
			std::cout << _twodArray[i][j].type << " ";
		}
		std::cout << "" << std::endl;
	}
//	std::cout << _twodArray[6][5].type << std::endl;
}

void Grid::loadingBmpPicture(char* filename)
{

	FILE* f = fopen(filename, "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, f);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int size = 3 * width * height;

	unsigned char* datan = new unsigned char[size]; //Allocating 3bytes for each pixel. R G B
	fread(datan, 1, size, f);
	fclose(f);

	for (int i = 0; i < size; i += 3)
	{
		unsigned char tmp = datan[i];
		datan[i] = datan[i + 2];
		datan[i + 2] = tmp;
	}

	_heightLength = height;
	_widthLength = width;

	buildgridarray();
	//filling the twodarray with number1;

	glm::vec3** colorarray ;
	//building the 2D array
	colorarray = new glm::vec3*[_heightLength];
	for (int i = 0; i < _heightLength; i++)
	{
		colorarray[i] = new glm::vec3[_widthLength];
	}

	int k = 0;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			_twodArray[i][j].xz.x = (float)i;
			_twodArray[i][j].xz.y = (float)j;
			colorarray[height - 1 - j][i].x = datan[k];
			colorarray[height - 1 - j][i].y = datan[k + 1];
			colorarray[height - 1 - j][i].z = datan[k + 2];
			k += 3;
		}
	}
	//setting the enum for each vertex.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (colorarray[i][j] == glm::vec3(255, 255, 255))
			{
				_twodArray[i][j].type = wall;
			}
			if (colorarray[i][j] == glm::vec3(0, 0, 0))
			{
				_twodArray[i][j].type = nothing;
			}
			if (colorarray[i][j] == glm::vec3(255, 0, 0))
			{
				_twodArray[i][j].type = exiting;
			}
			if (colorarray[i][j] == glm::vec3(0, 255, 0))
			{
				_twodArray[i][j].type = guard;
			}

		}
	}
	for (int i = 0; i < _heightLength; i++)
	{
		delete[] colorarray[i];
	}

	delete[] colorarray;
		delete[] datan;
		datan = 0;

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
			if (_twodArray[i][j].type != wall)
			{
				if (j != 0)
				{
					if (_twodArray[i][j - 1].type == wall)
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
					if (_twodArray[i][j + 1].type == wall)
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
					if (_twodArray[i + 1][j].type == wall)
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
					if (_twodArray[i - 1][j].type == wall)
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

	if (signbit(velocity.x) == false)// -->
	{
		if (_twodArray[currentX + 1][currentZ].type != wall)
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
		if (_twodArray[currentX - 1][currentZ].type != wall)
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
		if (_twodArray[currentX][currentZ + 1].type != wall)
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
		if (_twodArray[currentX][currentZ - 1].type != wall)
		{
			position->z += velocity.y;
		}
		else if (position->z - currentZ > 0.3f)
		{
			position->z += velocity.y;
		}
	}
}

void Grid::checkifPlayerWon(glm::vec3 playerpos)
{
	glm::vec2 fixedPlayerPos;
	_exit.x = glm::floor(_exit.x / GRIDSPACE);
	_exit.y = glm::floor(_exit.y / GRIDSPACE);
	fixedPlayerPos.x = glm::floor(playerpos.x / GRIDSPACE);
	fixedPlayerPos.y = glm::floor(playerpos.y / GRIDSPACE);
	if (fixedPlayerPos == _exit && _gotTheTreasure == true)
	{
		std::cout << "you won Congratulations" << std::endl;
	}

}

glm::vec3 Grid::getxandypoint12(int i)
{
	return pointxy[i];
}

void Grid::Createxandypoint12(glm::vec3 guardposition)
{
	//0 = x1,1=x2,2=y1,3=y2

	int i = (int)guardposition.x;
	int j = (int)guardposition.z;
	//first wall upwards
	for (i;i > -1;i--)
	{
		if (_twodArray[i][j].type == wall)
		{
			pointxy[0] = glm::vec3(_twodArray[j][i].xz.x, 0, _twodArray[j][i].xz.y + 1);
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
			pointxy[1] = glm::vec3(_twodArray[j][i].xz.x, 0, _twodArray[j][i].xz.y - 1);
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
			pointxy[2] = glm::vec3(_twodArray[j][i].xz.x+1, 0, _twodArray[j][i].xz.y);
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
			pointxy[3] = glm::vec3(_twodArray[j][i].xz.x - 1, 0, _twodArray[j][i].xz.y);
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
