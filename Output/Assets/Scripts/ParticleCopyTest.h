#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ParticleCopyTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_ParticleSystem particleSystem;
	API_ParticleSystem myParticles;
};

