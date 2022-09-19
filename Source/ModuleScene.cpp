#include "ModuleScene.h"
#include "Scene.h"
#include "SceneGame.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) :Module(app, start_enabled)
{
    scenes[(int)Scenes::GAME] = new SceneGame(app);
}

ModuleScene::~ModuleScene()
{
    for (int i = 0; i < (int)Scenes::TOTAL_SCENES; i++)
    {
        if (scenes[i] != nullptr)
        {
            RELEASE(scenes[i]);
        }
    }
}

bool ModuleScene::Start()
{
    currentScene = Scenes::GAME;

    scenes[(int)currentScene]->Start();

    return true;
}

UpdateStatus ModuleScene::PreUpdate()
{
    scenes[(int)currentScene]->PreUpdate();

    return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
    scenes[(int)currentScene]->Update();

    return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
    scenes[(int)currentScene]->PostUpdate();

    return UPDATE_CONTINUE;
}

bool ModuleScene::ChangeCurrentScene(uint index)
{
    //scenes[(int)currentScene]->CleanUp();

    //currentScene = (Scenes)index;

    //scenes[(int)currentScene]->Start();

    return true;
}

bool ModuleScene::CleanUp()
{
    for (int i = 0; i < (int)Scenes::TOTAL_SCENES; i++)
    {
        if (scenes[i] != nullptr)
        {
            scenes[i]->CleanUp();
        }
    }

    return true;
}
