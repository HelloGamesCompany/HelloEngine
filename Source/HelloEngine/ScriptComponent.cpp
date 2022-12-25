#include "Headers.h"
#include "ScriptComponent.h"
#include "ModuleLayers.h"
#include "LayerGame.h"

ScriptComponent::ScriptComponent(GameObject* go) : Component(go)
{
	Application::Instance()->layers->game->AddScriptComponent(this);
}

ScriptComponent::~ScriptComponent()
{
	Application::Instance()->layers->game->RemoveScriptComponent(this);
}
