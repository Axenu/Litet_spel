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

vector<glm::vec3> Grid::generateMesh()
{
	vector<glm::vec3> vertices;
	for (int i = 0; i < heightLength - 1; i++)
	{
		for (int j = 0; j < widthLength - 1; j++)
		{
			if (twodArray[i][j].enumet == wall && twodArray[i + 1][j].enumet == wall)												
			{
				if (twodArray[i][j + 1].enumet == wall && twodArray[i + 1][j + 1].enumet == wall)
				{																													//
					//Front quad																									//
					vertices.push_back(glm::vec3(twodArray[i    ][j + 1].xyz.x, ROOFHEIGHT, twodArray[i    ][j + 1].xyz.z));		//   ____
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, ROOFHEIGHT, twodArray[i + 1][j + 1].xyz.z));        //   |   | 
					vertices.push_back(glm::vec3(twodArray[i    ][j + 1].xyz.x, 0.f       , twodArray[i    ][j + 1].xyz.z));        //   |   |
																																    //   |___|
					vertices.push_back(glm::vec3(twodArray[i    ][j + 1].xyz.x, 0.f       , twodArray[i    ][j + 1].xyz.z));		//     ^
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, ROOFHEIGHT, twodArray[i + 1][j + 1].xyz.z));		//     |
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, 0.f       , twodArray[i + 1][j + 1].xyz.z));
																																	//
					//Left quad																										//
					vertices.push_back(glm::vec3(twodArray[i][j    ].xyz.x, ROOFHEIGHT, twodArray[i][j    ].xyz.z));				//   ____
					vertices.push_back(glm::vec3(twodArray[i][j + 1].xyz.x, ROOFHEIGHT, twodArray[i][j + 1].xyz.z));                //   |   |
					vertices.push_back(glm::vec3(twodArray[i][j    ].xyz.x, 0.f       , twodArray[i][j    ].xyz.z));                // ->|   |
																																	//   |___|
					vertices.push_back(glm::vec3(twodArray[i][j    ].xyz.x, 0.f       , twodArray[i][j    ].xyz.z));                // 
					vertices.push_back(glm::vec3(twodArray[i][j + 1].xyz.x, ROOFHEIGHT, twodArray[i][j + 1].xyz.z));                //
					vertices.push_back(glm::vec3(twodArray[i][j + 1].xyz.x, 0.f       , twodArray[i][j + 1].xyz.z));
																																	//     |
					//Back quad																										//     v
					vertices.push_back(glm::vec3(twodArray[i + 1][j].xyz.x, ROOFHEIGHT, twodArray[i + 1][j].xyz.z));				//   ____
					vertices.push_back(glm::vec3(twodArray[i    ][j].xyz.x, ROOFHEIGHT, twodArray[i    ][j].xyz.z));                //   |   |
					vertices.push_back(glm::vec3(twodArray[i + 1][j].xyz.x, 0.f       , twodArray[i + 1][j].xyz.z));                //   |   |
																																	//   |___|
					vertices.push_back(glm::vec3(twodArray[i + 1][j].xyz.x, 0.f       , twodArray[i + 1][j].xyz.z));                //
					vertices.push_back(glm::vec3(twodArray[i    ][j].xyz.x, ROOFHEIGHT, twodArray[i    ][j].xyz.z));                //
					vertices.push_back(glm::vec3(twodArray[i + 1][j].xyz.x, 0.f       , twodArray[i + 1][j].xyz.z));
																																	//
					//Right quad																									//
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, ROOFHEIGHT, twodArray[i + 1][j + 1].xyz.z));		//   ____
					vertices.push_back(glm::vec3(twodArray[i + 1][j    ].xyz.x, ROOFHEIGHT, twodArray[i + 1][j    ].xyz.z));		//   |   |
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, 0.f       , twodArray[i + 1][j + 1].xyz.z));        //   |   |<-
																																	//   |___|
					vertices.push_back(glm::vec3(twodArray[i + 1][j + 1].xyz.x, 0.f       , twodArray[i + 1][j + 1].xyz.z));        //
					vertices.push_back(glm::vec3(twodArray[i + 1][j    ].xyz.x, ROOFHEIGHT, twodArray[i + 1][j    ].xyz.z));		//
					vertices.push_back(glm::vec3(twodArray[i + 1][j    ].xyz.x, 0.f       , twodArray[i + 1][j    ].xyz.z));
				}
			}
		}
	}
	return vertices;
}
