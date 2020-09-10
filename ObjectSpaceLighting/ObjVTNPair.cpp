#include "ObjVTNPair.h"


//Constructor
ObjVTNPair::ObjVTNPair()
{
}

//Constructor
ObjVTNPair::ObjVTNPair(int vertexIndex, int texCoordIndex, int normalIndex)
{
	vertexIndex = this->vertexIndex;
	texCoordIndex = this->texCoordIndex;
	normalIndex = this->normalIndex;
}

//Destructor
ObjVTNPair::~ObjVTNPair()
{
}
