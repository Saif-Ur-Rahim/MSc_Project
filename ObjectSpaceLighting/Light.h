#pragma once
#ifndef _LIGHT_
#define _LIGHT_
#include<iostream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <string>
#include<vector>
#include "Shader.h"
#include "TextureImportor.h"
#include "VertexData.h"
#include "FaceData.h"
#include "ObjVTNPair.h"
#include<GLFW/glfw3.h>
#include"Texture.h"
#include "Vector3.h"

class Light
{
public:

	//Flag to check if loaded or not
	bool loaded = false;

	//OpenGL Buffers
	unsigned int VAO;
	unsigned int VBO;

	//Shader Object
	Shader shader;

	//Transform properties
	glm::vec3 worldPosition;
	glm::mat4 model;

	//Vertex Data
	std::vector<float> vertices{
		-0.2f, -0.2f, -0.2f,  0.0f, 0.0f,
		 0.2f, -0.2f, -0.2f,  1.0f, 0.0f,
		 0.2f,  0.2f, -0.2f,  1.0f, 1.0f,
		 0.2f,  0.2f, -0.2f,  1.0f, 1.0f,
		-0.2f,  0.2f, -0.2f,  0.0f, 1.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 0.0f,

		-0.2f, -0.2f,  0.2f,  0.0f, 0.0f,
		 0.2f, -0.2f,  0.2f,  1.0f, 0.0f,
		 0.2f,  0.2f,  0.2f,  1.0f, 1.0f,
		 0.2f,  0.2f,  0.2f,  1.0f, 1.0f,
		-0.2f,  0.2f,  0.2f,  0.0f, 1.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, 0.0f,

		-0.2f,  0.2f,  0.2f,  1.0f, 0.0f,
		-0.2f,  0.2f, -0.2f,  1.0f, 1.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, 0.0f,
		-0.2f,  0.2f,  0.2f,  1.0f, 0.0f,

		 0.2f,  0.2f,  0.2f,  1.0f, 0.0f,
		 0.2f,  0.2f, -0.2f,  1.0f, 1.0f,
		 0.2f, -0.2f, -0.2f,  0.0f, 1.0f,
		 0.2f, -0.2f, -0.2f,  0.0f, 1.0f,
		 0.2f, -0.2f,  0.2f,  0.0f, 0.0f,
		 0.2f,  0.2f,  0.2f,  1.0f, 0.0f,

		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f,
		 0.2f, -0.2f, -0.2f,  1.0f, 1.0f,
		 0.2f, -0.2f,  0.2f,  1.0f, 0.0f,
		 0.2f, -0.2f,  0.2f,  1.0f, 0.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, 0.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f,

		-0.2f,  0.2f, -0.2f,  0.0f, 1.0f,
		 0.2f,  0.2f, -0.2f,  1.0f, 1.0f,
		 0.2f,  0.2f,  0.2f,  1.0f, 0.0f,
		 0.2f,  0.2f,  0.2f,  1.0f, 0.0f,
		-0.2f,  0.2f,  0.2f,  0.0f, 0.0f,
		-0.2f,  0.2f, -0.2f,  0.0f, 1.0f
	};
	
	//Constructor
	Light();
	
	//Main function to render the object
	void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
	
	//Create the object 
	void CreateLight(Vector3 position);

	//Destructor
	~Light();
};
#endif
