#pragma once
#include "Module.h"
#include "ModuleFiles.h"
#include "Texture.h"

class GameObject;

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

	/// <summary>
	/// Import a file from assets to our desired file format.
	/// </summary>
	void ImportFile(const std::string& filePath);

	Resource* LoadFile(const std::string& filePath);

	bool IsFileLoaded(std::string fileName);
	bool IsFileLoaded(const char* fileName);

	std::map<std::string, Resource*> loadedResources;
};

