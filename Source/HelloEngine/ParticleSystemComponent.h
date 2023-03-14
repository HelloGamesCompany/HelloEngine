#pragma once
#include "Component.h"
#include "Particle.h"
#include "Emitter.h"
#include "P_Module.h"
#include "Mesh.h"

class Application;
class ResourceMesh;
class ResourceTexture;

class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* gameObject);

    ~ParticleSystemComponent();

    void CreateEmitterMesh(uint resourceUID);

    Mesh& GetEmitterMesh();

    void DestroyEmitterMesh();

    void ChangeEmitterMeshTexture(ResourceTexture* resource);

    void OnEnable() override;
    void OnDisable() override;

    void OnEditor() override;

    void DestroyEmitterMeshTexture();

    void StopEmitter();

    void MarkAsDead() override;
    void MarkAsAlive() override;

    void Serialization(json& j) override;
    void DeSerialization(json& j) override;

    bool GetPlayOnScene() { return playOnScene; }

    bool GetPauseOnScene() { return pauseOnScene; }

    bool GetPlayOnGame() { return playOnGame; }

    Emitter GetParticleSystemEmitter() { return ParticleEmitter; }

    void SetPlayOnGame(bool playongame);

private:

    void SetPlayOnScene(bool playonscene);
    void SetPauseOnScene(bool pauseonscene);
    
private:
    //Only for testing with particles on Scene
    bool playOnScene = false;
    //Play Particles on Game
    bool playOnGame = false;
    bool pauseOnScene = false;
    
    ResourceMesh* _resource = nullptr;
    ResourceTexture* _resourceText = nullptr;
    //Mark As Alive Use
    uint _resourceUID;
    uint _resourceTextUID;
    Application* app;
    Emitter ParticleEmitter;
    ParticleProperties particleProps;   

    //Particle System Modules
    std::vector<P_Module*> ParticleModules;

    friend class Emitter;
    friend class ParticleManager;
    friend class P_Module;
    friend class P_MainModule;
    friend class P_EmissionModule;
    friend class ModuleRenderer3D;
};

