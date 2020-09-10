#pragma once
#ifndef _DATALOADER_
#define _DATALOADER_
#include<iostream>
#include <string>
#include<vector>
#include<fstream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "VertexData.h"
#include"FaceData.h"

class TextureImportor
{
public:
	
	//Constructor
	TextureImportor();

	//Destructor
	~TextureImportor();

	//Load ppm texture
	bool LoadTexture(std::vector<unsigned char>& values, int & width, int & height, int & max_col_val, std::string path);
};

#endif