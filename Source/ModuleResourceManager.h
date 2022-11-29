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
	ResourceType type = ResourceType::UNDEFINED;
	uint referenceCount = 0;
	std::string resourcePath = "";
	std::string debugName = "NoName";
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture() { type = ResourceType::TEXTURE; }
	~ResourceTexture() {}
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

	ModelNode modelInfo;
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh() { type = ResourceType::MESH; };
	~ResourceMesh() {};

	GameObject* meshParent = nullptr;
	MeshInfo meshInfo;
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

	static void S_UpdateFileTree();

	/// <summary>
	/// Delete meta file and the resources attached to it. If you want to only destroy the resources, mark bool as true.
	/// </summary>
	static void S_DeleteMetaFile(const std::string& file, bool onlyResources = false);

	static void S_CreateResource(const MetaFile& metaFile);

	static void S_CreateResourceMesh(std::string filePath, uint UID, const std::string& name);

	static Resource* S_LoadResource(uint UID);

	static bool S_IsResourceCreated(uint UID);

private:
	static void GetResourcePath(ModelNode& node, std::vector<std::string>& vector);

public:
	static std::map<std::string, Resource*> loadedResources;

	static std::map<uint, Resource*> resources;

private:
	static FileTree* fileTree;
};