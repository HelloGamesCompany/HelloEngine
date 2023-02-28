#include "Headers.h"
#include "ParticleSystemComponent.h"
#include "ParticleManager.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::PARTICLE_SYSTEM;
	//To-Do: crea pushBack a ParticleManager->EmitterList
}

ParticleSystemComponent::~ParticleSystemComponent()
{
}

void ParticleSystemComponent::OnEnable()
{
}

void ParticleSystemComponent::OnDisable()
{
}

void ParticleSystemComponent::OnEditor()
{
}

void ParticleSystemComponent::Serialization(json& j)
{
}

void ParticleSystemComponent::DeSerialization(json& j)
{
}
