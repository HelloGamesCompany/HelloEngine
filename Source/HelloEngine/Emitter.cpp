#include "Headers.h"
#include "Emitter.h"

Emitter::Emitter()
{

	
}

Emitter::~Emitter()
{
}

void Emitter::SetParticlePoolSize(uint size)
{
	currentparticle = size - 1;

	ParticleList.resize(size);
}

void Emitter::InitBuffers()
{
}

void Emitter::EmitParticles(ParticleProperties& particleProps)
{

	if (currentparticle <= 0)
	{
		currentparticle = ParticleList.size() - 1;
	}

	Particle& particle = ParticleList[currentparticle];
	particle.Active = true;
	particle.position = particleProps.position;
	particle.startSize = particleProps.startsize + particleProps.sizevariation * (random.Float() - 0.5f); //Random number between -0.5 / 0.5
	particle.endSize = particleProps.endsize;

	//text = particleProps.texture;
	// Velocity
	particle.speed = particleProps.speed;
	particle.speed.x += particleProps.acceleration.x * (random.Float() - 0.5f);
	particle.speed.y += particleProps.acceleration.y * (random.Float() - 0.5f);
	particle.speed.z += particleProps.acceleration.z * (random.Float() - 0.5f);

	// Color
	particle.startColor = particleProps.startColor;
	particle.endColor = particleProps.endColor;

	particle.Lifetime = particleProps.Lifetime;
	particle.remainingLifetime = particleProps.Lifetime;

	//ListIndex = --ListIndex % ParticleList.size();

	currentparticle--;
	
}

void Emitter::Draw(Shader* shader, Quat BBrot)
{
}

void Emitter::UpdateParticles()
{
}


