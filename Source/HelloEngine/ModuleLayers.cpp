#include "Headers.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "LayerGame.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "TransformComponent.h"
#include "ModuleResourceManager.h"

#include "ModuleInput.h"
#include "ModuleWindow.h"

LayerGame* ModuleLayers::game = nullptr;
GameObject* ModuleLayers::rootGameObject = nullptr;
LayerEditor* ModuleLayers::editor = nullptr;
std::map<uint, GameObject*> ModuleLayers::gameObjects;
Layer* ModuleLayers::_layers[(uint)LayersID::MAX] = { nullptr };
bool ModuleLayers::_requestScene = false;
std::string ModuleLayers:: _requestScenePath = "null";
std::string ModuleLayers::_sceneBeginPath = "null";
std::vector<GameObject*> ModuleLayers::_deletedGameObjects;

ModuleLayers::ModuleLayers()
{
}

ModuleLayers::~ModuleLayers()
{
}

bool ModuleLayers::Start()
{
    _layers[(uint)LayersID::EDITOR] = new LayerEditor();
    _layers[(uint)LayersID::GAME] = new LayerGame();

    game = (LayerGame*)_layers[(uint)LayersID::GAME];
    editor = (LayerEditor*)_layers[(uint)LayersID::EDITOR];

    // Create Root GameObject (Scene)
    XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

    _sceneBeginPath = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

    if (!ModuleResourceManager::S_DeserializeScene(_sceneBeginPath))
    {
        rootGameObject = new GameObject(nullptr, "Root", "None");
        XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

        std::string newDir = ASSETS_PATH;

        // Change Title
        newDir += rootGameObject->name + ".HScene";

        std::string scenePath = " -- CurrentScene: " + newDir;

        ModuleWindow::S_AddTitleExtraInfo(scenePath);

        sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value(newDir.c_str());
        
        ModuleResourceManager::S_SerializeScene(rootGameObject);
    }

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled()) 
            _layers[i]->Start();
    }

    return true;
}

UpdateStatus ModuleLayers::PreUpdate()
{
    if(_requestScene)
    {
        if(!ModuleResourceManager::S_DeserializeScene(_requestScenePath))
            _requestScenePath = "null";

        _requestScene = false;
    }

    for (int i = 0; i < _deletedGameObjects.size(); i++)
    {
        RELEASE(_deletedGameObjects[i]);
    }
    _deletedGameObjects.clear();

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled()) 
            _layers[i]->PreUpdate();
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::Update()
{
    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled()) 
            _layers[i]->Update();
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleLayers::S_DrawLayers()
{
    for (int i = 0; i < (uint)LayersID::EDITOR; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled())
            _layers[i]->PostUpdate();
    }
}

void ModuleLayers::S_DrawEditor()
{
    if (_layers[(uint)LayersID::EDITOR] && _layers[(uint)LayersID::EDITOR]->IsEnabled()) 
        _layers[(uint)LayersID::EDITOR]->PostUpdate();
}

bool ModuleLayers::CleanUp()
{
    XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

    std::string configScene = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

    if (configScene == "null")
    {
        std::string newDir = ASSETS_PATH;

        newDir += rootGameObject->name + ".HScene";

        sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value(newDir.c_str());
    }

    ModuleResourceManager::S_SerializeScene(rootGameObject);

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
       if (_layers[i]) 
           _layers[i]->CleanUp();
       
       RELEASE(_layers[i]);
    }
   
    RELEASE(rootGameObject);
    return true;
}

uint ModuleLayers::S_AddGameObject(GameObject* go, uint ID)
{
    ID = ID == 0 ? HelloUUID::GenerateUUID() : ID;
    gameObjects[ID] = go;
    return ID;
}

void ModuleLayers::S_RequestLoadScene(const std::string& scenePath)
{
    _requestScenePath = scenePath;

    _requestScene = true;
}
