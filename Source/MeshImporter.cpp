#include "Headers.h"
#include "MeshImporter.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "ModelRenderManager.h"
#include "LayerGame.h"
#include "ModuleLayers.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModuleResourceManager.h"
#include "MaterialComponent.h"
#include "LayerEditor.h"

#include "File_Model.h"
#include "ModuleXML.h"

std::map<std::string, MeshCacheData> MeshImporter::loadedMeshes;
Assimp::Importer MeshImporter::importer;
GameObject* MeshImporter::returnGameObject = nullptr;
std::string MeshImporter::currentPath = "";

std::string MeshImporter::ImportModel(std::string path)
{
	// Load AiScene
	const aiScene* scene = GetAiScene(path);

	if (scene == nullptr)
	{
		std::string errorMessage = "Cannot load FBX: " + path;
		Console::S_Log(errorMessage);
		return "Null";
	}

	std::string modelFilePath = "Resources/Models/" + ModuleFiles::S_GetFileName(path, false) + ".xml";

	// Create XML file to store Model data.
	XMLNode xmlRootNode = Application::Instance()->xml->CreateXML(modelFilePath, "Model");

	// TODO: Material importer
	//pugi::xml_node texturesNode = xmlRootNode.node.append_child("Textures");
	//pugi::xml_node diffuseTextureNode = texturesNode.append_child("Diffuse");
	//pugi::xml_attribute diffuseTextureAtt = diffuseTextureNode.append_attribute("Name");
	
	ModelNode modelRootNode;
	for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		ProcessNode(scene->mRootNode->mChildren[i], scene, modelRootNode);
	}

	modelRootNode.WriteToXML("ModelRoot", xmlRootNode.node);
	
	// Save XML file
	xmlRootNode.Save(".hmodel");

	return modelFilePath;
}

void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene, ModelNode& parentNode)
{
	ModelNode newNode;
	newNode.name = node->mName.C_Str();

	// Get Node transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);

	float3 scale(scaling.x, scaling.y, scaling.z);

	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	newNode.position = pos;
	newNode.rotation = eulerRot;
	newNode.scale = scale;

	// We are assuming that every node can contain only one mesh!!!
	// If we have more than one mesh inside a node, we should put them together inside the same game object!
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		newNode.meshPath = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, newNode.name);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, newNode);
	}

	parentNode.children.push_back(newNode);

}

std::string MeshImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string fileName)
{
	MeshInfo meshInfo;

	// Get Mesh information
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normals.x = mesh->mNormals[i].x;
			vertex.normals.y = mesh->mNormals[i].y;
			vertex.normals.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;

			// Tangent and bit tangent
		}
		else
		{
			vertex.texCoords = { 0.0f,0.0f };
		}

		meshInfo.vertices.push_back(vertex);
	}

	meshInfo.indices.resize(mesh->mNumFaces * 3);

	if (mesh->HasFaces())
	{
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			memcpy(&meshInfo.indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	meshInfo.hasTexture = 0; // TODO: Determine this with Material importer later on.

	return meshInfo.SaveToBinaryFile(fileName);
}

void MeshImporter::LoadModel(std::string path)
{
	XMLNode rootNode = Application::Instance()->xml->OpenXML(path);

	// Read root node from XML
	ModelNode modelRootNode;
	modelRootNode.ReadFromXML("ModelRoot", rootNode);

	LoadNode(modelRootNode, nullptr);
	// Process root node recursively, checking children and meshes.
	// When get into a mesh, process its mesh file.
	// Every node and every mesh should be an independent GameObject, but respecting original hierarchy.
	// What should this function return?
}

void MeshImporter::LoadNode(ModelNode& node, GameObject* parent)
{
	GameObject* nodeGameObject = new GameObject(parent == nullptr ? Application::Instance()->layers->rootGameObject : parent, node.name);

	nodeGameObject->transform->SetPosition(node.position);
	nodeGameObject->transform->SetRotation(node.rotation);
	nodeGameObject->transform->SetScale(node.scale);

	if (node.meshPath != "N")
	{
		LoadMeshNode(node.meshPath, nodeGameObject);
	}

	for (int i = 0; i < node.childrenNum; i++)
	{
		LoadNode(node.children[i], nodeGameObject);
	}

}

void MeshImporter::LoadMeshNode(std::string filePath, GameObject* parent)
{
	MeshInfo meshInfo;
	meshInfo.LoadFromBinaryFile(filePath);

	MeshRenderComponent* meshRender = parent->AddComponent<MeshRenderComponent>();
	meshRender->InitAsNewMesh(meshInfo.vertices, meshInfo.indices);

}

GameObject* MeshImporter::LoadMesh(std::string path)
{
	returnGameObject = nullptr;
	currentPath = path;
	// Load AiScene
	const aiScene* scene = GetAiScene(path);

	if (scene == nullptr)
	{
		std::string errorMessage = "Cannot load FBX: " + path;
		Console::S_Log(errorMessage);
		return nullptr;
	}

	// Check if this file path has already been loaded.
	bool alreadyLoaded = loadedMeshes.find(path) != loadedMeshes.end();

	if (alreadyLoaded)
	{
		uint firstID = loadedMeshes[path].initialID;
		ProcessLoadedNode(scene->mRootNode, scene, firstID);
	}
	else
	{
		loadedMeshes[path].initialID = Application::Instance()->renderer3D->modelRender.GetMapSize(); // Set the ID of the first mesh inside this Model
		loadedMeshes[path].numOfMeshes = 0;
		loadedMeshes[path].meshDiffuseTexture = -1.0f;
		ProcessNewNode(scene->mRootNode, scene, path);
	}

	currentPath = "none";
	// Create a new RenderManager or add Mesh information as an instance
	// Create a MeshRenderComponent inside a GameObject for every Mesh, following the assimp hierachy structure.
	return returnGameObject;
}
const aiScene* MeshImporter::GetAiScene(std::string path)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	return scene;
}

