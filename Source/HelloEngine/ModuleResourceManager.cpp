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
#include "ModuleCommand.h"

#include "GameObject.h"
#include "ModuleLayers.h"
#include "ModuleWindow.h"

// In create resource mesh method save my index and model UID.
// Save ResourceModel UID and index.
// On Deserialization, look for ModelUID. If exists, check if index exists. If exists, load that resource mesh.

std::map<std::string, Resource*> ModuleResourceManager::loadedResources;
std::map<uint, Resource*> ModuleResourceManager::resources;
FileTree* ModuleResourceManager::_fileTree = nullptr;

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

	RELEASE(_fileTree);
}

bool ModuleResourceManager::Init()
{
	// Create checkers texture resource
	S_CreateResourceText("Null", CHECKERS_RESOURCE_UID, "Checkers", false);

	// Create meta files for every asset that doesnt have one.
	// Check if file has a defined reosurce type
	// If it does, create meta file. (MODULEFILESYSTEM)

	_fileTree = new FileTree();

	ModuleFiles::S_UpdateFileTree(_fileTree);

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
	case ResourceType::HSCRIPT:
	case ResourceType::CPPSCRIPT:
	{
		ModuleFiles::S_CreateMetaData(filePath, "don't have resource path now");
		break;
	}
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
	case ResourceType::HSCRIPT:
	{
		// Call Hot reload
		// LayerGame::HotReload();
	}
	}

	RELEASE_ARRAY(buffer);
}

void ModuleResourceManager::S_LoadFileIntoResource(Resource* resource)
{
	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(resource->resourcePath, &buffer);

	if (size == 0)
		return;

	switch (resource->type)
	{
	case ResourceType::MODEL:
	{
		// When loading a Model, we need to load each mesh of the model into separate Mesh Resources.
		// A meta file of a model IS NOT a ResourceMesh instance.
		// When loading a Model, all Resources of that model get loaded, but there is no ResourceModel.
		ResourceModel* modelRes = (ResourceModel*)resource;
		MeshImporter::LoadModelIntoResource(modelRes);
	}
	break;
	case ResourceType::TEXTURE:
	{
		ResourceTexture* textureRes = (ResourceTexture*)resource;
		TextureImporter::Load(buffer, size, textureRes);
	}
	break;
	case ResourceType::MESH:
	{
		ResourceMesh* meshRes = (ResourceMesh*)resource;
		meshRes->meshInfo.LoadFromBinaryFile(meshRes->resourcePath);
		meshRes->CreateBuffers();
		meshRes->CalculateNormalsAndAABB();
	}
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
	if(_fileTree)
	{
		tree = _fileTree;
		return true;
	}
	return false;
}

std::string ModuleResourceManager::S_GetCurrentWorkingDir()
{
	return _fileTree->_currentDir->path;
}

void ModuleResourceManager::S_UpdateFileTree()
{
	ModuleFiles::S_UpdateFileTree(_fileTree);
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
	std::string savePath = Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").as_string();

	if (savePath == "null")
		savePath = _fileTree->_currentDir->path + g->name + ".HScene";

	// Change Title
	std::string scenePath = " -- CurrentScene: " + savePath;

	ModuleWindow::S_AddTitleExtraInfo(scenePath);

	std::string buffer = j.dump();

	ModuleFiles::S_Save(savePath, &buffer[0], buffer.size(), false);
}

bool ModuleResourceManager::S_DeserializeScene(const std::string& filePath)
{
	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	if (size == 0)
		return false;

	// Change Title
	std::string scenePath = " -- CurrentScene: " + filePath;

	ModuleWindow::S_AddTitleExtraInfo(scenePath);

	ModuleCommand::S_CleanCommandQueue();

	json sceneFile = json::parse(buffer);
	RELEASE(buffer);

	// Create New GameObject for root GameObject
	if(ModuleLayers::rootGameObject)
		ModuleLayers::rootGameObject->Destroy();

	std::vector<std::pair<GameObject*, uint>> temp;

	// First  create game objects
	for (int i = 0; i < sceneFile.size(); i++)
	{
		GameObject* g = new GameObject(nullptr, sceneFile[i]["Name"], sceneFile[i]["Tag"], sceneFile[i]["UID"]);

		temp.push_back(std::make_pair(g, sceneFile[i]["ParentUID"]));
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].second != 0)
			temp[i].first->SetParent(ModuleLayers::gameObjects[temp[i].second]);
	}

	// then add their components
	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			temp[i].first->AddComponentSerialized(componentType, object[j]);
		}
	}

	ModuleLayers::rootGameObject = temp[0].first;

	return true;
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
		ResourceModel* resourceModel = new ResourceModel();
		resources[metaFile.UID] = resourceModel;
		resources[metaFile.UID]->UID = metaFile.UID;
		resourceModel->resourcePath = metaFile.resourcePath;
		resourceModel->modelInfo.ReadFromJSON(resourceModel->resourcePath);
		resourceModel->CreateResourceMeshes();
	}
	break;
	case ResourceType::TEXTURE:
	{
		resources[metaFile.UID] = new ResourceTexture();
	}
	break;
	case ResourceType::HSCRIPT:
	case ResourceType::CPPSCRIPT:
	{
		resources[metaFile.UID] = new ResourceScript();
		// Get class name from path file name
		ResourceScript* res = (ResourceScript*)resources[metaFile.UID];
		res->className = ModuleFiles::S_GetFileName(metaFile.name, false);
		res->debugName = res->className;
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

void ModuleResourceManager::S_CreateResourceMesh(const std::string& filePath, uint UID, const std::string& name, bool load, ResourceModel* model)
{
	if (resources.count(UID) != 0)
		return;

	ResourceMesh* newResource = new ResourceMesh();
	if (load)
	{
		newResource->meshInfo.LoadFromBinaryFile(filePath);
		newResource->CreateBuffers();
		newResource->CalculateNormalsAndAABB();
	}
	resources[UID] = newResource;
	resources[UID]->debugName = name + ".hmesh";
	resources[UID]->UID = UID;
	resources[UID]->resourcePath = filePath;
	if (model != nullptr)
	{
		newResource->modelUID = model->UID;
		newResource->indexInsideModel = model->modelMeshes.size();
	}


}

void ModuleResourceManager::S_CreateResourceText(const std::string& filePath, uint UID, const std::string& name, bool load)
{
	if (resources.count(UID) != 0)
		return;

	ResourceTexture* newResource = new ResourceTexture();

	resources[UID] = newResource;
	resources[UID]->debugName = name + ".dds";
	resources[UID]->UID = UID;
	resources[UID]->resourcePath = filePath;

	if (load)
		S_LoadFileIntoResource(newResource);
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
		if(!g->_children[i]->_isPendingToDelete)
			SerializeSceneRecursive(g->_children[i], j);
	}
}

