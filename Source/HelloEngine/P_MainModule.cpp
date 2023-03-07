#include "Headers.h"
#include "P_MainModule.h"
#include "ParticleSystemComponent.h"

P_MainModule::P_MainModule()
{
}

P_MainModule::~P_MainModule()
{
}

void P_MainModule::OnEditor()
{
	if (ImGui::CollapsingHeader("Particle Main Module")) 
	{
		if (ImGui::DragFloat("Duration", &component->ParticleEmitter.Duration)) {
			component->ParticleEmitter.DurationCpy = component->ParticleEmitter.Duration;
		}
		ImGui::Checkbox("Looping", &component->ParticleEmitter.loop);
		if (ImGui::DragFloat("Start Delay", &component->ParticleEmitter.StartDelay)) {
			component->ParticleEmitter.StartDelayCpy = component->ParticleEmitter.StartDelay;
		}

		ImGui::DragFloat("Particle Life Time", &component->particleProps.Lifetime);

		ImGui::DragFloat3("Begin Scale", &component->particleProps.startsize[0], 0.1f);
		ImGui::DragFloat3("End Scale", &component->particleProps.endsize[0], 0.1f);

		ImGui::DragFloat3("Speed", &component->particleProps.speed[0], 0.1f);
		ImGui::DragFloat3("Speed Variation", &component->particleProps.speedVariation[0], 0.1f);
		ImGui::DragFloat3("Acceleration", &component->particleProps.acceleration[0], 0.1f);
	}

}
