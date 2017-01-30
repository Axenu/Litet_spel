#include "GridDataStructure.h"
#pragma warning(disable:4996)

Grid::Grid()
{
	 loadingBmpPicture("roomtest.bmp");	
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



void Grid::print2darraydata()
{
	for (int i = 0; i < _heightLength; i++)
	{
		for (int j = 0; j < _widthLength; j++)
		{	
	//		std::cout << _twodArray[i][j].color.x << " ";
			std::cout << _twodArray[i][j].enumet << " ";
		}
		std::cout << "" << std::endl;
	}

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
				_twodArray[i][j].enumet = wall;
			}
			if (colorarray[i][j] == glm::vec3(0, 0, 0))
			{
				_twodArray[i][j].enumet = nothing;
			}
			if (colorarray[i][j] == glm::vec3(255, 0, 0))
			{
				_twodArray[i][j].enumet = exiting;
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

void Grid::generateMesh(std::vector<glm::vec3> *position, std::vector<glm::vec3> *normal)
{
	for (int j = 0; j < _heightLength; j++)
	{
		for (int i = 0; i < _widthLength; i++)
		{
			if (_twodArray[i][j].enumet != wall)
			{
				if (j != 0)
				{
					if (_twodArray[i][j - 1].enumet == wall)
					{
						// Positions
						position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , j * GRIDSPACE));
						// Normals
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
						normal->push_back(glm::vec3(0.f, 0.f, -1.f));
					}
				}
				if (j != _heightLength - 1)
				{
					if (_twodArray[i][j + 1].enumet == wall)
					{
						// Positions
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						// Normals
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
					}
				}
				if (i != 0)
				{
					if (_twodArray[i - 1][j].enumet == wall)
					{
						// Positions
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						// Normals
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
						normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
					}
				}
				if (i != _widthLength - 1)
				{
					if (_twodArray[i + 1][j].enumet == wall)
					{
						// Positions
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
						position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
						// Normals
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
						normal->push_back(glm::vec3(0.f, 0.f, 1.f));
					}
				}
			}
			//if (!(_twodArray[i][j].enumet == wall && _twodArray[i][j].enumet == wall && _twodArray[i][j].enumet == wall && _twodArray[i][j].enumet == wall))
			//{
			//	if (_twodArray[i][j].enumet == wall && _twodArray[i + 1][j].enumet == wall && _twodArray[i + 1][j + 1].enumet != wall && _twodArray[i][j + 1].enumet != wall)
			//	{
			//		// Positions
			//		position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
			//		position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
			//		position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , j * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, j * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , j * GRIDSPACE));
			//		//Normals
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, -1.f));
			//	}
			//	if (_twodArray[i][j].enumet == wall && _twodArray[i][j + 1].enumet == wall)
			//	{
			//		// Positions
			//		position->push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
			//		position->push_back(glm::vec3(i * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3(i * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3(i * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
			//		position->push_back(glm::vec3(i * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
			//		//Normals
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(1.f, 0.f, 0.f));
			//	}
			//	if (_twodArray[i][j + 1].enumet == wall && _twodArray[i + 1][j + 1].enumet == wall)
			//	{
			//		// Positions
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3( i      * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3( i      * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		//Normals
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//		normal->push_back(glm::vec3(0.f, 0.f, 1.f));
			//	}
			//	if (_twodArray[i + 1][j].enumet == wall && _twodArray[i + 1][j + 1].enumet == wall)
			//	{
			//		// Positions
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT,  j      * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       ,  j      * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, ROOFHEIGHT, (j + 1) * GRIDSPACE));
			//		position->push_back(glm::vec3((i + 1) * GRIDSPACE, 0.f       , (j + 1) * GRIDSPACE));
			//		//Normals
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//		normal->push_back(glm::vec3(-1.f, 0.f, 0.f));
			//	}
			//}
		}
	}
	return;
}
