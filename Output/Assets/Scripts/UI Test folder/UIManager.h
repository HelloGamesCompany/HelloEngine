#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class UIManager : HelloBehavior
{
public:
	enum class CurrentPanel
	{
		NONE,
		PAUSE,
		MAP
	};

	void Start() override; 
	void Update() override;

	void ContinueGame();

	API_GameObject pausePanel;
	API_GameObject mapPanel;
	API_GameObject HUDPanel;

	CurrentPanel currentPanel;
};

