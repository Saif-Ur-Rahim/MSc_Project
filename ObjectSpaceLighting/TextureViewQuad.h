#pragma once
#ifndef _TEXTUREVIEWQUAD_
#define _TEXTUREVIEWQUAD_
#include<vector>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "Vector3.h"
//#include "Matrix4.h"
#include "Shader.h"
class TextureViewQuad
{
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Shader shader;
	//Vector3 worldPosition = Vector3(0.7f,-0.7f,0.0f);
	glm::vec3 worldPosition = glm::vec3(0.7f,-0.7f,0.0f);
	/*Matrix4 model;
	Matrix4 scale;*/
	glm::mat4 model;
	glm::mat4 scale;

	std::vector<float> vertices = {
		 0.2f,  0.4f, 0.0f, 1.0f, 1.0f,  // top right
		 0.2f, -0.2f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.2f,  0.4f, 0.0f, 0.0f, 1.0f,
		 0.2f, -0.2f, 0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f, 0.0f, 0.0f, 0.0f,
		-0.2f,  0.4f, 0.0f, 0.0f, 1.0f, };


	TextureViewQuad();
	void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition, unsigned int &renderTexture);
	//Create the object 
	void Create();
	~TextureViewQuad();
};
#endif
