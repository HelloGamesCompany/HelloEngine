#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"
#include "glmath.h"

class Radios;
class Follower;

class SceneGame :  public Scene
{
public:

	Radios* radioManager = nullptr;

	Follower* follower = nullptr;

public:
	SceneGame(Application* app);

	~SceneGame();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

};

#endif // !__SCENEGAME_H__
