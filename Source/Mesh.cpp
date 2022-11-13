#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

#define _USE_MATH_DEFINES

#include <math.h>

Mesh::Mesh()
{
	modelMatrix.SetIdentity();
}

Mesh::~Mesh()
{
	std::cout << "Destroy Mesh" << std::endl;
}

void Mesh::InitAsMeshInformation(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;
}

bool Mesh::Update()
{
	if (!draw) return false;
	if (TextureManager::loadedTextures.find(textureID) != TextureManager::loadedTextures.end())
	{
		OpenGLTextureID = TextureManager::BindTexture(textureID);
	}

	modelMatrix = component->_gameObject->transform->GetGlobalMatrix();
	modelMatrix.Transpose();

	return true;
}

void Mesh::InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices, float3 pos, float3 scale)
{
	this->position = position;
	this->scale = scale;

	this->_vertices = new std::vector<Vertex>(vertices);
	this->_indices = new std::vector<uint>(indices);

}

void Mesh::CleanUp()
{	
	RELEASE(_vertices);
	RELEASE(_indices);
}

void Mesh::SetPosition(float3 pos)
{
	this->position = pos;
	_updateMatrix = true;
}

void Mesh::SetScale(float3 s)
{
	this->scale = s;	
	_updateMatrix = true;
}

void Mesh::SetRotation(float3 rot)
{
	this->rotation = rot;
	_updateMatrix = true;
}

void Mesh::SetTransform(float3 pos, float3 s, float3 rot)
{
	this->position = pos;
	this->scale = s;
	this->rotation = rot;
	_updateMatrix = true;
}
