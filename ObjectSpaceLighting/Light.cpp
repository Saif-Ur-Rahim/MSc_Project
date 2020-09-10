#include "Light.h"



//Constructor
Light::Light()
{
}

//Main function to render the object
void Light::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition)
{
	shader.Activate();
	model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.SetMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

//Create the object
void Light::CreateLight(Vector3 position)
{
	worldPosition = glm::vec3(position.x, position.y, position.z);
	model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);

	shader = Shader("Shaders/LightVertex.txt", "Shaders/LightFragment.txt");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	vertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

//Destructor
Light::~Light()
{
}
