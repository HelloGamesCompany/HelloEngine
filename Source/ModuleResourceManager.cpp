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

#include "GameObject.h"
#include "ModuleLayers.h"

std::map<std::string, Resource*> ModuleResourceManager::loadedResources;
std::map<uint, Resource*> ModuleResourceManager::resources;
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
	for (auto& resource1 : resources)
	{
		RELEASE(resource1.second);
	}
	resources.clear();

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

	// TODO: Restruct Inport parameters
	switch (type)
	{
	case ResourceType::MODEL:
	{
		std::string path = MeshImporter::ImportModel(filePath);
		ModuleFiles::S_CreateMetaData(filePath, path);
	}
	break;
	case ResourceType::TEXTURE:
	{
		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);
		std::string path = TextureImporter::ImportImage(ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		ModuleFiles::S_CreateMetaData(filePath, path);
		RELEASE_ARRAY(buffer);
	}
	break;
	default:
		break;
	}
}

void ModuleResourceManager::S_ReImportFile(const std::string& filePath, ResourceType resourceType)
{
	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	// TODO: Restruct Inport parameters
	switch (resourceType)
	{
	case ResourceType::MODEL:
	{
		std::string path = MeshImporter::ImportModel(filePath);
		ModuleFiles::S_UpdateMetaData(filePath, path);
	}
	break;
	case ResourceType::TEXTURE:
	{
		std::string path = TextureImporter::ImportImage(ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		ModuleFiles::S_UpdateMetaData(filePath, path);
	}
	break;
	default:
		break;
	}

	RELEASE_ARRAY(buffer);
}

void ModuleResourceManager::S_LoadFileIntoResource(Resource* resource)
{
	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(resource->resourcePath, &buffer);

	switch (resource->type)
	{
	case ResourceType::MODEL:
	{
		// When loading a Model, we need to load each mesh of the model into separate Mesh Resources.
		// A meta file of a model IS NOT a ResourceMesh instance.
		// When loading a Model, all Resources of that model get loaded, but there is no ResourceModel.
		ResourceModel* meshRes = (ResourceModel*)resource;
		MeshImporter::LoadModelIntoResource(meshRes);
	}
	break;
	case ResourceType::TEXTURE:
	{
		ResourceTexture* textureRes = (ResourceTexture*)resource;
		TextureImporter::Load(buffer, size, textureRes);
	}
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

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	switch (type)
	{
	case ResourceType::MESH:
	{
		/*ResourceMesh* resource = new ResourceMesh();
		resource->meshParent = MeshImporter::LoadMesh(filePath);
		RELEASE_ARRAY(buffer);
		loadedResources[ModuleFiles::S_GetFileName(filePath, true)] = resource;
		return resource;*/
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
		TextureImporter::Load(buffer, size, resource);
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

void ModuleResourceManager::S_SerializeScene(GameObject*& g)
{
	if (!g)
		return;

	// Create json
	json j;

	// Write json
	SerializeSceneRecursive(g, j);

	// Save json 
	std::string savePath = fileTree->_currentDir->path + g->name + ".HScene";

	std::string buffer = j.dump();

	ModuleFiles::S_Save(savePath, &buffer[0], buffer.size(), false);
}

void ModuleResourceManager::S_DeserializeScene(const std::string& filePath)
{
	ModuleLayers* layers = Application::Instance()->layers;

	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	json j = json::parse(buffer);

	RELEASE(buffer);

	// Create New GameObject for root GameObject

	layers->rootGameObject->Destroy();

	std::vector<std::pair<GameObject*, uint>> temp;

	for (int i = 0; i < j.size(); i++)
	{
		GameObject* g = new GameObject(nullptr, j[i]["Name"], j[i]["Tag"], j[i]["UID"]);

		temp.push_back(std::make_pair(g, j[i]["ParentUID"]));

		// Create components
		
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].second != 0)
			temp[i].first->SetParent(layers->gameObjects[temp[i].second]);
	}

	layers->rootGameObject = temp[0].first;
}

void ModuleResourceManager::S_DeleteMetaFile(const std::string& file, bool onlyResources)
{
	MetaFile meta = ModuleFiles::S_LoadMeta(file);

	switch (meta.type)
	{
	case ResourceType::MODEL:
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
	if (!onlyResources)
		ModuleFiles::S_Delete(file);
}

void ModuleResourceManager::S_CreateResource(const MetaFile& metaFile)
{
	if (resources.count(metaFile.UID) != 0)
		return;

	switch (metaFile.type)
	{
	case ResourceType::MESH:
	{
		resources[metaFile.UID] = new ResourceMesh();
	}
	break;
	case ResourceType::MODEL:
	{
		resources[metaFile.UID] = new ResourceModel();
	}
	break;
	case ResourceType::TEXTURE:
	{
		resources[metaFile.UID] = new ResourceTexture();
	}
	break;
	default:
		Console::S_Log("Cannot create a resource of an undefined meta file!");
		return;
	}

	resources[metaFile.UID]->referenceCount = 0;
	resources[metaFile.UID]->resourcePath = metaFile.resourcePath;
	resources[metaFile.UID]->type = metaFile.type;
	resources[metaFile.UID]->debugName = metaFile.name;
	resources[metaFile.UID]->UID = metaFile.UID;
}

void ModuleResourceManager::S_CreateResourceMesh(const std::string& filePath, uint UID, const std::string& name)
{
	if (resources.count(UID) != 0)
		return;

	ResourceMesh* newResource = new ResourceMesh();
	newResource->meshInfo.LoadFromBinaryFile(filePath);

	resources[UID] = newResource;
	resources[UID]->debugName = name + ".hmesh";
	resources[UID]->UID = UID;
}

Resource* ModuleResourceManager::S_LoadResource(const uint& UID)
{
	if (resources.count(UID) == 0)
		return nullptr;

	Resource* ret = resources[UID];
	
	// Load resource
	if (ret->referenceCount == 0)
		S_LoadFileIntoResource(ret);
		
	ret->referenceCount++;

	return ret; // We assume we always find a resource.
}

bool ModuleResourceManager::S_IsResourceCreated(const uint& UID)
{
	return resources.count(UID) == 1;
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

void ModuleResourceManager::SerializeSceneRecursive(const GameObject* g, json& j)
{
	json _j;

	_j["ParentUID"] = g->_parent ? g->_parent->_ID : 0;
	_j["UID"] = g->_ID;
	_j["Name"] = g->name;
	_j["Tag"] = g->tag;

	for (int i = 0; i < g->_components.size(); i++)
	{
		// Serialize components
		g->_components[i]->Serialization(_j);
	}

	j.push_back(_j);

	for (int i = 0; i < g->_children.size(); i++)
	{
		// Recursive to serialize children
		SerializeSceneRecursive(g->_children[i], j);
	}
}