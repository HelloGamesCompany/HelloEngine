#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"

class Scene;

enum class Scenes
{
	MAIN_MENU,
	GAME,
	GAME_OVER,
	TOTAL_SCENES
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);

	~ModuleScene();

	bool Start();

	UpdateStatus PreUpdate();

	UpdateStatus Update();

	UpdateStatus PostUpdate();

	bool ChangeCurrentScene(uint index);

	bool CleanUp();

public:

	Scenes currentScene = Scenes::MAIN_MENU;

	Scene* scenes[(int)Scenes::TOTAL_SCENES] = { nullptr };
};

#endif // !__MODULESCENE_H__