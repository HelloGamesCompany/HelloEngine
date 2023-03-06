#include "Headers.h"
#include "Emitter.h"
#include "ModuleRenderer3D.h"
#include "InstanceRenderer.h"
#include "LayerGame.h"
#include "ParticleSystemComponent.h"

Emitter::Emitter()
{
	app = Application::Instance();
	app->renderer3D->particleManager.EmitterList.emplace_back(this);

	BBRotAroundZ = Quat::identity;

	SetParticlePoolSize(1);
	
}

Emitter::~Emitter()
{
	if (_meshID != -1)
	{
		app->renderer3D->particleManager.RemoveEmitterInList(this);
	}

}

void Emitter::SetParticlePoolSize(uint size)
{
	currentparticle = size - 1;

	ParticleList.resize(size);
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

	particle.SetTransformMatrix(BBRotAroundZ);

	currentparticle--;

}

void Emitter::Draw()
{

	//manager->Draw();

}

void Emitter::UpdateParticles(Quat rotation)
{
	for (int i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active)
			continue;

		if (ParticleList[i].remainingLifetime <= 0.0f)
		{
			Mesh& meshReference = manager->GetMap()[ParticleList[i]._instanceID];

			meshReference.draw = false;

			ParticleList[i].Active = false;
			continue;
		}

		if (LayerGame::S_IsPlaying() == false && component->playOnScene)
		{
			//Compute all the calculus needed to move the particles

			//Remaining life minus dt
			ParticleList[i].remainingLifetime -= EngineTime::EngineTimeDeltaTime();


			// velocity = acceleration * dt
			ParticleList[i].speed += ParticleList[i].acceleration * EngineTime::EngineTimeDeltaTime();


			// pos += velocity * dt
			ParticleList[i].position += ParticleList[i].speed * EngineTime::EngineTimeDeltaTime();
		}
		else 
		{
			//Compute all the calculus needed to move the particles

			//Remaining life minus dt
			ParticleList[i].remainingLifetime -= EngineTime::GameDeltaTime();


			// velocity = acceleration * dt
			ParticleList[i].speed += ParticleList[i].acceleration * EngineTime::GameDeltaTime();


			// pos += velocity * dt
			ParticleList[i].position += ParticleList[i].speed * EngineTime::GameDeltaTime();
		}
	
		float life = ParticleList[i].remainingLifetime / ParticleList[i].Lifetime;

		ParticleList[i].scale = Lerp(ParticleList[i].endSize, ParticleList[i].startSize, life);

		BBRotAroundZ = rotation; 

		ParticleList[i].SetTransformMatrix(rotation);

		manager = app->renderer3D->renderManager.GetRenderManager(_meshID);
		
		Mesh& meshReference = manager->GetMap()[ParticleList[i]._instanceID];

		meshReference.draw = true;

		meshReference.modelMatrix = ParticleList[i].transformMat;

	}
}


