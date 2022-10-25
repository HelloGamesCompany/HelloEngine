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

void ModuleResourceManager::LoadFile(const std::string& filePath)
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
		TextureImporter::Load(buffer, size, nullptr, nullptr);
		break;
	default:
		break;
	}

	RELEASE_ARRAY(buffer);
}
