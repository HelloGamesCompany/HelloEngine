#include "Headers.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "LayerGame.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "TransformComponent.h"
#include "ModuleResourceManager.h"

// Temporary, Delete Later!
#include "File_Model.h"

ModuleLayers::ModuleLayers()
{
}

ModuleLayers::~ModuleLayers()
{
}

bool ModuleLayers::Init()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");
    return true;
}

bool ModuleLayers::Start()
{
    layers[(uint)LayersID::EDITOR] = editor = new LayerEditor();
    layers[(uint)LayersID::GAME] = new LayerGame();

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (layers[i] && layers[i]->IsEnabled()) layers[i]->Start();
    }

    GameObject* house = MeshImporter::LoadMesh("Assets/FBX/BakerHouse.fbx");

    XMLNode test = Application::Instance()->xml->CreateXML("Assets/ModelFileTest.xml", "Model");

    ModelNode testNode;
    testNode.meshPath = "TestingPath";
    testNode.position = { 0,1,3 };
    testNode.rotation = { 1,2,3 };
    testNode.scale = { 10,15,20 };

    ModelNode testNodeChild1;
    testNodeChild1.name = "Node1";
    testNodeChild1.meshPath = "NodeChild1";
    testNodeChild1.position = { 1,1,1 };
    testNodeChild1.rotation = { 2,2,2 };
    testNodeChild1.scale = { 3,3,3 };

    ModelNode testNodeChild2;
    testNodeChild2.name = "Node2";
    testNodeChild2.meshPath = "NodeChild2";
    testNodeChild2.position = { 2,2,2 };
    testNodeChild2.rotation = { 4,4,4 };
    testNodeChild2.scale = { 6,6,6 };

    testNode.children.push_back(testNodeChild1);
    testNode.children.push_back(testNodeChild2);

    testNode.WriteToXML("TestNode", test.node);

    test.Save();

   /* XMLNode readTest = Application::Instance()->xml->OpenXML("Assets/test.xml");

    ModelNode readTestNode;
    readTestNode.ReadFromXML("TestNode", readTest);

    readTestNode;*/

    return true;
}

UpdateStatus ModuleLayers::PreUpdate()
{
    for (int i = 0; i < deletedGameObjects.size(); i++)
    {
        RELEASE(deletedGameObjects[i]);
    }

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
