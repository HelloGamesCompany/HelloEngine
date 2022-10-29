#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "ImWindow.h"
#include "Layer.h"
#include "LayerGame.h"

enum class ImWindowID
{
	CONFIGURATION,
	ABOUT,
	OPENGL,
	CONSOLE,
	PROJECT,
	HIERARCHY,
	INSPECTOR,
	QUICKSAVE,
	PERFORMANCE,
	GAME,
	SCENE,
	MAX,
};

class LayerEditor :public Layer
{
public:
	LayerEditor();
	~LayerEditor();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void CleanUp() override;

	void SetSelectGameObject(GameObject* g);
	GameObject* GetSelectedGameObject() { return selectedGameObject; }

	void PopUpMessage(std::string message);


private:
	ImWindow* imWindows[(uint)ImWindowID::MAX] = { nullptr };

	LayerGame* game = nullptr;

	GameObject* selectedGameObject = nullptr;

	int gameWidth = 0, gameHeight = 0;

	//Popup message variables
	bool displayPopUp = false;
	std::string popUpMessage = "";
	float messageTime = 2.0f;
	float currentMessageTime = 0.0f;

	friend class ImWindowHierarchy;
};

#endif // !__MODULE_ENGINE_H__