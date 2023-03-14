#include "Headers.h"
#include "ParticleManager.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "LayerGame.h"
#include "BillBoardComponent.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::Init()
{
}

void ParticleManager::Draw()
{

	for (Emitter* emitter : EmitterList)
	{

		if ((LayerGame::S_IsPlaying() && emitter->component->GetPlayOnGame()) || emitter->component->GetPlayOnScene())
		{
			//UPDATE EACH EMITTER
			if (!emitter->loop) {
				if (emitter->Duration <= 0) {
					emitter->stop = true;				
				}
				else {
					emitter->stop = false;
				}
			}
			else {
				emitter->stop = false;
			}

			if (emitter->component != nullptr)
				emitter->component->particleProps.position = emitter->component->_gameObject->transform->GetGlobalPosition();

			/*if (emitter->enableEmissionModule)
			{
				int particlesToEmit = 0;
				if (LayerGame::S_IsPlaying()) {
					particlesToEmit = EngineTime::GameDeltaTime() * emitter->ParticlesPerSecond;
				}
				else {
					particlesToEmit = EngineTime::EngineTimeDeltaTime() * emitter->ParticlesPerSecond;
				}
				
				emitter->ParticleCount = particlesToEmit;

				if (emitter->ParticleCount > emitter->ParticlesPerSecond) {
					emitter->ParticleCount = emitter->ParticlesPerSecond;
				}

				for (int i = 0; i < emitter->ParticleCount; i++) {
					emitter->EmitParticles(emitter->component->particleProps);
				}

				emitter->ParticleCount = 0;

			}*/
			//else{
				if (!emitter->stop) {
					emitter->EmitParticles(emitter->component->particleProps);
				}
			//}

			emitter->UpdateParticles();

			// DRAW EACH EMITTER
			
			//var->Draw();
			
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

void ParticleManager::RemoveAllEmitters()
{
	EmitterList.clear();
}


