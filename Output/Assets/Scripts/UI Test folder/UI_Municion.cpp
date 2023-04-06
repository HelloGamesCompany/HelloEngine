#include "UI_Municion.h"
HELLO_ENGINE_API_C UI_Municion* CreateUI_Municion(ScriptToInspectorInterface* script)
{
	UI_Municion* classInstance = new UI_Municion();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Infinit munition on", &classInstance->text_munition_InfinitWeapon_On);
	script->AddDragBoxGameObject("Infinit munition off", &classInstance->text_munition_InfinitWeapon_Off);

	script->AddDragBoxUIText("Normal munition on", &classInstance->text_munition_Normal_On);
	script->AddDragBoxUIText("Normal munition off", &classInstance->text_munition_Normal_Off);
	script->AddDragBoxUIText("variableName on", &classInstance->text_munition_Special_On);
	script->AddDragBoxUIText("variableName off", &classInstance->text_munition_Special_Off);
	return classInstance;
}

void UI_Municion::Start()
{

	text_munition_Normal_On.GetGameObject().GetMeshRenderer().SetActive(false);
	text_munition_Normal_Off.GetGameObject().GetMeshRenderer().SetActive(false);
	text_munition_Special_On.GetGameObject().GetMeshRenderer().SetActive(false);
	text_munition_Special_Off.GetGameObject().GetMeshRenderer().SetActive(false);
	
	text_munition_InfinitWeapon_On.SetActive(true);
	text_munition_InfinitWeapon_Off.SetActive(false);
	text_munition_Normal_On.GetGameObject().SetActive(false);
	text_munition_Normal_Off.GetGameObject().SetActive(false);
	text_munition_Special_On.GetGameObject().SetActive(false);
	text_munition_Special_Off.GetGameObject().SetActive(false);
}

void UI_Municion::Update()
{

}



void UI_Municion::GetMunitionInfinit(bool Weaponactive)
{

	if (Weaponactive == true)
	{
		text_munition_InfinitWeapon_On.SetActive(true);
		text_munition_InfinitWeapon_Off.SetActive(false);
	}

	if (Weaponactive == false)
	{
		text_munition_InfinitWeapon_On.SetActive(false);
		text_munition_InfinitWeapon_Off.SetActive(true);
	}

}

void UI_Municion::GetMunitionNormal(normalWeapon_Type WeaponType,int munition_Normal, bool Weaponactive)
{

	switch (WeaponType)
	{
	case normalWeapon_Type::NONE:
		text_munition_Normal_On.SetText(" ");
		text_munition_Normal_Off.SetText(" ");
		break;
	case normalWeapon_Type::AUTO:
		text_munition_Normal_On.SetText(std::to_string(munition_Normal).c_str());
		text_munition_Normal_Off.SetText(std::to_string(munition_Normal).c_str());
		break;
	case normalWeapon_Type::BURST:
		text_munition_Normal_On.SetText(std::to_string(munition_Normal).c_str());
		text_munition_Normal_Off.SetText(std::to_string(munition_Normal).c_str());
		break;
	case normalWeapon_Type::SEMI:
		text_munition_Normal_On.SetText(std::to_string(munition_Normal).c_str());
		text_munition_Normal_Off.SetText(std::to_string(munition_Normal).c_str());
		break;
	case normalWeapon_Type::SHOTGUN:
		text_munition_Normal_On.SetText(std::to_string(munition_Normal).c_str());
		text_munition_Normal_Off.SetText(std::to_string(munition_Normal).c_str());
		break;
	case normalWeapon_Type::REVOLVER:
		text_munition_Normal_On.SetText(std::to_string(munition_Normal).c_str());
		text_munition_Normal_Off.SetText(std::to_string(munition_Normal).c_str());
		break;
	default:
		break;
	}

	if (Weaponactive == true)
	{
		text_munition_Normal_On.GetGameObject().SetActive(true);
		text_munition_Normal_Off.GetGameObject().SetActive(false);
	}
	
	if (Weaponactive == false)
	{
		text_munition_Normal_On.GetGameObject().SetActive(false);
		text_munition_Normal_Off.GetGameObject().SetActive(true);
	}
}

void UI_Municion::GetMunitionSpecial(specialWeapon_Type WeaponType, int munition_Special, bool Weaponactive)
{
	switch (WeaponType)
	{
	case specialWeapon_Type::NONE:
		text_munition_Special_On.SetText(" ");
		text_munition_Special_Off.SetText(" ");
		break;
	case specialWeapon_Type::FLAMETHROWER:
		text_munition_Special_On.SetText(std::to_string(munition_Special).c_str());
		text_munition_Special_Off.SetText(std::to_string(munition_Special).c_str());
		break;
	case specialWeapon_Type::RICOCHET:
		text_munition_Special_On.SetText(std::to_string(munition_Special).c_str());
		text_munition_Special_Off.SetText(std::to_string(munition_Special).c_str());
		break;
	default:
		break;
	}

	if (Weaponactive == true)
	{
		text_munition_Special_On.GetGameObject().SetActive(true);
		text_munition_Special_Off.GetGameObject().SetActive(false);
	}

	if (Weaponactive == false)
	{
		text_munition_Special_On.GetGameObject().SetActive(false);
		text_munition_Special_Off.GetGameObject().SetActive(true);
	}

}
