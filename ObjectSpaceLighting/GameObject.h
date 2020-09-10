#pragma once
#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_
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
#include "Light.h"
#include"ComputeShaderRP2.h"
//Main class to represent the sphere
class GameObject
{
public:
	
	//Flag to check if object is created
	bool  created = false;

	//Name of Object
	std::string name;

	//Buffers for opengl
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	//Texture Properties
	bool textureAttached = false;
	Texture *textureData;

	//Scale
	float scale = 1.0f;

	//Render mode 
	GLenum renderMode;

	//Shader Objects
	Shader RenderPass3_Shader;
	Shader RenderPass1_Shader;
	
	//WorldPosition
	glm::vec3 worldPosition;
	
	//Translator
	glm::mat4 translator;
	
	//Model Matrix
	glm::mat4  model;

	//Compute Shader Object
	ComputeShaderRP2 ComputeShaderRP2;
	
	//Texture Map Buffers
	GLuint RP1_FrameBuffer = 0;
	GLuint RP1_DepthBuffer;
	GLuint RP1_TextureMap;

	//OBJ File data containers
	std::vector<ObjVTNPair> objPairs;
	std::vector<unsigned int> indices;
	std::vector<VertexData> uniqueVertices;
	std::vector<glm::vec2> uniqueTextCoords;
	std::vector<glm::vec3> uniqueNormals;
	
	//Pre-Processed Data
	std::vector<float> computePipelineVertexData;

	//Constructor
	GameObject();

	//Main function to run render pass 1
	void RenderPass_1(glm::mat4  view, glm::mat4  projection, glm::vec3 cameraPosition, float lightCount, std::vector<std::shared_ptr<Light>> &Lights);

	//Main function to run render pass 3
	void RenderPass_3(glm::mat4  view, glm::mat4  projection, glm::vec3 cameraPosition,float lightCount, std::vector<std::shared_ptr<Light>> &Lights);
	
	//Create the object 
	void CreateObject(glm::vec3 worldPosition, float scale, GLenum renderMode, std::string objPath, std::string texturePath);
	
	//Creates the FrameBuffer
	void CreateFrameBuffer(float width, float height, float texWidth, float texHeight);

	//Attach the texture file
	void AttachTexture(Texture &texturePassed);

	//Setup and bind first render pass data
	void BindRenderPass_1();

	//Setup and bind second render pass data
	void BindRenderPass_3();

	//Destructor
	~GameObject();
};
#endif
