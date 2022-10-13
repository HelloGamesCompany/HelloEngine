#include "Headers.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "LayerGame.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

ModuleLayers::ModuleLayers()
{
}

ModuleLayers::~ModuleLayers()
{
}

bool ModuleLayers::Start()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");

    for (int i = 0; i < 1; i++)
    {
        MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
    }
    //uint bakerTexture = TextureImporter::ImportTexture("Assets/Baker_house.png");
    uint bakerTexture = TextureImporter::ImportTextureSTBI("Assets/Baker_house.png");

    for (auto& gameObject : gameObjects)
    {
        MeshRenderComponent* rc = nullptr;
        rc = gameObject.second->GetComponent<MeshRenderComponent>();
        if (rc != nullptr)
        {
            rc->GetMesh().textureID = bakerTexture;
        }
    }

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
