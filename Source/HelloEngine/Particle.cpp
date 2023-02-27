#include "Headers.h"
#include "Particle.h"

Particle::Particle():startColor(0.0f, 1.0f, 0.0f, 1.0f), endColor(0.0f,1.0f,0.0f,0.0f)
{
	 
	this->position = { 0.0f,0.0f,0.0f };

	this->scale = { 1.0f,1.0f,1.0f };

	this->Lifetime = 1.0f;

	this->remainingLifetime = 0.0f;

	this->rotation = 180.0f;

	//this->rotationSpeed = 0.0f;

	this->startSize = {5.0f,5.0f,5.0f};

	this->endSize = { 5.0f,5.0f,5.0f };

	this->speed = { 0.0f,0.0f,0.0f };

	this->Active = false;
	
}

Particle::~Particle()
{
}
