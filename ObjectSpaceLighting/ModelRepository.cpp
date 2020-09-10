#include "ModelRepository.h"


//Constructor
ModelRepository::ModelRepository()
{
}

//Function adds GameObject to repo
void ModelRepository::AddGameObjectToRepo(std::shared_ptr <GameObject>const &gameObject)
{
	gameObjects.push_back(gameObject);
}

//Destructor
ModelRepository::~ModelRepository()
{
}
