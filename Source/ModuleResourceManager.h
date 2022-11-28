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
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture() { type = ResourceType::TEXTURE; }
	~ResourceTexture() {}
	Texture textureInfo;
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh() { type = ResourceType::MESH; };
	~ResourceMesh() {};

	GameObject* meshParent = nullptr;
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

	static bool S_IsFileLoaded(const std::string& fileName);

	static bool S_GetFileTree(FileTree*& tree);

	static void S_UpdateFileTree();

	/// <summary>
	/// Delete meta file and the resources attached to it. If you want to only destroy the resources, mark bool as true.
	/// </summary>
	static void S_DeleteMetaFile(const std::string& file, bool onlyResources = false);

private:
	static void GetResourcePath(ModelNode& node, std::vector<std::string>& vector);

public:
	static std::map<std::string, Resource*> loadedResources;

private:
	static FileTree* fileTree;
};