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

std::map<std::string, MeshCacheData> MeshImporter::loadedMeshes;
Assimp::Importer MeshImporter::importer;

void MeshImporter::LoadMesh(std::string path)
{
	// Load AiScene
	const aiScene* scene = GetAiScene(path);

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
		ProcessNewNode(scene->mRootNode, scene, path);
	}


	// Create a new RenderManager or add Mesh information as an instance
	// Create a MeshRenderComponent inside a GameObject for every Mesh, following the assimp hierachy structure.

}

const aiScene* MeshImporter::GetAiScene(std::string path)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	return scene;
}

void MeshImporter::ProcessNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	// Create empty Gameobject 
	GameObject* newParent = nullptr;

	if (parent == nullptr) newParent = new GameObject(Application::Instance()->layers->rootGameObject, "Mesh");
	else newParent = new GameObject(parent, "Mesh");

	// Set new GameObject position with node Transform.
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerZYX();	// TODO: Transform should save rotation as Quaternion?

	newParent->GetComponent<TransformComponent>()->SetTransform(pos, {1.0f,1.0f,1.0f}, eulerRot);

	loadedMeshes[path].numOfMeshes += node->mNumMeshes; // Increase the number of meshes for every mesh inside this node.

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Process mesh and create a GameObject with a MeshRenderComponent that is child to the epmty game object
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessNewMesh(mesh, scene, newParent);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessNewNode(node->mChildren[i], scene, path, newParent);
	}


}

void MeshImporter::ProcessNewMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent)
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
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	// TODO: Load texture data 

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	}

	// Load into a Mesh object
	GameObject* newGameObject = new GameObject(parent, "Mesh");
	newGameObject->AddComponent<MeshRenderComponent>()->InitAsNewMesh(vertices, indices);

	// TODO: Generate a GameObject with a RenderMeshComponent
	//LayerGame* game = (LayerGame*)Application::Instance()->layers->layers[LayersID::GAME];//TEMPORAL CODE
	//game->meshComponentTest.push_back(newMeshComponent);
}

void MeshImporter::ProcessLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent)
{
	// Create an empty GameObject that represents the Node
	GameObject* newParent = nullptr;

	if (parent == nullptr) newParent = new GameObject(Application::Instance()->layers->rootGameObject, "Mesh");
	else newParent = new GameObject(parent, "Mesh");

	ModelRenderManager* renderManager = &Application::Instance()->renderer3D->modelRender;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Create a GameObject with a MeshRenderComponent that represents the Mesh
		GameObject* newGameObject = new GameObject(Application::Instance()->layers->rootGameObject, "Mesh");
		newGameObject->AddComponent<MeshRenderComponent>()->InitAsLoadedMesh(firstMeshID++);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessLoadedNode(node->mChildren[i], scene, firstMeshID, newParent);
	}
}

