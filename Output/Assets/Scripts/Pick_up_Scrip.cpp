#include "Pick_up_Scrip.h"
HELLO_ENGINE_API_C Pick_up_Scrip* CreatePick_up_Scrip(ScriptToInspectorInterface* script)
{
	Pick_up_Scrip* classInstance = new Pick_up_Scrip();

	//weapon 1
	script->AddDragBoxTextureResource("Texture_Pick_up_1_on", &classInstance->Texture_Pick_up_1_on);
	script->AddDragBoxTextureResource("Texture_Pick_up_1_off", &classInstance->Texture_Pick_up_1_off);
	script->AddDragBoxMaterialComponent("Material Pick_up 1", &classInstance->Material_Pick_up_1);
	script->AddDragBoxGameObject("GameObject Pick_up_1", &classInstance->Gameobject_Pick_up_1);

	//weapon 2
	script->AddDragBoxTextureResource("Texture_Pick_up_on", &classInstance->Texture_Pick_up_2_on);
	script->AddDragBoxTextureResource("Texture_Pick_up_off", &classInstance->Texture_Pick_up_2_off);
	script->AddDragBoxMaterialComponent("Material Pick_up 2", &classInstance->Material_Pick_up_2);
	script->AddDragBoxGameObject("GameObject Pick_up_2", &classInstance->Gameobject_Pick_up_2);

	//weapon 3
	script->AddDragBoxTextureResource("Texture_Pick_up_on", &classInstance->Texture_Pick_up_3_on);
	script->AddDragBoxTextureResource("Texture_Pick_up_off", &classInstance->Texture_Pick_up_3_off);
	script->AddDragBoxMaterialComponent("Material Pick_up 3", &classInstance->Material_Pick_up_3);
	script->AddDragBoxGameObject("GameObject Pick_up_3", &classInstance->Gameobject_Pick_up_3);

	//weapon 4
	script->AddDragBoxTextureResource("Texture_Pick_up_on", &classInstance->Texture_Pick_up_4_on);
	script->AddDragBoxTextureResource("Texture_Pick_up_off", &classInstance->Texture_Pick_up_4_off);
	script->AddDragBoxMaterialComponent("Material Pick_up 4", &classInstance->Material_Pick_up_4);
	script->AddDragBoxGameObject("GameObject Pick_up_4", &classInstance->Gameobject_Pick_up_4);

	//weapon 5
	script->AddDragBoxTextureResource("Texture_Pick_up_on", &classInstance->Texture_Pick_up_5_on);
	script->AddDragBoxTextureResource("Texture_Pick_up_off", &classInstance->Texture_Pick_up_5_off);
	script->AddDragBoxMaterialComponent("Material Pick_up 5", &classInstance->Material_Pick_up_5);
	script->AddDragBoxGameObject("GameObject Pick_up_5", &classInstance->Gameobject_Pick_up_5);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Pick_up_Scrip::Start()
{

}
void Pick_up_Scrip::Update()
{

}