#include "GridDataStructure.h"

Grid::Grid()
{
	 heightLength = 10;
	 widthLength = 10;
	 buildgridarray();
}

Grid::~Grid()
{
	for (int i = 0; i < heightLength; i++)
	delete[] twodArray[i];
	delete[] twodArray;
}
void Grid::buildgridarray()
{
	//building the twodarray
	twodArray = new gridValues*[heightLength];
	for (int i = 0; i < heightLength;i++)
	{
		twodArray[i] = new gridValues[widthLength];
	}
	
	//filling the twodarray with number1;
	for (int i = 0; i < heightLength; i++)
	{
		for (int j = 0; j < widthLength; j++)
		{
			twodArray[i][j].xyz = glm::vec3(1,2,3);
			twodArray[i][j].enumet = wall;
		}
	}


}
void Grid::print2darraydata()
{
	for (int i = 0;i < heightLength;i++)
	{
		for (int j = 0;j < widthLength;j++)
		{
			cout << twodArray[i][j].xyz.x << "," << twodArray[i][j].xyz.y << "," << twodArray[i][j].xyz.z << " " << twodArray[i][j].enumet << " "<< j<<" ";
		}
		cout << "" << endl;
	}
}