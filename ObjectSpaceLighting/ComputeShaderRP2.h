#pragma once
#ifndef _COMPUTETEST_
#define _COMPUTETEST_
#include<iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Vector3.h"
#include<GLFW/glfw3.h>
#include"DebugFunctions.h"
#include"Shader.h"
#include"ResolutionProperties.h" 
class ComputeShaderRP2
{
public:

	//Output buffer
	GLuint tex_output;

	//Shader object
	Shader shaderCompute;

	//Debug bool
	bool printShade = false;

	//Original Texture Storage
	std::vector<float> OriginalTextureData;

	//Handler for compute shader result
	unsigned char *ComputeResult;

	//Contructor
	ComputeShaderRP2();
    
	//Function to Initially Setup the Compute Shader Data
	void ComputeShaderTextureDataSetup(std::vector<unsigned char>&textureData, std::vector<float>&vertexData, float textureWidth, float textureHeight);
	
	//Debug Function to store data to file
	void WriteDataToFile(float textureWidth, float textureHeight);
	
	//Function to Launch the Compute Shader
	void LaunchComputeShader(float *dataBuffer, std::vector<float>&vertexData, std::vector<unsigned char>&textureData, glm::vec3 cameraPosition, glm::mat4 modelMatrix,float textureWidth, float textureHeight);
	
	//Destructor
	~ComputeShaderRP2();
};
#endif
