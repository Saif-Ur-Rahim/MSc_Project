#pragma once
#ifndef _MODELIMPORTER_
#define _MODELIMPORTER_
#include<iostream>
#include <string>
#include<vector>
#include<fstream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "VertexData.h"
#include "ObjVTNPair.h"
#include "GameObject.h"
#include"ModelRepository.h"
class ModelImportor
{
public:

	//Contairs to store OBJ data
	std::vector<ObjVTNPair> pairsData;
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec3> objNormals;
	std::vector<glm::vec2> objTextureCoordinates;

	//Reference to repo
	ModelRepository * modelRepository;

	//Constructor
	ModelImportor();
	ModelImportor(ModelRepository &modelRepo);

	//Function to load tri face obj file
	bool LoadObjFile(std::string path, std::shared_ptr <GameObject> const &gameObject);

	//Function to add obj data pair to the list
	void AddPair(ObjVTNPair pair, std::shared_ptr <GameObject>const &gameObject);

	//Function to setup a game object fields
	void SetupGameObjectData(std::shared_ptr <GameObject>const &gameObject);

	//Utility function to compare custom class values
	static bool ComparePairs(ObjVTNPair pair, ObjVTNPair other);
	
	//Destructor
	~ModelImportor();
};
#endif
