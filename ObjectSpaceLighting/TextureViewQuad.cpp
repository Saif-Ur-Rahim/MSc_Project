#include "TextureViewQuad.h"



TextureViewQuad::TextureViewQuad()
{
}

void TextureViewQuad::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition, unsigned int &renderTexture)
{
	shader.Activate();


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glUniform1i(glGetUniformLocation(shader.Shader_ID, "textureMap"), 0);
	glActiveTexture(GL_TEXTURE0);


	

	glBindTexture(GL_TEXTURE_2D, renderTexture);




	model = glm::mat4(1.0f);

	model = glm::translate(model, worldPosition);

	shader.SetMat4("model", model);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);;
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void TextureViewQuad::Create()
{
	shader = Shader("Shaders/TextureViewQuadVertex.txt", "Shaders/TextureViewQuadFragment.txt");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//glEnableVertexAttribArray(0);

}

TextureViewQuad::~TextureViewQuad()
{
}