void MeshImporter::ProcessNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		ProcessNewNode(node->mChildren[0], scene, path, parent);
		return;
	}

	// Create empty Gameobject 
	GameObject* newParent = nullptr;


	bool necessaryNode = node->mNumChildren > 1;

	if (parent == nullptr) returnGameObject = newParent = new GameObject(Application::Instance()->layers->rootGameObject, "Node");
	else newParent = new GameObject(parent, "NodeNew");

	// Set new GameObject position with node Transform.
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerZYX();	// TODO: Transform should save rotation as Quaternion?
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	newParent->GetComponent<TransformComponent>()->SetTransform(pos, { 1.0f,1.0f,1.0f }, eulerRot);

	uint meshNum = node->mNumMeshes;

	loadedMeshes[path].numOfMeshes += meshNum; // Increase the number of meshes for every mesh inside this node.

	for (unsigned int i = 0; i < meshNum; i++)
	{
		// Process mesh and create a GameObject with a MeshRenderComponent that is child to the epmty game object
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessNewMesh(mesh, scene, newParent, (meshNum > 1 || necessaryNode));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessNewNode(node->mChildren[i], scene, path, newParent);
	}
}

void MeshImporter::ProcessNewMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, bool createGO)
{
	std::vector<Vertex> vertices;	
	std::vector<uint> indices;

	// Get Mesh information
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normals.x = mesh->mNormals[i].x;
			vertex.normals.y = mesh->mNormals[i].y;
			vertex.normals.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;

			// Tangent and bit tangent
		}
		else
		{
			vertex.texCoords = { 0.0f,0.0f };
		}

		vertices.push_back(vertex);
	}

	indices.resize(mesh->mNumFaces * 3);

	if (mesh->HasFaces())
	{
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	// TODO: Load texture data 

	float textureID = -1.0f;

	if (mesh->mMaterialIndex >= 0)
	{
		// TODO: All the code inside this snippet is temporary to achieve Assignment 1 requirements of loading the mesh bounded texture.
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString path; 
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (path.length != 0)
		{
			std::string ddsName = path.C_Str();
			ddsName = ddsName.substr(0, ddsName.find_last_of("."));
			ddsName += ".dds";

			if (Application::Instance()->resource->IsFileLoaded(ddsName))
			{
				ResourceTexture* textureResource = (ResourceTexture*)Application::Instance()->resource->loadedResources[ddsName];
				textureID = textureResource->textureInfo.OpenGLID;
			}
			else
			{
				std::string filePath = currentPath;

				filePath = filePath.substr(0, filePath.find_last_of('/'));

				std::string assetsPath = filePath += "/";
				assetsPath += path.C_Str();

				if (ModuleFiles::S_Exists(assetsPath))
				{
					// Import file to create DDS file
					Application::Instance()->resource->ImportFile(assetsPath);

					// Get path of our loaded DDS file inside Resources
					std::string resourcesPath = "Resources/Textures/";

					resourcesPath += ddsName;

					// Load DDS file
					Resource* resource = Application::Instance()->resource->LoadFile(resourcesPath);

					if (resource->type != ResourceType::TEXTURE)
						return;

					ResourceTexture* textureResource = (ResourceTexture*)resource;

					textureID = textureResource->textureInfo.OpenGLID;
				}
				else
				{
					Application::Instance()->layers->editor->AddPopUpMessage("Error loading diffuse texture. Check console.");
					Console::S_Log("Error loading texture. The path inside FBX doesnt correspond with any file in the FBX current folder. Check that the FBX bounded texture is inside the same folder.");
					loadedMeshes[currentPath].displayMissingTextureError = true;
				}
			}

		}


	}

	loadedMeshes[currentPath].meshDiffuseTexture = textureID;
	bool hasMaterial = textureID != -1.0f;

	// Load into a Mesh object
	if (createGO)
	{
		GameObject* newGameObject = new GameObject(parent, mesh->mName.C_Str());
		MeshRenderComponent* meshRC = newGameObject->AddComponent<MeshRenderComponent>();
		meshRC->InitAsNewMesh(vertices, indices);
		meshRC->GetMesh().textureID = textureID;
		if (hasMaterial)
		{
			newGameObject->AddComponent<MaterialComponent>();
		}
	}
	else
	{
		parent->name = mesh->mName.C_Str();
		MeshRenderComponent* meshRC = parent->AddComponent<MeshRenderComponent>();
		meshRC->InitAsNewMesh(vertices, indices);
		meshRC->GetMesh().textureID = textureID;

		if (hasMaterial) 
		{
			parent->AddComponent<MaterialComponent>();
		}
	}
}

