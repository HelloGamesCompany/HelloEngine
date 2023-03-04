#pragma once
#include "Component.h"
#include "Particle.h"
#include "Emitter.h"
#include "P_MainModule.h"

class Application;
class ResourceMesh;
class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* gameObject);

    ~ParticleSystemComponent();

    void CreateEmitterMesh(uint resourceUID);

    void DestroyEmitterMesh();

    void OnEnable() override;
    void OnDisable() override;

#ifdef STANDALONE
    void OnEditor() override;
#endif
    void Serialization(json& j) override;
    void DeSerialization(json& j) override;


private:
    
    ResourceMesh* _resource;
    uint _resourceUID;
    Application* app;
    Emitter ParticleEmitter;
    ParticleProperties particleProps;   
    bool playOnScene = false;

    //Particle System Modules
    P_MainModule mainModule;

    friend class ParticleManager;
    friend class P_MainModule;
};

