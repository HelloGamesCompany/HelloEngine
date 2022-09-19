#ifndef __SCENE_H__
#define __SCENE_H__

#include "GameObject.h"
#include "List.h"

class PhysBody3D;

class Scene
{
protected:
	int ID = 0;

	Application* _app = nullptr;

	List<GameObject*> gameObjects;

public:
	std::string name = "null";

public:
	// Constructor
	Scene(Application* app);

	// Destructor
	~Scene();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

	void AddGameObject(GameObject* g);

	void DestroyGameObject(GameObject* gameObject);

	int getID()
	{
		return ID;
	}
};

#endif //_SCENE_H_