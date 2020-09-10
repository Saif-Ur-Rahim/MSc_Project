#include "GameObject.h"


//Constructor 
GameObject::GameObject()
{
}

//Main function to run render pass 1
void GameObject::RenderPass_1(glm::mat4  view, glm::mat4  projection, glm::vec3 cameraPosition, float lightCount, std::vector<std::shared_ptr<Light>>& Lights)
{
	//Bind the data 
	BindRenderPass_1();

	//Enable texture 2D
	glEnable(GL_TEXTURE_2D);

	//Activate Shader
	RenderPass1_Shader.Activate();

	//Setup matrices
	translator = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	
	//translate to world position
	model =glm::translate(model, worldPosition);

	//Setup Uniform Buffers
	RenderPass1_Shader.SetVec3("directionalLightPos", 1.0f, 1.0f, 1.0f);
	RenderPass1_Shader.SetVec3("directionalLightColor", 1.2f, 1.0f, 2.0f);
	RenderPass1_Shader.SetVec3("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	RenderPass1_Shader.SetMat4("transform", translator);
	RenderPass1_Shader.SetMat4("view", view);
	RenderPass1_Shader.SetMat4("projection", projection);
	RenderPass1_Shader.SetMat4("model", model);
	RenderPass1_Shader.SetFloat("textureAttached", (float)textureAttached);

	if (textureAttached) {
		//Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureData->textureBit);
	}

	glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, uniqueVertices.size());
	glBindVertexArray(0);
}

//Main function to run render pass 3
void GameObject::RenderPass_3(glm::mat4  view, glm::mat4  projection, glm::vec3 cameraPosition, float lightCount, std::vector<std::shared_ptr<Light>> &Lights)
{
	//Bind the data 
	BindRenderPass_3();

	//Enable Texturing
	glEnable(GL_TEXTURE_2D);
	
	//Activate Shader
	RenderPass3_Shader.Activate();

	//Setup matrices
	translator = glm::mat4(1.0f);
	model = glm::mat4(1.0f);

	//translate to world position
    model = glm::translate(model, worldPosition);
	
	//Setup Uniform Buffers
	RenderPass3_Shader.SetVec3("directionalLightPos", 1.0f, 1.0f, 1.0f);
	RenderPass3_Shader.SetVec3("directionalLightColor", 1.2f, 1.0f, 2.0f);
	RenderPass3_Shader.SetVec3("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	RenderPass3_Shader.SetMat4("transform", translator);
	RenderPass3_Shader.SetMat4("view", view);
	RenderPass3_Shader.SetMat4("projection", projection);
	RenderPass3_Shader.SetMat4("model", model);
	RenderPass3_Shader.SetFloat("textureAttached", (float)textureAttached);

	if (textureAttached) {
    //Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureData->textureBit);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, uniqueVertices.size());
	glBindVertexArray(0);

}

//Create the object 
void GameObject::CreateObject(glm::vec3 worldPosition, float scale, GLenum renderMode, std::string objPath, std::string texturePath)
{
	//Set the created flag
	created = true;
	
	//Set transform properties
	this->worldPosition = worldPosition;
	this->scale = scale;

	//Setup Matrices
	translator = glm::mat4(1.0f);
	model = glm::mat4(1.0f);

	//Translate model to world position
	model = glm::translate(model, worldPosition);

	//Set render mode
	this->renderMode = renderMode;
	
	//Set total vertex count
	int totalVertexCount = (int)uniqueVertices.size();

	//load the shader
	RenderPass3_Shader = Shader("Shaders/RP3_Vertex.txt", "Shaders/RP3_Fragment.txt");
	RenderPass1_Shader = Shader("Shaders/RP1_Vertex.txt", "Shaders/RP1_Fragment.txt",totalVertexCount);
	
	//Store the preprocessed data
	for (int i = 0; i < uniqueVertices.size(); i++)
	{
		computePipelineVertexData.push_back(uniqueVertices[i].position[0]);
		computePipelineVertexData.push_back(uniqueVertices[i].position[1]);
		computePipelineVertexData.push_back(uniqueVertices[i].position[2]);
		computePipelineVertexData.push_back(uniqueVertices[i].normal[0]);
		computePipelineVertexData.push_back(uniqueVertices[i].normal[1]);
		computePipelineVertexData.push_back(uniqueVertices[i].normal[2]);
		computePipelineVertexData.push_back(uniqueVertices[i].texCoord[0]);
		computePipelineVertexData.push_back(uniqueVertices[i].texCoord[1]);
		computePipelineVertexData.push_back(uniqueVertices[i].triangleIndex);
	}

}
	
//Creates the User Defined FrameBuffer
void GameObject::CreateFrameBuffer(float width, float height,float texWidth, float texHeight)
{
	//Create a frame buffer
	RP1_FrameBuffer = 0;
	glGenFramebuffers(1, &RP1_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, RP1_FrameBuffer);

	//Create a texture
	glGenTextures(1, &RP1_TextureMap);
	glBindTexture(GL_TEXTURE_2D, RP1_TextureMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Set the rendering operation to use depth and this user defined buffer
	glGenRenderbuffers(1, &RP1_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, RP1_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RP1_DepthBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, RP1_TextureMap, 0);

	//Store the buffer
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	//Bind it 
	glBindFramebuffer(GL_FRAMEBUFFER, RP1_FrameBuffer);
	glViewport(0, 0, texWidth, texHeight);
}

//Attach the texture file
void GameObject::AttachTexture(Texture & texturePassed)
{
	//Set the texture reference
	textureData = &texturePassed;

	//Setup the texture
	glGenTextures(1, &textureData->textureBit);
	glBindTexture(GL_TEXTURE_2D, textureData->textureBit);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->textureWidth, textureData->textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->textureData.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setup the uniform buffer sampler
	glUniform1i(glGetUniformLocation(RenderPass3_Shader.Shader_ID, "textureData"), 0);

	//Set the attached flag to true
	textureAttached = true;

}

//Setup and bind first render pass data
void GameObject::BindRenderPass_1()
{
	//Bind vertex buffer and index buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) *uniqueVertices.size(), uniqueVertices.data(), GL_STATIC_DRAW);

	//Setup the layout binding
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
}

//Setup and bind second render pass data
void GameObject::BindRenderPass_3()
{
	//Bind vertex buffer and index buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) *uniqueVertices.size(), uniqueVertices.data(), GL_STATIC_DRAW);

	//Setup the layout binding
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);


}


GameObject::~GameObject()
{
}
