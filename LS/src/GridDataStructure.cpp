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
			std::cout << _twodArray[i][j].color.x << " ";
		}
		std::cout << "" << std::endl;
	}
	for (int i = 0; i < _heightLength; i++)
	{
		for (int j = 0; j < _widthLength; j++)
		{
			std::cout << _twodArray[i][j].color.y << " ";
		}
		std::cout << "" << std::endl;
	}
}

void Grid::loadingBmpPicture(char* filename)
{
	std::ofstream myfile;
	myfile.open("y.txt");
	FILE* f = fopen(filename, "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, f);

	int width =  *(int*)&header[18];
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
//	unsigned char* tmparraydata = datan;
//	int j = 1;
//	int l = 0;
//	for (int i = 0;i < size;i++)
//	{		

//		if (j > width)
//		{
//			j = 1;
//			l++;
//		}
//		datan[width*j-l]=tmparraydata[size - i];
//		j++;
//	}
//	delete[] tmparraydata;
	_heightLength = height;
	_widthLength = width;

	buildgridarray();
	//filling the twodarray with number1;

	int k = 0;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			_twodArray[i][j].xz.x = (float)i;
			_twodArray[i][j].xz.y = (float)j;
			_twodArray[i][j].color.x = datan[k    ];
			_twodArray[i][j].color.y = datan[k + 1];
			_twodArray[i][j].color.z = datan[k + 2];
			k += 3;
		}
	}
	delete[] datan;
	datan = 0;
}

std::vector<glm::vec3> Grid::generateMesh()
{
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < _heightLength - 1; i++)
	{
		for (int j = 0; j < _widthLength - 1; j++)
		{
			if (_twodArray[i][j].enumet == wall && _twodArray[i + 1][j].enumet == wall)
			{
				if (_twodArray[i][j + 1].enumet == wall && _twodArray[i + 1][j + 1].enumet == wall)
				{																													
					//Front quad																								
					vertices.push_back(glm::vec3(_twodArray[i    ][j + 1].xz.x, ROOFHEIGHT, _twodArray[i    ][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, ROOFHEIGHT, _twodArray[i + 1][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i    ][j + 1].xz.x, 0.f       , _twodArray[i    ][j + 1].xz.y));
																																  
					vertices.push_back(glm::vec3(_twodArray[i    ][j + 1].xz.x, 0.f       , _twodArray[i    ][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, ROOFHEIGHT, _twodArray[i + 1][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, 0.f       , _twodArray[i + 1][j + 1].xz.y));
																																	
					//Left quad																										
					vertices.push_back(glm::vec3(_twodArray[i][j    ].xz.x, ROOFHEIGHT, _twodArray[i][j    ].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i][j + 1].xz.x, ROOFHEIGHT, _twodArray[i][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i][j    ].xz.x, 0.f       , _twodArray[i][j    ].xz.y));
																															
					vertices.push_back(glm::vec3(_twodArray[i][j    ].xz.x, 0.f       , _twodArray[i][j    ].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i][j + 1].xz.x, ROOFHEIGHT, _twodArray[i][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i][j + 1].xz.x, 0.f       , _twodArray[i][j + 1].xz.y));
																																
					//Back quad																						
					vertices.push_back(glm::vec3(_twodArray[i + 1][j].xz.x, ROOFHEIGHT, _twodArray[i + 1][j].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i    ][j].xz.x, ROOFHEIGHT, _twodArray[i    ][j].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j].xz.x, 0.f       , _twodArray[i + 1][j].xz.y));
																															
					vertices.push_back(glm::vec3(_twodArray[i + 1][j].xz.x, 0.f       , _twodArray[i + 1][j].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i    ][j].xz.x, ROOFHEIGHT, _twodArray[i    ][j].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j].xz.x, 0.f       , _twodArray[i + 1][j].xz.y));
																																	
					//Right quad																									
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, ROOFHEIGHT, _twodArray[i + 1][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j    ].xz.x, ROOFHEIGHT, _twodArray[i + 1][j    ].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, 0.f       , _twodArray[i + 1][j + 1].xz.y));
																																
					vertices.push_back(glm::vec3(_twodArray[i + 1][j + 1].xz.x, 0.f       , _twodArray[i + 1][j + 1].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j    ].xz.x, ROOFHEIGHT, _twodArray[i + 1][j    ].xz.y));
					vertices.push_back(glm::vec3(_twodArray[i + 1][j    ].xz.x, 0.f       , _twodArray[i + 1][j    ].xz.y));
				}
			}
		}
	}
	return vertices;
}
