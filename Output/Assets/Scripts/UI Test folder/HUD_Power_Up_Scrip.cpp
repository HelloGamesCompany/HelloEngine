#include "HUD_Power_Up_Scrip.h"
HELLO_ENGINE_API_C HUD_Power_Up_Scrip* CreateHUD_Power_Up_Scrip(ScriptToInspectorInterface* script)
{
	HUD_Power_Up_Scrip* classInstance = new HUD_Power_Up_Scrip();
	
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	//weapon 1
	script->AddDragBoxTextureResource("Texture_Pick_up_1", &classInstance->Texture_Pick_up_1);
	script->AddDragBoxMaterialComponent("Material Pick_up 1", &classInstance->Material_Pick_up_1);
	script->AddDragBoxGameObject("GameObject Pick_up_1", &classInstance->Gameobject_Pick_up_1);

	//weapon 2
	script->AddDragBoxTextureResource("Texture_Pick_up_2", &classInstance->Texture_Pick_up_2);
	script->AddDragBoxMaterialComponent("Material Pick_up 2", &classInstance->Material_Pick_up_2);
	script->AddDragBoxGameObject("GameObject Pick_up_2", &classInstance->Gameobject_Pick_up_2);

	//weapon 3
	script->AddDragBoxTextureResource("Texture_Pick_up_3", &classInstance->Texture_Pick_up_3);
	script->AddDragBoxMaterialComponent("Material Pick_up 3", &classInstance->Material_Pick_up_3);
	script->AddDragBoxGameObject("GameObject Pick_up_3", &classInstance->Gameobject_Pick_up_3);

	//weapon 4
	script->AddDragBoxTextureResource("Texture_Pick_up_4", &classInstance->Texture_Pick_up_4);
	script->AddDragBoxMaterialComponent("Material Pick_up 4", &classInstance->Material_Pick_up_4);
	script->AddDragBoxGameObject("GameObject Pick_up_4", &classInstance->Gameobject_Pick_up_4);

	//weapon 5
	script->AddDragBoxTextureResource("Texture_Pick_up_5", &classInstance->Texture_Pick_up_5);
	script->AddDragBoxMaterialComponent("Material Pick_up 5", &classInstance->Material_Pick_up_5);
	script->AddDragBoxGameObject("GameObject Pick_up_5", &classInstance->Gameobject_Pick_up_5);
	
	return classInstance;
}

void HUD_Power_Up_Scrip::Start()
{
	Gameobject_Pick_up_1.SetActive(false);
	Gameobject_Pick_up_2.SetActive(false);
	Gameobject_Pick_up_3.SetActive(false);
	Gameobject_Pick_up_4.SetActive(false);
	Gameobject_Pick_up_5.SetActive(false);

	
}
void HUD_Power_Up_Scrip::Update()
{

}