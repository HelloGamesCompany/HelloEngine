#include "Headers.h"
#include "API_ParticleSystem.h"
#include "ModuleLayers.h"
#include "ParticleSystemComponent.h"

API::API_ParticleSystem::API_ParticleSystem()
{
}

API::API_ParticleSystem::~API_ParticleSystem()
{
}

API::API_GameObject API::API_ParticleSystem::GetGameObject()
{
	if (_particleSystem == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return *ModuleLayers::emptyAPIGameObject;
	}

	API_GameObject goToReturn;
	goToReturn.SetGameObject(_particleSystem->GetGameObject());

	return goToReturn;
}

void API::API_ParticleSystem::Play()
{
	if (_particleSystem == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return;
	}
	_particleSystem->SetPlayOnGame(true);

}

void API::API_ParticleSystem::Pause()
{
	if (_particleSystem == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return;
	}
	_particleSystem->SetPlayOnGame(false);
}

void API::API_ParticleSystem::Stop()
{
	if (_particleSystem == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return;
	}
	
	if (_particleSystem->GetPlayOnGame())
	{
		_particleSystem->SetPlayOnGame(false);

		_particleSystem->GetParticleSystemEmitter().ResetEmitter();
	}
}

ParticleSystemComponent* API::API_ParticleSystem::GetComponent()
{

	return _particleSystem;

}

void API::API_ParticleSystem::SetComponent(ParticleSystemComponent* component)
{

	_particleSystem = component;

}
