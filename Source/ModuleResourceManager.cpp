#include "Headers.h"
#include "ModuleResourceManager.h"
#include "ModuleFiles.h"
#include "Console.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "json.hpp"
#include "FileTree.hpp"


std::map<std::string, Resource*> ModuleResourceManager::loadedResources;
FileTree* ModuleResourceManager::fileTree = nullptr;

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

	RELEASE(fileTree);
}

bool ModuleResourceManager::Init()
{
	// Create meta files for every asset that doesnt have one.
	// Check if file has a defined reosurce type
	// If it does, create meta file. (MODULEFILESYSTEM)

	fileTree = new FileTree();

	ModuleFiles::S_UpdateFileTree(fileTree);

	// Check all meta files and create a resource per file using FileTree.
	// Save all resources in a map, using as key the meta file UID.
	// Unload all resources.

	// When a resource needs to be loaded, it will always be already created.
	return true;
}

void ModuleResourceManager::S_ImportFile(const std::string& filePath)
{
	ResourceType type = ModuleFiles::S_GetResourceType(filePath);

	if (type == ResourceType::UNDEFINED)
	{
		Console::S_Log("Tried to import an undefined file. Filename: " + filePath);
		return;
	}

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	// TODO: Restruct Inport parameters
	switch (type)
	{
	case ResourceType::MESH:
	{
		std::string path = MeshImporter::ImportModel(filePath);
		ModuleFiles::S_CreateMetaData(filePath, path);
	}
	break;
	case ResourceType::TEXTURE:
	{
		std::string path = TextureImporter::ImportImage(ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		ModuleFiles::S_CreateMetaData(filePath, path);
	}
	break;
	default:
		break;
	}

	RELEASE_ARRAY(buffer);
}

Resource* ModuleResourceManager::S_LoadFile(const std::string& filePath)
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

bool ModuleResourceManager::S_IsFileLoaded(const std::string& fileName)
{
	return loadedResources.find(fileName) != loadedResources.end();
}

bool ModuleResourceManager::S_GetFileTree(FileTree*& tree)
{
	if(fileTree)
	{
		tree = fileTree;
		return true;
	}
	return false;
}

void ModuleResourceManager::S_UpdateFileTree()
{
	ModuleFiles::S_UpdateFileTree(fileTree);
}

void ModuleResourceManager::S_DeleteMetaFile(const std::string& file)
{
	MetaFile meta = ModuleFiles::S_LoadMeta(file);

	switch (meta.type)
	{
	case ResourceType::MESH:
		{
			std::vector<std::string> meshFilePaths;

			ModelNode model;
			model.ReadFromJSON(meta.resourcePath);
			GetResourcePath(model, meshFilePaths);

			for (int i = 0; i < meshFilePaths.size(); i++)
			{
				ModuleFiles::S_Delete(meshFilePaths[i]);
			}
			ModuleFiles::S_Delete(meta.resourcePath);
		}
		break;
	default:
		{
		ModuleFiles::S_Delete(meta.resourcePath);
		}
		break;
	}
	ModuleFiles::S_Delete(file);
}

void ModuleResourceManager::GetResourcePath(ModelNode& node, std::vector<std::string>& vector)
{
	if (node.meshPath != "N")
	{
		vector.push_back(node.meshPath);
	}
	for (int i = 0; i < node.children.size(); i++)
	{
		GetResourcePath(node.children[i], vector);
	}
}
