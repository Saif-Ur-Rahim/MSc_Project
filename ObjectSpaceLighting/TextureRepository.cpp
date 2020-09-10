#include "TextureRepository.h"


//Constructor
TextureRepository::TextureRepository()
{
	loader = TextureImportor();
	Textures.reserve(100);
}

//Function to add texture to repo
void TextureRepository::AddTextureToRepo(Texture texture)
{
	Textures.push_back(texture);
}

//Function to load texture
void TextureRepository::LoadTexture(std::string texturePath)
{
	Texture texture;
	bool textureStatus = false;
	textureStatus = loader.LoadTexture(texture.textureData, texture.textureWidth, texture.textureHeight, texture.textureMaxColumnValue, texturePath);
	if (textureStatus)
	{
		texture.name = texturePath;
		texture.textureID = Textures.size();
		AddTextureToRepo(texture);
	}
	else
	{
		std::cout << "Error at line 26 Texture Repositry" << std::endl;
		exit(0);
	}
}

//Destructor
TextureRepository::~TextureRepository()
{
}
