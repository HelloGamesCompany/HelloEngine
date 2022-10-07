#include "Headers.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "LayerGame.h"

ModuleLayers::ModuleLayers()
{
}

ModuleLayers::~ModuleLayers()
{
}

bool ModuleLayers::Start()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");

    GameObject* go1 = new GameObject(rootGameObject, "go1", "go1");
    GameObject* go2 = new GameObject(rootGameObject, "go2", "go2");

    GameObject* go21 = new GameObject(go2, "go21", "go21");

    go21->AddComponent<MeshRenderComponent>()->InitAsDefaultCube();
    go21->GetComponent<MeshRenderComponent>()->GetMesh().position = { 5.0f, 3.0f, 1.0f };
    go21->GetComponent<MeshRenderComponent>()->GetMesh().scale = { 5.0f, 3.0f, 1.0f };


    layers[(uint)LayersID::EDITOR] = new LayerEditor();
    layers[(uint)LayersID::GAME] = new LayerGame();

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (layers[i] && layers[i]->IsEnabled()) layers[i]->Start();
    }
    return true;
}

UpdateStatus ModuleLayers::PreUpdate()
{
    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (layers[i] && layers[i]->IsEnabled()) layers[i]->PreUpdate();
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::Update()
{
    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (layers[i] && layers[i]->IsEnabled()) layers[i]->Update();
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::PostUpdate()
{

    return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleLayers::DrawLayers()
{
    for (int i = 0; i < (uint)LayersID::EDITOR; i++)
    {
        if (layers[i] && layers[i]->IsEnabled()) layers[i]->PostUpdate();
    }
}

void ModuleLayers::DrawEditor()
{
    if (layers[(uint)LayersID::EDITOR] && layers[(uint)LayersID::EDITOR]->IsEnabled()) layers[(uint)LayersID::EDITOR]->PostUpdate();
}

bool ModuleLayers::CleanUp()
{
    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
       if (layers[i]) layers[i]->CleanUp();
       RELEASE(layers[i]);
    }
    RELEASE(rootGameObject);
    return true;
}

uint ModuleLayers::AddGameObject(GameObject* go)
{
    gameObjects[IDcounter] = go;
    return IDcounter++;
}
