#include "ModelImportor.h"


//Constructor
ModelImportor::ModelImportor()
{
}

//Constructor
ModelImportor::ModelImportor(ModelRepository & modelRepo)
{
	modelRepository = &modelRepo;
}


//Function to load tri face obj file
bool ModelImportor::LoadObjFile(std::string path, std::shared_ptr <GameObject> const & gameObject)
{
	float triIndex = 0;

	//Clear containers
	objVertices.clear();
	objNormals.clear();
	objTextureCoordinates.clear();
	pairsData.clear();

	std::ifstream inFile;
	//Get the file
	std::size_t found = path.find(".obj");

	//Check if file was found
	if (!(found != std::string::npos))
	{
		std::cout << "Please provide a file with .obj Extension" << std::endl;
		return false;
	}
	//Check if path was empty
	if (path.empty())
	{
		std::cout << "Please provide a file with .obj Extension this path is empty" << std::endl;
		return false;
	}
	else
	{
		//Open the file
		inFile.open(path);
		if (!inFile)
		{
			std::cout << "Unable to the file" << std::endl;
			return false;
		}
		else
		{
			std::string v1;
			while (inFile >> v1)
			{
				//Get the line which starts with v
				if (v1 == "v")
				{
					//Get the vertices values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					glm::vec3 v(x, y, z);
					objVertices.push_back(v);
				}
				//Get the line which starts with vn
				else if (v1 == "vn")
				{
					//Get the normal values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					glm::vec3 n(x, y, z);
					objNormals.push_back(n);
				}
				//Get the line which starts with vt
				else if (v1 == "vt")
				{
					//Get the textureCoordinates values and store them in list
					float x, y;
					inFile >> x >> y;
					glm::vec2 t(x, y);
					objTextureCoordinates.push_back(t);

				}
				//Get the line which starts with f
				else if (v1 == "f")
				{
					uint32_t vp1, vt1, vn1, vp2, vt2, vn2, vp3, vt3, vn3;
					char slash;
					//Fetch the three bundles of indices for current face (v1/vt1/vn1)
					inFile >> vp1 >> slash >> vt1 >> slash >> vn1;
					inFile >> vp2 >> slash >> vt2 >> slash >> vn2;
					inFile >> vp3 >> slash >> vt3 >> slash >> vn3;
					
					VertexData vertex;

					vertex.position[0] = objVertices[vp1 - 1].x;
					vertex.position[1] = objVertices[vp1 - 1].y;
					vertex.position[2] = objVertices[vp1 - 1].z;
					vertex.normal[0] = objNormals[vn1 - 1].x;
					vertex.normal[1] = objNormals[vn1 - 1].y;
					vertex.normal[2] = objNormals[vn1 - 1].z;
					vertex.texCoord[0] = objTextureCoordinates[vt1 - 1].x;
					vertex.texCoord[1] = objTextureCoordinates[vt1 - 1].y;
					vertex.triangleIndex = triIndex;

					VertexData vertex2;

					vertex2.position[0] = objVertices[vp2 - 1].x;
					vertex2.position[1] = objVertices[vp2 - 1].y;
					vertex2.position[2] = objVertices[vp2 - 1].z;
					vertex2.normal[0] = objNormals[vn2 - 1].x;
					vertex2.normal[1] = objNormals[vn2 - 1].y;
					vertex2.normal[2] = objNormals[vn2 - 1].z;
					vertex2.texCoord[0] = objTextureCoordinates[vt2 - 1].x;
					vertex2.texCoord[1] = objTextureCoordinates[vt2 - 1].y;
					vertex2.triangleIndex = triIndex;

					VertexData vertex3;

					vertex3.position[0] = objVertices[vp3 - 1].x;
					vertex3.position[1] = objVertices[vp3 - 1].y;
					vertex3.position[2] = objVertices[vp3 - 1].z;
					vertex3.normal[0] = objNormals[vn3 - 1].x;
					vertex3.normal[1] = objNormals[vn3 - 1].y;
					vertex3.normal[2] = objNormals[vn3 - 1].z;
					vertex3.texCoord[0] = objTextureCoordinates[vt3 - 1].x;
					vertex3.texCoord[1] = objTextureCoordinates[vt3 - 1].y;
					vertex3.triangleIndex = triIndex;


					gameObject->uniqueVertices.push_back(vertex);
					gameObject->uniqueVertices.push_back(vertex2);
					gameObject->uniqueVertices.push_back(vertex3);
					triIndex++;
				}

			}
			gameObject->name = path;
			modelRepository->AddGameObjectToRepo(gameObject);
			return true;
		}


	}
}


//Function to add obj data pair to the list
void ModelImportor::AddPair(ObjVTNPair pair, std::shared_ptr <GameObject>const &gameObject)
{
	for (unsigned int i = 0; i < gameObject->objPairs.size(); i++)
	{
		bool status = ComparePairs(pair, gameObject->objPairs[i]);
		if (status)
		{
			gameObject->indices.push_back(i);
			return;
		}
	}
	gameObject->objPairs.push_back(pair);
	gameObject->indices.push_back(gameObject->objPairs.size() - 1);
}

//Function to setup a game object fields
void ModelImportor::SetupGameObjectData(std::shared_ptr <GameObject>const &gameObject)
{

	for (unsigned int i = 0; i < gameObject->objPairs.size(); i++)
	{
		int vertexIndex = gameObject->objPairs[i].vertexIndex;
		int texCoordIndex = gameObject->objPairs[i].texCoordIndex;
		int normalIndex = gameObject->objPairs[i].normalIndex;

		VertexData vertex;
		vertex.position[0] = objVertices[vertexIndex].x;
		vertex.position[1] = objVertices[vertexIndex].y;
		vertex.position[2] = objVertices[vertexIndex].z;
		vertex.normal[0] = objNormals[normalIndex].x;
		vertex.normal[1] = objNormals[normalIndex].y;
		vertex.normal[2] = objNormals[normalIndex].z;
		vertex.texCoord[0] = objTextureCoordinates[texCoordIndex].x;
		vertex.texCoord[1] = objTextureCoordinates[texCoordIndex].y;
		
		gameObject->uniqueVertices.push_back(vertex);

	}



}

//Utility function to compare custom class values
bool ModelImportor::ComparePairs(ObjVTNPair pair, ObjVTNPair other)
{
	if (pair.vertexIndex == other.vertexIndex && pair.texCoordIndex == other.texCoordIndex && pair.normalIndex == other.normalIndex)
	{
		return true;
	}
	else
	{
		return false;
	}
}

ModelImportor::~ModelImportor()
{
}
