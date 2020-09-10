#include "TextureImportor.h"

//Constructor
TextureImportor::TextureImportor()
{
}

//Destructor
TextureImportor::~TextureImportor()
{
}

//Load ppm texture
bool TextureImportor::LoadTexture(std::vector<unsigned char>& values, int & width, int & height, int & max_col_val, std::string path)
{
	std::ifstream inFile;
	//open file 
	inFile.open(path);
	// check if opened
	if (!inFile)
	{
		std::cout << "Unable to the file" << std::endl;
		return false;
	}
	else
	{
		int triple = 0;
		// simple string to read the lines
		std::string str;

		int total = 0;

		while (std::getline(inFile, str)) {
			// total is 2 we are at the line that has the rows and columns values
			if (total == 1)
			{
				// find the position of empty 
				int pos = str.find(' ');
				//get the left side sub string
				std::string sub_1 = str.substr(0, pos);
				//get the right side sub string
				std::string sub_2 = str.substr(pos + 1);
				// cast back to int and save in rows
				width = std::stoi(sub_1);
				// cast back to int and save in columns
				height = std::stoi(sub_2);
			}
			else if (total > 2) // if the total is more than 3 then we are reading the rgb values
			{
				values.push_back((unsigned char)std::stoi(str));
			}
			//increment the total
			total++;
		}
	}
	return true;
}