void MeshImporter::ProcessLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		ProcessLoadedNode(node->mChildren[0], scene, firstMeshID, parent);
		return;
	}

	// Create an empty GameObject that represents the Node
	GameObject* newParent = nullptr;

	bool necessaryNode = node->mNumChildren > 1;

	if (parent == nullptr)
	{
		returnGameObject = newParent = new GameObject(Application::Instance()->layers->rootGameObject, "Node");
	}
	else
	{
		newParent = new GameObject(parent, "Node");
	}

	// Set new GameObject position with node Transform.
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerZYX();	// TODO: Transform should save rotation as Quaternion?
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	newParent->GetComponent<TransformComponent>()->SetTransform(pos, { 1.0f,1.0f,1.0f }, eulerRot);

	ModelRenderManager* renderManager = &Application::Instance()->renderer3D->modelRender;

	uint meshNum = node->mNumMeshes;

	if ((meshNum > 1 || necessaryNode))
	{
		for (uint i = 0; i < meshNum; i++)
		{
			// Create a GameObject with a MeshRenderComponent that represents the Mesh
			GameObject* newGameObject = new GameObject(newParent, "Mesh");
			MeshRenderComponent* mesh = newGameObject->AddComponent<MeshRenderComponent>();
			mesh->InitAsLoadedMesh(firstMeshID++);
			mesh->GetMesh().textureID = loadedMeshes[currentPath].meshDiffuseTexture;

			if (loadedMeshes[currentPath].meshDiffuseTexture != -1)
			{
				newGameObject->AddComponent<MaterialComponent>();
			}
		}
	}
	else if (meshNum != 0)
	{
		MeshRenderComponent* mesh = newParent->AddComponent<MeshRenderComponent>();
		mesh->InitAsLoadedMesh(firstMeshID++);
		mesh->GetMesh().textureID = loadedMeshes[currentPath].meshDiffuseTexture;

		if (loadedMeshes[currentPath].meshDiffuseTexture != -1)
		{
			newParent->AddComponent<MaterialComponent>();
		}
	}

	if (loadedMeshes[currentPath].displayMissingTextureError)
	{
		Application::Instance()->layers->editor->AddPopUpMessage("Error loading diffuse texture. Check console.");
		Console::S_Log("Error loading texture. The path inside FBX doesnt correspond with any file in the FBX current folder. Check that the FBX bounded texture is inside the same folder.");
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessLoadedNode(node->mChildren[i], scene, firstMeshID, newParent);
	}
}

