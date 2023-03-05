#pragma once

#include "LayerGame.h"


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class GameObject;
struct ModelNode;
class ResourceModel;

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

	// Returns path to the model custom file.
	static std::string ImportModel(std::string path, uint UID = 0);

	static GameObject* LoadModelIntoResource(ResourceModel* resource);

	static GameObject* LoadModelIntoScene(ResourceModel* resource);

	static GameObject* LoadModel(const std::string& path);

private:
	static void ProcessNode(aiNode* node, const aiScene* scene, ModelNode& parentNode);
	static std::string ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string fileName);
	static uint ProcessTexture(const std::string& textureName);
	static std::map<std::string, BoneData> ProcessBones(std::vector<Vertex>* vertices, aiMesh* mesh, const aiScene* scene);

	static void ProcessAnimation(const aiScene* scene);

	static void SetVertexBoneData(Vertex& vertex, int boneId = -1, float weight = 0);

	static void LoadNode(ModelNode& node, GameObject* parent);
	static void LoadMeshNode(std::string filePath, GameObject* parent);

	static void LoadTexture(uint resourceUID, GameObject* parent);

	static const aiScene* GetAiScene(std::string path);
		
	static Assimp::Importer importer;
	static GameObject* returnGameObject;
	static std::string currentPath;
};

