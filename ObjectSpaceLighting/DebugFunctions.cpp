#include "DebugFunctions.h"


//Constructor
DebugFunctions::DebugFunctions()
{
}

//Function write buffer to text file
void DebugFunctions::WriteBufferToTextFile(float *Buff,int size, std::string path)
{
	std::ofstream WriteToFile;
	WriteToFile.open(path);
	for (int i = 0; i < size; i++)
	{

		WriteToFile << Buff[i];

		WriteToFile << "\n";
	}


	WriteToFile.close();
	std::cout << "Read Complete" << std::endl;

}

//Function write frame buffer to ppm
void DebugFunctions::WriteFrameBufferToPPM(unsigned char * Buff,int size, std::string path,float width, float height)
{
	std::ofstream WriteToFile;
	WriteToFile.open(path);
	WriteToFile << "P3\n";
	WriteToFile << width;
	WriteToFile << " ";
	WriteToFile << height;
	WriteToFile << "\n";
	WriteToFile << "255\n";
	for (int i = 0; i < size; i++)
	{

		WriteToFile << (int)Buff[i];

		WriteToFile << "\n";
	}


	WriteToFile.close();
	std::cout << "Read Complete" << std::endl;
}

//Function write frame buffer to ppm and takes int buffer
void DebugFunctions::WriteFrameBufferToPPM_INT(unsigned int * Buff, int size, std::string path)
{
	std::ofstream WriteToFile;
	WriteToFile.open(path);
	WriteToFile << "P3\n";
	WriteToFile << "# Test Image\n";
	WriteToFile << "512 512\n";
	WriteToFile << "255\n";
	for (int i = 0; i < size; i++)
	{

		WriteToFile << Buff[i];

		WriteToFile << "\n";
	}


	WriteToFile.close();
	std::cout << "Read Complete" << std::endl;
}

//Destructor
DebugFunctions::~DebugFunctions()
{
}
