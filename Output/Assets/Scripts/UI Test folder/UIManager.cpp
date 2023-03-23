#include "UIManager.h"
HELLO_ENGINE_API_C UIManager* CreateUIManager(ScriptToInspectorInterface* script)
{
	UIManager* classInstance = new UIManager();
	script->AddDragBoxGameObject("Pause Panel", &classInstance->pausePanel);
	script->AddDragBoxGameObject("Map Panel", &classInstance->mapPanel);
	script->AddDragBoxGameObject("HUD Panel", &classInstance->HUDPanel);

	return classInstance;
}

void UIManager::Start()
{
	currentPanel = CurrentPanel::NONE;
}

void UIManager::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_START) == KeyState::KEY_DOWN)
	{
		if (currentPanel == CurrentPanel::NONE)
		{
			// Call Pause on game
			Time::ChangeTimeScale(0.0f);
			// Show pause menu
			pausePanel.SetActive(true);
			HUDPanel.SetActive(false);
			currentPanel = CurrentPanel::PAUSE;
		}
	
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (currentPanel != CurrentPanel::PAUSE)
		{
			bool hasMap = currentPanel != CurrentPanel::MAP;
			mapPanel.SetActive(hasMap);
			HUDPanel.SetActive(!hasMap);
			currentPanel = hasMap ? CurrentPanel::MAP : CurrentPanel::NONE;
		}
	}
}

void UIManager::ContinueGame()
{
	// Call Pause on game
	Time::ChangeTimeScale(1.0f);

	// Show pause menu
	pausePanel.SetActive(false);
	HUDPanel.SetActive(true);

	currentPanel = CurrentPanel::NONE;

}
