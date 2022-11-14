#pragma once

#include "LayerGame.h"

#include "nommgr.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mmgr.h"

class GameObject;
struct ModelNode;

struct MeshCacheData
{
	uint numOfMeshes = 0;
	uint initialID = 0;
	uint meshDiffuseTexture = 0;
	bool displayMissingTextureError = false;
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

	static void ImportModel(std::string path);

	static GameObject* LoadMesh(std::string path); // This function loads all the information in the mesh and creates all necessary Gameobjects in the process

private:
	static void ProcessNode(aiNode* node, const aiScene* scene, ModelNode& parentNode);
	static std::string ProcessMesh(aiMesh* mesh, const aiScene* scene);

	static const aiScene* GetAiScene(std::string path);
	
	// ProcessNew creates a new RenderManger for each Mesh
	static void ProcessNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent = nullptr);
	static void ProcessNewMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, bool createGO = false);

	//ProcessLoaded creates a new Instance inside a RenderManager for each Mesh
	static void ProcessLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent = nullptr);

	static std::map<std::string, MeshCacheData> loadedMeshes;
	static Assimp::Importer importer;
	static GameObject* returnGameObject;
	static std::string currentPath;
};

