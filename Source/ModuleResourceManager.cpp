#include "Headers.h"
#include "ModuleResourceManager.h"
#include "ModuleFiles.h"
#include "Console.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

ModuleResourceManager::ModuleResourceManager()
{
}

ModuleResourceManager::~ModuleResourceManager()
{
	for (auto& resource : loadedResources)
	{
		RELEASE(resource.second);
	}
	loadedResources.clear();
}

void ModuleResourceManager::ImportFile(const std::string& filePath)
{
	ResourceType type = ModuleFiles::S_GetResourceType(filePath);

	if (type == ResourceType::UNDEFINED)
	{
		Console::S_Log("Tried to load an undefined file. Filename: " + filePath);
		return;
	}

	// TODO: Create Meta object that knows where this resource will be inside Resources file.

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	switch (type)
	{
	case ResourceType::MESH:
		//MeshImporter::LoadMesh()
		break;
	case ResourceType::TEXTURE:
		 TextureImporter::ImportImage("Resources/Textures/" + ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		break;
	default:
		break;
	}

	RELEASE_ARRAY(buffer);
}

Resource* ModuleResourceManager::LoadFile(const std::string& filePath)
{
	ResourceType type = ModuleFiles::S_GetResourceType(filePath);

	if (type == ResourceType::UNDEFINED)
	{
		Console::S_Log("Tried to load an undefined file. Filename: " + filePath);
		return nullptr;
	}

	// TODO: Create Meta object that knows where this resource will be inside Resources file.

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	switch (type)
	{
	case ResourceType::MESH:
		//MeshImporter::LoadMesh()
		break;
	case ResourceType::TEXTURE:
	{
		// If we already loaded this texture, return its pointer.
		if (loadedResources.find(ModuleFiles::S_GetFileName(filePath, true)) != loadedResources.end())
		{
			RELEASE_ARRAY(buffer);
			return loadedResources[ModuleFiles::S_GetFileName(filePath, true)];
		}

		ResourceTexture* resource = new ResourceTexture();
		resource->textureInfo.OpenGLID = TextureImporter::Load(buffer, size, nullptr, nullptr, ModuleFiles::S_GetFileName(filePath, false));
		RELEASE_ARRAY(buffer);
		loadedResources[ModuleFiles::S_GetFileName(filePath, true)] = resource;
		return resource;
	}
	}

	RELEASE_ARRAY(buffer);

	return nullptr;
}

bool ModuleResourceManager::IsFileLoaded(std::string fileName)
{
	return loadedResources.find(fileName) != loadedResources.end();
}

bool ModuleResourceManager::IsFileLoaded(const char* fileName)
{
	return loadedResources.find(fileName) != loadedResources.end();
}
