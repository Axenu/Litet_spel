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

std::vector<glm::vec3> Grid::generateMesh()
{
	std::vector<glm::vec3> vertices;
	// For each position in the 2D grid. Check if the point bellow is a wall, if so, make a wall between these points.
	// Then check if the point next to the current point is a wall, if so, create a wall between these points.

	//There is some special cases when we reach different edges on the 2D grid, based on which edge we're at we need to avoid checking outside of the array.
	for (int i = 0; i < _heightLength - 1; i++)
	{
		for (int j = 0; j < _widthLength - 1; j++)
		{
			if (_twodArray[i][j].enumet == wall)
			{
				if (i != _heightLength || j != _widthLength)
				{
					if (_twodArray[i + 1][j].enumet == wall)
					{
						vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
						vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
						vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));

						vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));
						vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
						vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, 0.f, i * GRIDSPACE));
					}
					if (_twodArray[i][j + 1].enumet == wall)
					{
						vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
						vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, (i + 1) * GRIDSPACE));
						vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));

						vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));
						vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, (i + 1) * GRIDSPACE));
						vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, (i + 1) * GRIDSPACE));
					}
				}
				//Special cases
				if (i == _heightLength - 1 && j != _widthLength - 1) 
				{
					vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
					vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, (i + 1) * GRIDSPACE));
					vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));

					vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));
					vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, (i + 1) * GRIDSPACE));
					vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, (i + 1) * GRIDSPACE));
				}
				if (j == _widthLength - 1 && i != _heightLength - 1)
				{
					vertices.push_back(glm::vec3(j * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
					vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
					vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));

					vertices.push_back(glm::vec3(j * GRIDSPACE, 0.f, i * GRIDSPACE));
					vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, ROOFHEIGHT, i * GRIDSPACE));
					vertices.push_back(glm::vec3((j + 1) * GRIDSPACE, 0.f, i * GRIDSPACE));
				}
			}
		}
	}
	return vertices;
}
