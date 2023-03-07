#pragma once
#include "Component.h"
#include "Particle.h"
#include "Emitter.h"
#include "P_Module.h"
#include "Mesh.h"

class Application;
class ResourceMesh;
class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* gameObject);

    ~ParticleSystemComponent();

    void CreateEmitterMesh(uint resourceUID);

    Mesh& GetEmitterMesh();

    void DestroyEmitterMesh();

    void OnEnable() override;
    void OnDisable() override;

#ifdef STANDALONE
    void OnEditor() override;

    void MarkAsDead() override;
    void MarkAsAlive() override;
#endif
    void Serialization(json& j) override;
    void DeSerialization(json& j) override;


public:
    bool playOnScene = false;
private:
    
    ResourceMesh* _resource;
    uint _resourceUID;
    Application* app;
    Emitter ParticleEmitter;
    ParticleProperties particleProps;   

    //Particle System Modules
    std::vector<P_Module*> ParticleModules;

    friend class ParticleManager;
    friend class P_Module;
    friend class P_MainModule;
};

