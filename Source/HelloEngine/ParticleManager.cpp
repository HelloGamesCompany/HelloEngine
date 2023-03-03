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

void ParticleManager::Update(Emitter& emitter)
{
	// EMIT AND UPDATE EMITTER
	
	if(emitter.component != nullptr)
		emitter.component->particleProps.position = emitter.component->_gameObject->transform->GetGlobalPosition();

	emitter.EmitParticles(emitter.component->particleProps);

	emitter.UpdateParticles();

}

void ParticleManager::Draw()
{

	for (Emitter* var : EmitterList)
	{

		if (LayerGame::S_IsPlaying() || var->component->playOnScene)
		{
			//UPDATE EACH EMITTER

			Update(*(var));

			// DRAW EACH EMITTER
			
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


