#include "Headers.h"
#include "ModuleResourceManager.h"
#include "ModuleFiles.h"
#include "Console.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

ModuleResourceManager::ModuleResourceManager()
{
	Console::S_Log("Initialaizing DevIL.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) 
	{
		Console::S_Log("Wrong DevIL version detected.");
	}
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
		Console::S_Log("Tried to import an undefined file. Filename: " + filePath);
		return;
	}

	// Create Meta data
	CreateMetaData(filePath);

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	// TODO: Restruct Inport parameters
	switch (type)
	{
	case ResourceType::MESH:
		MeshImporter::ImportModel(filePath);
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
	{
		ResourceMesh* resource = new ResourceMesh();
		resource->meshParent = MeshImporter::LoadMesh(filePath);
		RELEASE_ARRAY(buffer);
		loadedResources[ModuleFiles::S_GetFileName(filePath, true)] = resource;
		return resource;
	}
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
		resource->textureInfo.OpenGLID = TextureImporter::Load(buffer, size, nullptr, nullptr, filePath.c_str());
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

bool ModuleResourceManager::CreateMetaData(const std::string file)
{
	if (!CheckMetaExist(file))
	{
		std::string newFile = ModuleFiles::S_RemoveExtension(file) + ".helloMeta";

		// get modify time
		std::string meta = "Last modify: ";

		time_t currentTime = time(0);

		char time[26];

		ctime_s(time, sizeof(time), &currentTime);

		meta += time;

		// get resource path

		meta += "\nResource path : Resources/Textures/" + ModuleFiles::S_GetFileName(file, false);

		ModuleFiles::S_Save(newFile, &meta[0], meta.size(), false);

		return true;
	}

	return false;
}

bool ModuleResourceManager::CheckMetaExist(const std::string file)
{
	std::string meta = ModuleFiles::S_RemoveExtension(file) + ".helloMeta";

	return ModuleFiles::S_Exists(meta);
}
