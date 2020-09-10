#pragma once
#ifndef _TEXTUREREPO_
#define _TEXTUREREPO_
#include"Texture.h"
#include"TextureImportor.h"
#include<string>
#include<vector>
class TextureRepository
{
public:

	//Constainer to store textures
	std::vector<Texture> Textures;

	//Ref to loader
	TextureImportor loader;
	
	//Constructor
	TextureRepository();

	//Function to add texture to repo
	void AddTextureToRepo(Texture texture);

	//Function to load texture
	void LoadTexture(std::string texturePath);

	//Destructor
	~TextureRepository();
};
#endif
