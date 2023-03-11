#include "Headers.h"
#include "LayerUI.h"
#include "ComponentUI.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerGame.h"

std::map<uint, ComponentUI*> LayerUI::_componentUIs;

LayerUI::LayerUI()
{
}

LayerUI::~LayerUI()
{
}

void LayerUI::Start()
{
}

void LayerUI::PreUpdate()
{
	if (!LayerGame::S_IsPlaying())
		return;

	// Update all Component UIs
	for (auto& uiComponent : _componentUIs)
	{
		uiComponent.second->InputUpdate();
	}
}

void LayerUI::Update()
{
}

void LayerUI::PostUpdate()
{
}

void LayerUI::CleanUp()
{
}

void LayerUI::AddUI(uint UID, ComponentUI* component)
{
	_componentUIs[UID] = component;
}

void LayerUI::RemoveUI(uint gameObjectID)
{
	_componentUIs.erase(gameObjectID);
}
