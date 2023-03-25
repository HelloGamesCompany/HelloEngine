#include "DashUI.h"
HELLO_ENGINE_API_C DashUI* CreateDashUI(ScriptToInspectorInterface* script)
{
	DashUI* classInstance = new DashUI();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
	script->AddDragBoxTextureResource("Text_Dash_on", &classInstance->Texture_Dash_on);
	script->AddDragBoxTextureResource("Text_Dash_off", &classInstance->Texture_Dash_off);
	script->AddDragFloat("Cooldown", &classInstance->cooldownValue);
	script->AddDragFloat("CooldownBetween", &classInstance->coolDownBetweenDashValue);
	return classInstance;
}

void DashUI::Start()
{
	activeDash = true;
	cooldown = 0;
	Dash();
}

void DashUI::Update()
{

	cooldown += Time::GetDeltaTime();

	if (Input::GetKey(KeyCode::KEY_B) == KeyState::KEY_DOWN)
	{
		if (activeDash == true)
		{
			if (cooldownBetweenDash >= coolDownBetweenDashValue) {
				activeDash = false;
				cooldown = 0;
				Dash();
			}
		}
	}

	if (activeDash == true)
	{
		cooldownBetweenDash += Time::GetDeltaTime();
	}

	if (cooldown >= cooldownValue) {
		cooldown = 0;
		if (activeDash == false)
		{
			activeDash = true;
			cooldownBetweenDash = 0;
			Dash();
		}
	}
}

void DashUI::Dash()
{
	if (activeDash == false)
	{
		gameObject.GetMaterialCompoennt().ChangeAlbedoTexture(Texture_Dash_off);
	}

	if (activeDash == true)
	{
		gameObject.GetMaterialCompoennt().ChangeAlbedoTexture(Texture_Dash_on);
	}
}
