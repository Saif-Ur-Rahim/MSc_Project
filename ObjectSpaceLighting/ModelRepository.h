#pragma once
#ifndef _MODELREPOSITORY_
#define _MODELREPOSITORY_
#include<vector>
#include <vector>
#include <memory>
#include <cstdio>
#include"GameObject.h"
class ModelRepository
{
public:

	//List of shared ptr to gameobjects
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	//Constructor
	ModelRepository();

	//Function adds GameObject to repo
	void AddGameObjectToRepo(std::shared_ptr <GameObject>const &gameObject);

	//Destructor
	~ModelRepository();
};
#endif
