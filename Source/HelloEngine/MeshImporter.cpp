#include "Headers.h"
#include "MeshImporter.h"
#include "LayerGame.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "ModuleResourceManager.h"
#include "MaterialComponent.h"
#include "LayerEditor.h"
#include "File_Model.h"
#include "SkinnedMeshRenderComponent.h"

Assimp::Importer MeshImporter::importer;
GameObject* MeshImporter::returnGameObject = nullptr;
std::string MeshImporter::currentPath = "";

std::string MeshImporter::ImportModel(std::string path, uint UID)
{
	// Load AiScene
	const aiScene* scene = GetAiScene(path);

	if (scene == nullptr)
	{
		std::string errorMessage = "Cannot load FBX: " + path;
		Console::S_Log(errorMessage);
		return "Null";
	}

	std::string stringUID = UID == 0 ? std::to_string(HelloUUID::GenerateUUID()) : std::to_string(UID);
	std::string modelFilePath = "Resources/Models/" + stringUID + ".hmodel";
	currentPath = path;

	ModelNode modelRootNode;
	for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		ProcessNode(scene->mRootNode->mChildren[i], scene, modelRootNode);
	}

	modelRootNode.WriteToJSON(modelFilePath);

	if (scene->HasAnimations()) {
		ProcessAnimation(scene);
	}

	currentPath = "";

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
	// If we have more than one mesh inside a node, we should create more nodes!
	if (node->mNumMeshes > 1)
	{
		Console::S_Log("Importing a model with multiple meshes inside the same node!");
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			ModelNode meshNode;
			meshNode.meshPath = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, newNode.name);
			newNode.children.push_back(meshNode);
			if (scene->mMeshes[node->mMeshes[i]]->mMaterialIndex >= 0)
			{
				aiString texture;
				scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
				if (texture.C_Str() != "")
					meshNode.resourceMaterialUID = ProcessTexture(texture.C_Str());
			}
		}
	}
	else if (node->mNumMeshes != 0)
	{
		newNode.meshPath = ProcessMesh(scene->mMeshes[node->mMeshes[0]], scene, newNode.name);
		if (scene->mMeshes[node->mMeshes[0]]->mMaterialIndex >= 0)
		{
			aiString texture;
			scene->mMaterials[scene->mMeshes[node->mMeshes[0]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
			if (texture.C_Str() != "")
				newNode.resourceMaterialUID = ProcessTexture(texture.C_Str());
		}
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

	// TODO: This is only to prevent a crash when animation controllers get imported as a mesh (for some reason) and they have null indices that point to infinity. 
	// Check that no indices are out of bounds (an index is pointing outside the bounds of the vertices array)
	for (int i = 0; i < meshInfo.indices.size(); i++)
	{
		if (meshInfo.indices[i] >= meshInfo.vertices.size())
		{
			meshInfo.indices[i] = meshInfo.vertices.size() - 1;
		}
	}

	if (mesh->HasBones())
	{
		meshInfo.boneDataMap = ProcessBones(&meshInfo.vertices, mesh, scene);
	}

	meshInfo.hasTexture = 0; // TODO: Determine this with Material importer later on.

	return meshInfo.SaveToBinaryFile(fileName);
}

uint MeshImporter::ProcessTexture(const std::string& textureName)
{
	if (textureName == "")
		return 0;

	// Check if the given texture is inside the current folder.
	std::string folder = currentPath.substr(0, currentPath.find_last_of('/')+1);

	std::string textureAsset = folder + textureName;

	// If the texture is inside the same folder
	if (ModuleFiles::S_Exists(textureAsset))
	{
		if (ModuleFiles::S_CheckMetaExist(textureAsset))
		{
			// If the texture has a meta file, return that meta file's resource path.
			return ModuleFiles::S_LoadMeta(textureAsset + ".helloMeta").UID;
		}
	}

	return 0;
}

std::map<std::string, BoneData> MeshImporter::ProcessBones(std::vector<Vertex>* vertices, aiMesh* mesh, const aiScene* scene)
{
	std::map<std::string, BoneData> boneDataMap;

	int boneCounter = 0;
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneId = -1;
		std::string name = mesh->mBones[boneIndex]->mName.C_Str();

		if (boneDataMap.find(name) == boneDataMap.end())
		{  //Creates a new bone into the map
			BoneData newBoneData;
			newBoneData.id = boneCounter;


			//Get bone offset matrix
			aiVector3D translation, scaling;
			aiQuaternion rotation;
			mesh->mBones[boneIndex]->mOffsetMatrix.Decompose(scaling, rotation, translation);

			float3 pos(translation.x, translation.y, translation.z);
			float3 scale(scaling.x, scaling.y, scaling.z);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

			float4x4 offsetMat = float4x4::FromTRS(pos, rot, scale);
			newBoneData.offset = offsetMat;


			boneDataMap[name] = newBoneData;
			boneId = boneCounter;
			boneCounter++;
		}
		else
		{
			boneId = boneDataMap[name].id;
		}

		assert(boneId != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices->size());

			SetVertexBoneData(vertices->at(vertexId), boneId, weight);
		}
	}

	return boneDataMap;
}

