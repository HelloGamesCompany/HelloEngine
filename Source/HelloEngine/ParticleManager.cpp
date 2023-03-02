#include "Headers.h"
#include "ParticleManager.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "LayerGame.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::Init()
{
}

void ParticleManager::Update()
{
	// EMIT AND UPDATE EMITTER
	for (Emitter* var : EmitterList)
	{
		if(var->component != nullptr)
		var->component->particleProps.position = var->component->_gameObject->transform->GetGlobalPosition();

		var->EmitParticles(var->component->particleProps);

		var->UpdateParticles();
		
	}
	

}

void ParticleManager::Draw()
{

	if (LayerGame::S_IsPlaying())
	{
		Update();

		// DRAW EACH EMITTER

		for (Emitter* var : EmitterList)
		{
			var->Draw();
		}
	}
	
}

void ParticleManager::RemoveEmitterInList(Emitter* emitter)
{

	for (int i = 0; i < EmitterList.size(); i++)
	{
		if (EmitterList[i] == emitter)
		{
			EmitterList.erase(EmitterList.begin() + i);
		}
	}

}
