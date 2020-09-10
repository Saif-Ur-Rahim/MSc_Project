#pragma once
class FaceData
{
//Simple class that show the base face data 
public:

	//Array to store vertex data
	int vertexIndex[4];

	//Array to store uv data
	int textureCoordIndex[4];

	//Array to store normal data
	int normalIndex[4];

	//Constructor
	FaceData();

	//Destructor
	~FaceData();
};

