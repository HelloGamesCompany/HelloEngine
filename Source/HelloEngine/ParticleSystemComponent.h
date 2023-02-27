#pragma once
#include "Component.h"
class ParticleSystemComponent :
    public Component
{

    ParticleSystemComponent();

    ~ParticleSystemComponent();

    void OnEnable() override;
    void OnDisable() override;

    void OnEditor() override;

    
};

