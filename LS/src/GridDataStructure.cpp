#include "GridDataStructure.h"
#pragma warning(disable:4996)
Grid::Grid()
{
	 loadingBmpPicture("roomtest.bmp");
	
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
	

}



void Grid::print2darraydata()
{
	for (int i = 0;i < heightLength;i++)
	{
		for (int j = 0;j < widthLength;j++)
		{
			
			
			cout<< twodArray[i][j].color.x <<  " ";
			
		}
		cout << "" << endl;
	}
	for (int i = 0;i < heightLength;i++)
	{
		for (int j = 0;j < widthLength;j++)
		{


			cout << twodArray[i][j].color.y << " ";

		}
		cout << "" << endl;
	}

}

void Grid::loadingBmpPicture(char* filename)
{
	std::ofstream myfile;
	myfile.open("y.txt");
	FILE* f = fopen(filename, "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, f);
	int index;
	unsigned char heeight;

	int width =  *(int*)&header[18];
	int height = *(int*)&header[22];
	int size = 3 * width*height;

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
	heightLength = height;
	widthLength = width;

	buildgridarray();
	//filling the twodarray with number1;

	int k = 0;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			twodArray[i][j].xz.x = i;
			twodArray[i][j].xz.y = j;
			twodArray[i][j].color.x = datan[k];
			twodArray[i][j].color.y = datan[k+1];
			twodArray[i][j].color.z = datan[k+2];
			k += 3;
		}
	}
	delete[] datan;
	datan = 0;
}
