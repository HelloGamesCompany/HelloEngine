#pragma once
#include "Module.h"
#include "ModuleFiles.h"
#include "Texture.h"

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
	void ImportFile(const std::string& filePath);

	Resource* LoadFile(const std::string& filePath);

	bool IsFileLoaded(const std::string& fileName);

	bool IsFileLoaded(const char* fileName);

	bool GetFileTree(FileTree*& tree);

	void UpdateFileTree();

	static void S_DeleteMetaFile(const std::string& file);

public:
	std::map<std::string, Resource*> loadedResources;

private:
	FileTree* fileTree = nullptr;
};