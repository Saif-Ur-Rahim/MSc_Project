#pragma once
#ifndef _OBJVTNPAIR_
#define _OBJVTNPAIR_

//Custom class to sotre obj data
class ObjVTNPair
{
public:

	//obj data indices
	int vertexIndex;
	int texCoordIndex;
	int normalIndex;

	//Constructor
	ObjVTNPair();
	ObjVTNPair(int vertexIndex, int texCoordIndex, int normalIndex);

	//Destructor
	~ObjVTNPair();
};

#endif
