#include "Headers.h"
#include "Emitter.h"
#include "ModuleRenderer3D.h"

Emitter::Emitter()
{
	app = Application::Instance();
	//To-Do: crea pushBack a ParticleManager->EmitterList
	app->renderer3D->particleManger.EmitterList.emplace_back(this);
	
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
	particle.speed.x += particleProps.speedVariation.x * (random.Float() - 0.5f);
	particle.speed.y += particleProps.speedVariation.y * (random.Float() - 0.5f);
	particle.speed.z += particleProps.speedVariation.z * (random.Float() - 0.5f);

	// Acceleration
	particle.acceleration = particleProps.acceleration;

	// Color
	particle.startColor = particleProps.startColor;
	particle.endColor = particleProps.endColor;

	particle.Lifetime = particleProps.Lifetime;
	particle.remainingLifetime = particleProps.Lifetime;

	currentparticle--;

}

void Emitter::Draw(Shader* shader, Quat BBrot)
{
}

void Emitter::UpdateParticles()
{
	for (int i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active)
			continue;

		if (ParticleList[i].remainingLifetime <= 0.0f)
		{
			ParticleList[i].Active = false;
			continue;
		}

		//Compute all the calculus needed to move the particles

		//Remaining life minus dt
		ParticleList[i].remainingLifetime -= EngineTime::GameDeltaTime();

		if (ParticleList[i].remainingLifetime > 0.0f)
		{
			// velocity = acceleration * dt
			ParticleList[i].speed += ParticleList[i].acceleration * EngineTime::GameDeltaTime();

			// pos += velocity * dt
			ParticleList[i].position += ParticleList[i].speed * EngineTime::GameDeltaTime();
		}
	}
}


