#include "ComputeShaderRP2.h"


//Contructor
ComputeShaderRP2::ComputeShaderRP2()
{

}

//Function to Initially Setup the Compute Shader Data
void ComputeShaderRP2::ComputeShaderTextureDataSetup(std::vector<unsigned char>& textureData, std::vector<float>&PreProcessedData, float textureWidth, float textureHeight)
{
	//Alot memory to result buffer
	ComputeResult = new unsigned char[textureWidth * textureHeight * 4];

	//Clear Original texture vector and fill it with passed data
	OriginalTextureData.clear();
	for (int i = 0; i < textureData.size(); i++)
	{
		OriginalTextureData.push_back(((float)textureData[i]) / 255);
	}

	//Shader Object Initialization and activation
    shaderCompute = Shader("Shaders/ComputeShaders/ComputeShading.txt");
	shaderCompute.Activate();

	//SSBO to store preprocessed data
	GLuint ssbo2;
	glGenBuffers(1, &ssbo2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * PreProcessedData.size(), PreProcessedData.data(), GL_STATIC_READ); 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo2);

	//SSBO to store original texture data
	GLuint ssbo3;
	glGenBuffers(1, &ssbo3);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * OriginalTextureData.size(), OriginalTextureData.data(), GL_STATIC_READ); 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo3);
	

}

//Debug Function to store data to file
void ComputeShaderRP2::WriteDataToFile(float textureWidth, float textureHeight)
{
	int sizePixel = textureWidth * textureHeight * 4;
	std::ofstream WriteToFile;
	WriteToFile.open("ComputeResult.ppm");
	WriteToFile << "P3\n";
	WriteToFile << textureWidth;
	WriteToFile << " ";
	WriteToFile << textureHeight;
	WriteToFile << "\n";
	WriteToFile << "255\n";
	int tri = 0;
	for (int i = 0; i < sizePixel; i++)
	{

		if (tri == 3)
		{
			tri = 0;
		}
		else
		{
			WriteToFile << (int)ComputeResult[i];

			WriteToFile << "\n";
			tri++;
		}
	}
	WriteToFile.close();
	std::cout << "Image Generated" << std::endl;
}




//Function to Launch the Compute Shader
void ComputeShaderRP2::LaunchComputeShader(float *dataBuffer, std::vector<float>&vertexData,std::vector<unsigned char>&textureData, glm::vec3 cameraPosition, glm::mat4 modelMatrix,float textureWidth, float textureHeight)
{
	//Set Dimension of texture
	int tex_w = textureWidth;
	int tex_h = textureHeight;

	//Bind the texture
	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,	NULL);
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	
	//Activate the compute shader
	shaderCompute.Activate();

	//Pass data to uniform buffers
	shaderCompute.SetMat4("modelMatrix", modelMatrix);
	shaderCompute.SetVec3("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	shaderCompute.SetVec3("directionalLightPos", 1.0f, 1.0f, 1.0f);
	shaderCompute.SetVec3("directionalLightColor", 1.2f, 1.0f, 2.0f);
	shaderCompute.SetFloat("TextureWidth", textureWidth);
	shaderCompute.SetFloat("TextureHeight", textureHeight);

	//Size of preprocessed data
	int roundedSize = vertexData.size() / 27;
	shaderCompute.SetFloat("PreProcessedElementCount", (float)roundedSize);

	//SSBO to store frame buffer data
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*  textureWidth * textureHeight * 3, dataBuffer, GL_STATIC_READ); 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);

	//Run the shader
	glDispatchCompute((GLuint)textureWidth, (GLuint)textureHeight, 1);

	//Wait for result to be written
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//Read the result
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ComputeResult);


}


ComputeShaderRP2::~ComputeShaderRP2()
{
}

