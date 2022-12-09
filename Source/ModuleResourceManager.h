#pragma once

#include "Module.h"
#include "ModuleFiles.h"
#include "Texture.h"
#include "File_Model.h"

class GameObject;
class FileTree;

class Resource
{
public:
	Resource() {}
	virtual ~Resource() {}

	void Dereference()
	{
		referenceCount--;
		if (referenceCount == 0)
			UnLoad();
	}
protected:
	virtual void UnLoad() {}

public:

	ResourceType type = ResourceType::UNDEFINED;
	uint referenceCount = 0;
	std::string resourcePath = "";
	std::string debugName = "NoName";
	uint UID = 0;
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture() { type = ResourceType::TEXTURE; }
	~ResourceTexture() {}

private:
	void UnLoad() override
	{
		glDeleteTextures(1, &OpenGLID);
		name = "";
		OpenGLID = 0;
		width = 0;
		height = 0;
		isTransparent = false;
	}

public:
	std::string name = "";
	uint OpenGLID = 0;
	int width = 0;
	int height = 0;
	bool isTransparent = false;
};

class ResourceModel : public Resource
{
public:
	ResourceModel() { type = ResourceType::MODEL; };
	~ResourceModel() {};

	void CreateResourceMeshes();

	ModelNode modelInfo;
	std::vector<ResourceMesh*> modelMeshes;
private:
	void CreateResourceMeshesRecursive(ModelNode& node);
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh() { type = ResourceType::MESH; };
	~ResourceMesh() {};

	void UnLoad() override
	{
		meshInfo.Clear();
		vertexNormals.clear();
		faceNormals.clear();

		//glDeleteBuffers(1, &VertexNormalsVAO);
		//glDeleteBuffers(1, &VertexNormalsVBO);
		//glDeleteBuffers(1, &FaceNormalsVAO);
		//glDeleteBuffers(1, &FaceNormalsVBO);

		//VertexNormalsVAO = 0;
		//VertexNormalsVBO = 0;
		//FaceNormalsVAO = 0;
		//FaceNormalsVBO = 0;
	}

	void CalculateNormalsAndAABB();

	MeshInfo meshInfo;
	uint modelUID = 0;
	uint indexInsideModel = 0;

	std::vector<float3> vertexNormals;
	std::vector<float3> faceNormals;
	AABB localAABB;

	uint VertexNormalsVAO = 0;
	uint VertexNormalsVBO = 0;
	uint FaceNormalsVAO = 0;
	uint FaceNormalsVBO = 0;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager();

	~ModuleResourceManager();

	bool Init() override;

	/// <summary>
	/// Import a file from assets to our desired file format.
	/// </summary>
	static void S_ImportFile(const std::string& filePath);

	static void S_ReImportFile(const std::string& filePath, ResourceType resourceType);

	static Resource* S_LoadFile(const std::string& filePath);

	static void S_LoadFileIntoResource(Resource* resource);

	static bool S_IsFileLoaded(const std::string& fileName);

	static bool S_GetFileTree(FileTree*& tree);

	static std::string S_GetCurrentWorkingDir();

	static void S_UpdateFileTree();

	static void S_SerializeScene(GameObject*& g);

	static bool S_DeserializeScene(const std::string& filePath);

	/// <summary>
	/// Delete meta file and the resources attached to it. If you want to only destroy the resources, mark bool as true.
	/// </summary>
	static void S_DeleteMetaFile(const std::string& file, bool onlyResources = false);

	static void S_CreateResource(const MetaFile& metaFile);

	// Only for internal engine usage!
	static void S_CreateResourceMesh(const std::string& filePath, uint UID, const std::string& name, bool load = true, ResourceModel* model = nullptr);
	static void S_CreateResourceText(const std::string& filePath, uint UID, const std::string& name, bool load = true);

	static Resource* S_LoadResource(const uint& UID);

	static bool S_IsResourceCreated(const uint& UID);

private:
	static void GetResourcePath(ModelNode& node, std::vector<std::string>& vector);

	static void SerializeSceneRecursive(const GameObject* g, json& j);

public:
	static std::map<std::string, Resource*> loadedResources;

	static std::map<uint, Resource*> resources;

private:
	static FileTree* _fileTree;
};