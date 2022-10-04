#pragma once

#include "LayerGame.h"

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"


struct MeshCacheData
{
	uint numOfMeshes;
	uint initialID;
};

// This class is responsible for:
// Loading mesh data into the memory
// Checking if this mesh has already been loaded before
// If it has been loaded, create an instance.
// If not, load it inside a RenderManager and create an instance
// Then create an empty GameObject for every mesh loaded respectin the given file hierarchy
// When a GameObject with a MeshRenderComponent has been created for every mesh inside the loaded file, finish the process.
class MeshImporter
{
public:

	static void LoadMesh(std::string path); // This function loads all the information in the mesh and creates all necessary Gameobjects in the process

private:
	static const aiScene* GetAiScene(std::string path);
	
	// ProcessNew creates a new RenderManger for each Mesh
	static void ProcessNewNode(aiNode* node, const aiScene* scene, std::string path);
	static void ProcessNewMesh(aiMesh* mesh, const aiScene* scene);

	//ProcessLoaded creates a new Instance inside a RenderManager for each Mesh
	static void ProcessLoadedNode(aiNode* node, const aiScene* scene, uint firstMeshID);

	static std::map<std::string, MeshCacheData> loadedMeshes;
	static Assimp::Importer importer;
};

