#include "Headers.h"
#include "Particle.h"

Particle::Particle():startColor(0.0f, 1.0f, 0.0f, 1.0f), endColor(0.0f,1.0f,0.0f,0.0f)
{
	 
	this->position = { 0.0f,0.0f,0.0f };

	this->scale = { 1.0f,1.0f,1.0f };

	this->Lifetime = 1.0f;

	this->remainingLifetime = 0.0f;

	this->rotation = { 0.0f,0.0f,0.0f };

	this->transformMat = float4x4::identity;

	//this->rotationSpeed = 0.0f;

	this->startSize = {5.0f,5.0f,5.0f};

	this->endSize = { 5.0f,5.0f,5.0f };

	this->speed = { 0.0f,0.0f,0.0f };

	this->acceleration = { 0.0f,0.0f,0.0f };

	this->Active = false;
	
}

Particle::~Particle()
{
}

void Particle::SetTransformMatrix()
{
	float x = rotation.x * DEGTORAD;
	float y = rotation.y * DEGTORAD;
	float z = rotation.z * DEGTORAD;

	Quat q = Quat::FromEulerXYZ(x, y, z);

	transformMat = float4x4::FromTRS(position, q, scale).Transposed();
}
