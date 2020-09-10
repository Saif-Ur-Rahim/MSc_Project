#pragma once
#ifndef _TEXTURE_
#define _TEXTURE_
#include<vector>
#include<string>
class Texture
{
public:

	//List to store texture data 
	std::vector<unsigned char> textureData;

	//Properties
	unsigned int textureBit;
	int textureWidth;
	int textureHeight;
	int textureMaxColumnValue;

	//Name and ID
	std::string name;
	int textureID;

	//Constructor
	Texture();

	//Destructor
	~Texture();
};
#endif
