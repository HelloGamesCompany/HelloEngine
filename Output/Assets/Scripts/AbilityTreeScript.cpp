#include "AbilityTreeScript.h"
//MADE BY ALEX GARCIA SELMA
HELLO_ENGINE_API_C AbilityTreeScript* CreateAbilityTreeScript(ScriptToInspectorInterface* script)
{
	AbilityTreeScript* classInstance = new AbilityTreeScript();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Movement", &classInstance->AbilityList1);
	script->AddDragBoxUIButton("AbilityList2", &classInstance->AbilityList2);
	script->AddDragBoxUIButton("AbilityList3", &classInstance->AbilityList3);
	script->AddDragBoxUIButton("AbilityList4", &classInstance->AbilityList4);

	//panels
	script->AddDragBoxGameObject("PanelMovement", &classInstance->PanelAbility1);
	script->AddDragBoxGameObject("PanelAbility2", &classInstance->PanelAbility2);
	script->AddDragBoxGameObject("PanelAbility3", &classInstance->PanelAbility3);
	script->AddDragBoxGameObject("PanelAbility4", &classInstance->PanelAbility4);
	return classInstance;
}

void AbilityTreeScript::Start()
{

}
void AbilityTreeScript::Update()
{
	
	if (AbilityList1.OnHovered())
	{
		PanelAbility1.SetActive(true);
		PanelAbility2.SetActive(false);
		PanelAbility3.SetActive(false);
		PanelAbility4.SetActive(false);
	}

	else if (AbilityList2.OnHovered())
	{
		PanelAbility1.SetActive(false);
		PanelAbility2.SetActive(true);
		PanelAbility3.SetActive(false);
		PanelAbility4.SetActive(false);
	}

	else if (AbilityList3.OnHovered())
	{
		PanelAbility1.SetActive(false);
		PanelAbility2.SetActive(false);
		PanelAbility3.SetActive(true);
		PanelAbility4.SetActive(false);
	}

	else if (AbilityList4.OnHovered())
	{
		PanelAbility1.SetActive(false);
		PanelAbility2.SetActive(false);
		PanelAbility3.SetActive(false);
		PanelAbility4.SetActive(true);
	}
}
