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
	for (int i = 0; i < loadedResources.size(); i++)
	{
		RELEASE(loadedResources[i]);
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
		ResourceTexture* resource = new ResourceTexture();
		resource->textureInfo.OpenGLID = TextureImporter::Load(buffer, size, nullptr, nullptr, ModuleFiles::S_GetFileName(filePath, false));
		RELEASE_ARRAY(buffer);
		loadedResources.push_back(resource);
		return resource;
	}
	}

	RELEASE_ARRAY(buffer);

	return nullptr;
}
