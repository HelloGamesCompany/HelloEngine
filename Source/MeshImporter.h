#pragma once

#include "LayerGame.h"

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

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
	static void ProcessNode(aiNode* node, const aiScene* scene);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