void ResourceModel::CreateResourceMeshes()
{
	if (modelInfo.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(modelInfo.meshPath, false);
		uint meshUID = std::stoul(stringUID);
		if (!ModuleResourceManager::S_IsResourceCreated(meshUID))
		{
			ModuleResourceManager::S_CreateResourceMesh(modelInfo.meshPath, meshUID, modelInfo.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
	}
	for (int i = 0; i < modelInfo.children.size(); i++)
	{
		CreateResourceMeshesRecursive(modelInfo.children[i]);
	}
}

void ResourceModel::CreateResourceMeshesRecursive(ModelNode& node)
{
	// Iterate model. Create resource mesh per mesh inside model.
	if (node.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(node.meshPath, false);
		uint meshUID = std::stoul(stringUID);
		if (!ModuleResourceManager::S_IsResourceCreated(meshUID))
		{
			ModuleResourceManager::S_CreateResourceMesh(node.meshPath, meshUID, node.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
	}

	for (int i = 0; i < node.children.size(); i++)
	{
		CreateResourceMeshesRecursive(node.children[i]);
	}
}

void ResourceMesh::CreateBuffers()
{
	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshInfo.vertices.size(), &meshInfo.vertices.front(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	// Create Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshInfo.indices.size(), &meshInfo.indices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ResourceMesh::CalculateNormalsAndAABB()
{
	// Vertex normals
	vertexNormals.resize(meshInfo.vertices.size() * 2);
	float lineMangitude = 0.5f;

	int j = 0;
	for (int i = 0; i < meshInfo.vertices.size() * 2; i++)
	{
		if (i % 2 == 0)
		{
			vertexNormals[i] = meshInfo.vertices[j].position;
		}
		else
		{
			vertexNormals[i] = meshInfo.vertices[j].position + (meshInfo.vertices[j].normals * lineMangitude);
			j++;
		}

	}

	// Face normals
	faceNormals.resize((meshInfo.indices.size() / 3) * 2); // 3 vertices make a face; we need 2 points to display 1 face normal. 
	lineMangitude = 0.5f;
	int k = 0;
	int l = 0;

	int iterations = faceNormals.size() / 2;
	for (int i = 0; i < iterations; i++)
	{
		float3 faceCenter = { 0,0,0 };
		for (int j = 0; j < 3; j++)
		{
			faceCenter += meshInfo.vertices[meshInfo.indices[k++]].position;
		}
		faceCenter /= 3;
		faceNormals.push_back(faceCenter);

		float3 normalsDir = { 0,0,0 };
		for (int j = 0; j < 3; j++)
		{
			normalsDir += meshInfo.vertices[meshInfo.indices[l++]].normals;
		}
		normalsDir /= 3;
		normalsDir.Normalize();
		faceNormals.push_back(faceCenter + (normalsDir * lineMangitude));
	}

	// AABB
	localAABB.SetNegativeInfinity();

	std::vector<float3> vertexPositions;
	vertexPositions.resize(meshInfo.vertices.size());
	for (int i = 0; i < meshInfo.vertices.size(); i++)
	{
		vertexPositions[i] = meshInfo.vertices[i].position;
	}
	localAABB.Enclose(&vertexPositions[0], meshInfo.vertices.size());

	// OpenGL buffers
	// vertex normals buffers
	glGenVertexArrays(1, &VertexNormalsVAO);
	glBindVertexArray(VertexNormalsVAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VertexNormalsVBO);

	glBindBuffer(GL_ARRAY_BUFFER, VertexNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * vertexNormals.size(), &vertexNormals[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	// Face normals buffers
	glGenVertexArrays(1, &FaceNormalsVAO);
	glBindVertexArray(FaceNormalsVAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &FaceNormalsVBO);

	glBindBuffer(GL_ARRAY_BUFFER, FaceNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * faceNormals.size(), &faceNormals[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

}
