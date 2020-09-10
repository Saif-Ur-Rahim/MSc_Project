#pragma once
#ifndef _DEBUG_
#define _DEBUG_
#include<fstream>
#include<iostream>
#include<string>
class DebugFunctions
{
public:

	//Constructor
	DebugFunctions();

	//Function write buffer to text file
	static void WriteBufferToTextFile(float *Buff, int size, std::string path);

	//Function write frame buffer to ppm
	static void WriteFrameBufferToPPM(unsigned char *Buff, int size, std::string path,float width, float height);

	//Function write frame buffer to ppm and takes int buffer
	static void WriteFrameBufferToPPM_INT(unsigned int * Buff, int size, std::string path);
	
	//Destructor
	~DebugFunctions();
};
#endif
