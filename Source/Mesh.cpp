#include "Headers.h"
#include "Mesh.h"

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
}

Mesh::~Mesh()
{
}

void Mesh::InitAsCube(float3 position, float3 transform)
{
	this->position = position;
	this->transform = transform;

	_originalShape = new std::vector<float3>
	{
		float3(-0.5,	 0.5,	 0.5),
		float3(-0.5,	-0.5,	 0.5),
		float3( 0.5,	 0.5,	 0.5),
		float3( 0.5,	-0.5,	 0.5),
		float3(-0.5,	 0.5,	-0.5),
		float3(-0.5,	-0.5,	-0.5),
		float3( 0.5,	 0.5,	-0.5),
		float3( 0.5,	-0.5,	-0.5),
	};

	_vertices = new std::vector<float3>(*_originalShape);

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

void Mesh::InitAsSphere(float3 position, float3 transform)
{
	this->position = position;
	this->transform = transform;

	int rings = 12;
	int sectors = 24;
	int radius = 1;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	_originalShape = new std::vector<float3>(rings * sectors);
	_vertices = new std::vector<float3>(rings * sectors);
	_indices = new std::vector<uint>(rings * sectors * 4);

	int counter = 0;

	for (int r = 0; r < rings; r++) for (int s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		_originalShape->at(counter).x = x * radius;
		_originalShape->at(counter).y = y * radius;
		_originalShape->at(counter).z = z * radius;
		counter++;
		if (r < rings - 1) push_indices(*_indices, sectors, r, s);
	}
}

void Mesh::Update()
{
	// TODO: For performance improvement, this should be done inside a vertex shader.
	for (int i = 0; i < _vertices->size(); i++)
	{
		_vertices->at(i).x = (_originalShape->at(i).x * transform.x) + position.x;
		_vertices->at(i).y = (_originalShape->at(i).y * transform.y) + position.y;
		_vertices->at(i).z = (_originalShape->at(i).z * transform.z) + position.z;
	}
}

void Mesh::CleanUp()
{	
	RELEASE(_vertices);
	RELEASE(_indices);
	RELEASE(_originalShape);
}