void MeshImporter::ProcessAnimation(const aiScene* scene)
{
	Animation3d* anim = new Animation3d();

	//Check ONLY 1 animation
	aiAnimation* importedAnimation = scene->mAnimations[0];
	anim->durationTicks = importedAnimation->mDuration;
	anim->ticksPerSecond = importedAnimation->mTicksPerSecond;
	anim->bones.reserve(importedAnimation->mNumChannels);

	//Get all bones info
	for (int c = 0; c < importedAnimation->mNumChannels; c++)
	{
		aiNodeAnim* impBone = importedAnimation->mChannels[c];

		AnimatedBone* bone = new AnimatedBone(impBone->mNodeName.C_Str(), anim->durationTicks + 1);

		//Build keyframe matrixes
		for (int p = 1; p < impBone->mNumPositionKeys; p++) {
			aiVector3D pos = impBone->mPositionKeys[p].mValue;

			bone->keyframes[(int)impBone->mPositionKeys[p].mTime].SetTranslatePart(float3(pos.x, pos.y, pos.z));
		}

		for (int r = 1; r < impBone->mNumRotationKeys; r++) {
			aiQuaternion impRot = impBone->mRotationKeys[r].mValue;
			Quat rot = Quat(impRot.x, impRot.y, impRot.z, impRot.w);

			bone->keyframes[(int)impBone->mRotationKeys[r].mTime].SetRotatePart(rot);
		}

		for (int s = 1; s < impBone->mNumScalingKeys; s++) {
			aiVector3D sca = impBone->mScalingKeys[s].mValue;

			bone->keyframes[(int)impBone->mScalingKeys[s].mTime][0][0] *= sca.x;
			bone->keyframes[(int)impBone->mScalingKeys[s].mTime][1][1] *= sca.y;
			bone->keyframes[(int)impBone->mScalingKeys[s].mTime][2][2] *= sca.z;
		}

		anim->bones.push_back(bone);
	}

	//TESTING

	RELEASE(anim);
}

void MeshImporter::SetVertexBoneData(Vertex& vertex, int boneId, float weight)
{
	for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
	{
		if (vertex.boneIds[i] < 0)
		{
			vertex.boneIds[i] = boneId;
			vertex.weights[i] = weight;
			break;
		}
	}
}

GameObject* MeshImporter::LoadModelIntoResource(ResourceModel* resource)
{
	returnGameObject = nullptr;
	// Read root node from JSON
	resource->modelInfo.ReadFromJSON(resource->resourcePath);

	// Process root node recursively, checking children and meshes.
	// When get into a mesh, process its mesh file.
	// Every node and every mesh should be an independent GameObject, but respecting original hierarchy.
	// Returns a pointer to the root game object of this model.
	return returnGameObject;
}

GameObject* MeshImporter::LoadModelIntoScene(ResourceModel* resource)
{
	returnGameObject = nullptr;
	LoadNode(resource->modelInfo, nullptr);
	return returnGameObject;
}

GameObject* MeshImporter::LoadModel(const std::string& path)
{
	returnGameObject = nullptr;
	// Read root node from JSON
	ModelNode rootNode;
	rootNode.ReadFromJSON(path);

	LoadNode(rootNode, nullptr);
	// Process root node recursively, checking children and meshes.
	// When get into a mesh, process its mesh file.
	// Every node and every mesh should be an independent GameObject, but respecting original hierarchy.
	// Returns a pointer to the root game object of this model.
	return returnGameObject;
}

void MeshImporter::LoadNode(ModelNode& node, GameObject* parent)
{
	GameObject* nodeGameObject = nullptr;

	if (parent == nullptr)
	{
		nodeGameObject = new GameObject(ModuleLayers::rootGameObject, node.name);
		returnGameObject = nodeGameObject;
	}
	else
	{
		nodeGameObject = new GameObject(parent, node.name);
	}

	nodeGameObject->transform->SetPosition(node.position);
	nodeGameObject->transform->SetRotation(node.rotation);
	nodeGameObject->transform->SetScale(node.scale);

	//// Check if this node could be joined with its only child into a single node.
	//if (node.children.size() == 1 && node.meshPath == "N")
	//{
	//	if (node.children[0].meshPath != "N")
	//	{
	//		LoadMeshNode(node.children[0].meshPath, nodeGameObject);

	//		if (node.children[0].resourceMaterialUID != 0)
	//			LoadTexture(node.children[0].resourceMaterialUID, nodeGameObject);
	//	}
	//	return; // TODO: This check could give problems with the animation bones loading (empty nodes with no meshes).
	//}

	if (node.meshPath != "N")
	{
		LoadMeshNode(node.meshPath, nodeGameObject);
		if (node.resourceMaterialUID != 0)
			LoadTexture(node.resourceMaterialUID, nodeGameObject);
	}

	for (int i = 0; i < node.childrenNum; i++)
	{
		LoadNode(node.children[i], nodeGameObject);
	}

}

void MeshImporter::LoadMeshNode(std::string filePath, GameObject* parent)
{
	std::string stringUID = ModuleFiles::S_GetFileName(filePath, false);
	uint UID = std::stoul(stringUID);

	if (!ModuleResourceManager::S_IsResourceCreated(UID))
	{
		ModuleResourceManager::S_CreateResourceMesh(filePath, UID, parent->name);
	}
	
	MeshRenderComponent* meshRender = parent->AddComponent<MeshRenderComponent>();
	meshRender->CreateMesh(UID);

	//TODO: Temporal Solution, Should be improved
	if (meshRender->HasBones())
	{
		parent->DestroyComponent(meshRender);

		SkinnedMeshRenderComponent* skinnedMeshRender = parent->AddComponent<SkinnedMeshRenderComponent>();
		skinnedMeshRender->CreateMesh(UID);
	}
}

void MeshImporter::LoadTexture(uint resourceUID, GameObject* parent)
{
	MaterialComponent* newMaterial = parent->AddComponent<MaterialComponent>();
	newMaterial->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(resourceUID));
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





