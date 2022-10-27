#pragma once
#include "Module.h"
#include "ModuleFiles.h"
#include "Texture.h"

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

	std::vector<Resource*> loadedResources;
};

