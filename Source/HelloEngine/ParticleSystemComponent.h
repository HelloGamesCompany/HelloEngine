#pragma once
#include "Component.h"
#include "Particle.h"
#include "Emitter.h"

class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* gameObject);

    ~ParticleSystemComponent();

    void OnEnable() override;
    void OnDisable() override;

#ifdef STANDALONE
    void OnEditor() override;
#endif
    void Serialization(json& j) override;
    void DeSerialization(json& j) override;

    Emitter ParticleEmitter;
    ParticleProperties particleProps;    
};

