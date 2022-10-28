#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"

#define _USE_MATH_DEFINES

#include <math.h>

inline void push_indices(std::vector<uint>& indices, int sectors, int r, int s) 
{
	int curRow = r * sectors;
	int nextRow = (r + 1) * sectors;
	int nextS = (s + 1) % sectors;

	indices.push_back(curRow + s);
	indices.push_back(nextRow + s);
	indices.push_back(nextRow + nextS);

	indices.push_back(curRow + s);
	indices.push_back(nextRow + nextS);
	indices.push_back(curRow + nextS);
}


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

void Mesh::InitAsCube(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;

	_vertices = new std::vector<Vertex>(8);

	_vertices->at(0).position = float3(-0.5, 0.5, 0.5);
	_vertices->at(0).normals = float3(0.0, 0.0, 1.0);

	_vertices->at(1).position = float3(-0.5, -0.5, 0.5);
	_vertices->at(1).normals = float3(0.0, 0.0, 1.0);

	_vertices->at(2).position = float3(0.5, 0.5, 0.5);
	_vertices->at(2).normals = float3(0.0, 0.0, 1.0);

	_vertices->at(3).position = float3(0.5, -0.5, 0.5);
	_vertices->at(3).normals = float3(0.0, 0.0, 1.0);

	_vertices->at(4).position = float3(-0.5, 0.5, -0.5);
	_vertices->at(4).normals = float3(0.0, 0.0, -1.0);

	_vertices->at(5).position = float3(-0.5, -0.5, -0.5);
	_vertices->at(5).normals = float3(0.0, 0.0, -1.0);

	_vertices->at(6).position = float3(0.5, 0.5, -0.5);
	_vertices->at(6).normals = float3(0.0, 0.0, -1.0);

	_vertices->at(7).position = float3(0.5, -0.5, -0.5);
	_vertices->at(7).normals = float3(0.0, 0.0, -1.0);

	_indices = new std::vector<uint>
	{
	  0, 2, 3, 0, 3, 1,
	  2, 6, 7, 2, 7, 3,
	  6, 4, 5, 6, 5, 7,
	  4, 0, 1, 4, 1, 5,
	  0, 4, 6, 0, 6, 2,
	  1, 5, 7, 1, 7, 3,
	};
}

void Mesh::InitAsSphere(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;

	int rings = 12;
	int sectors = 24;
	int radius = 1;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	_vertices = new std::vector<Vertex>(rings * sectors);
	_indices = new std::vector<uint>(rings * sectors * 4);

	int counter = 0;

	for (int r = 0; r < rings; r++) for (int s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		_vertices->at(counter).position.x = x * radius;
		_vertices->at(counter).position.y = y * radius;
		_vertices->at(counter).position.z = z * radius;
		counter++;
		if (r < rings - 1) push_indices(*_indices, sectors, r, s);
	}
}

bool Mesh::Update()
{
	if (!draw) return false;
	if (TextureManager::loadedTextures.find(textureID) != TextureManager::loadedTextures.end())
	{
		OpenGLTextureID = TextureManager::BindTexture(textureID);
	}

	if (!_updateMatrix) return true;

	modelMatrix.SetIdentity();

	math::Quat rot = rot.FromEulerXYZ(math::DegToRad(rotation.x), math::DegToRad(rotation.y), math::DegToRad(rotation.z));
	rot.Normalize();

	modelMatrix = float4x4::FromTRS(position, rot, scale);

	modelMatrix.Transpose();

	_updateMatrix = false;
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
