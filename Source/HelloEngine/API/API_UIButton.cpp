#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UIButton.h"
#include "ComponentUIButton.h"

API::API_UIButton::API_UIButton()
{
}

API::API_UIButton::~API_UIButton()
{
}

API::API_GameObject API::API_UIButton::GetGameObject()
{
	if (!_UIButton)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIButton");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_UIButton->GetGameObject());
	return returnGO;
}

bool API::API_UIButton::OnPress()
{
	if (!_UIButton)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
		return false;
	}

	return _UIButton->State == ButtonState::ONPRESS;
}

bool API::API_UIButton::OnHold()
{
	if (!_UIButton)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
		return false;
	}

	return _UIButton->State == ButtonState::ONHOLD;
}

bool API::API_UIButton::OnHovered()
{
	if (!_UIButton)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
		return false;
	}

	return _UIButton->State == ButtonState::HOVERED;
}


ComponentUIButton* API::API_UIButton::GetComponent()
{
	return _UIButton;
}

void API::API_UIButton::SetComponent(ComponentUIButton* component)
{
	_UIButton = component;
}


