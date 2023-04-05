#include "HUD_Shield_Indicator.h"
HELLO_ENGINE_API_C HUD_Shield_Indicator* CreateHUD_Shield_Indicator(ScriptToInspectorInterface* script)
{
	HUD_Shield_Indicator* classInstance = new HUD_Shield_Indicator();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Shield_GO", &classInstance->Shield);

	return classInstance;
}

void HUD_Shield_Indicator::Start()
{
	Shield.SetActive(false);
}
void HUD_Shield_Indicator::Update()
{
	/*if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_DOWN && shield_Enable == false)
	{
		shield_Enable = true;
	}
	else if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_DOWN && shield_Enable == true)
	{
		shield_Enable = false;
	}

	isShieldEnable(shield_Enable);*/

}

void HUD_Shield_Indicator::isShieldEnable(bool Shieldpowerup_True)
{
	if (Shieldpowerup_True == true)
	{
		Shield.SetActive(true);
	}

	else
	{
		Shield.SetActive(false);
	}

}
