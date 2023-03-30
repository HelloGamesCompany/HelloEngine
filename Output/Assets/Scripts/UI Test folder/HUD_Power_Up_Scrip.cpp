#include "HUD_Power_Up_Scrip.h"
HELLO_ENGINE_API_C HUD_Power_Up_Scrip* CreateHUD_Power_Up_Scrip(ScriptToInspectorInterface* script)
{
	HUD_Power_Up_Scrip* classInstance = new HUD_Power_Up_Scrip();
	
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
	
	//weapon 1
	//script->AddDragBoxTextureResource("Texture_Pick_up_1", &classInstance->Texture_Pick_up_1);
	//script->AddDragBoxMaterialComponent("Material Pick_up 1", &classInstance->Material_Pick_up_1);
	//script->AddDragBoxGameObject("GameObject Pick_up_1", &classInstance->Gameobject_Pick_up_1);


	//weapon 2
	//script->AddDragBoxTextureResource("Texture_Pick_up_2", &classInstance->Texture_Pick_up_2);
	//script->AddDragBoxMaterialComponent("Material Pick_up 2", &classInstance->Material_Pick_up_2);
	//script->AddDragBoxGameObject("GameObject Pick_up_2", &classInstance->Gameobject_Pick_up_2);


	//weapon 3
	//script->AddDragBoxTextureResource("Texture_Pick_up_3", &classInstance->Texture_Pick_up_3);
	//script->AddDragBoxMaterialComponent("Material Pick_up 3", &classInstance->Material_Pick_up_3);
	//script->AddDragBoxGameObject("GameObject Pick_up_3", &classInstance->Gameobject_Pick_up_3);


	//weapon 4
	//script->AddDragBoxTextureResource("Texture_Pick_up_4", &classInstance->Texture_Pick_up_4);
	//script->AddDragBoxMaterialComponent("Material Pick_up 4", &classInstance->Material_Pick_up_4);
	//script->AddDragBoxGameObject("GameObject Pick_up_4", &classInstance->Gameobject_Pick_up_4);


	//weapon 5
	//script->AddDragBoxTextureResource("Texture_Pick_up_5", &classInstance->Texture_Pick_up_5);
	//script->AddDragBoxMaterialComponent("Material Pick_up 5", &classInstance->Material_Pick_up_5);
	//script->AddDragBoxGameObject("GameObject Pick_up_5", &classInstance->Gameobject_Pick_up_5);
	
	
	script->AddDragBoxMaterialComponent("Material Pick_up 1", &classInstance->Material_Pick_up[0]);
	script->AddDragBoxMaterialComponent("Material Pick_up 2", &classInstance->Material_Pick_up[1]);
	script->AddDragBoxMaterialComponent("Material Pick_up 3", &classInstance->Material_Pick_up[2]);
	script->AddDragBoxMaterialComponent("Material Pick_up 4", &classInstance->Material_Pick_up[3]);
	script->AddDragBoxMaterialComponent("Material Pick_up 5", &classInstance->Material_Pick_up[4]);

	//Textures
	script->AddDragBoxTextureResource("Texture_Pick_up_1", &classInstance->Texture_Pick_up[0]);
	script->AddDragBoxTextureResource("Texture_Pick_up_2", &classInstance->Texture_Pick_up[1]);
	script->AddDragBoxTextureResource("Texture_Pick_up_3", &classInstance->Texture_Pick_up[2]);
	script->AddDragBoxTextureResource("Texture_Pick_up_4", &classInstance->Texture_Pick_up[3]);
	script->AddDragBoxTextureResource("Texture_Pick_up_5", &classInstance->Texture_Pick_up[4]);

	//GameObjects
	script->AddDragBoxGameObject("GamesObjects_1", &classInstance->Gameobjects_Pickables[0]);
	script->AddDragBoxGameObject("GamesObjects_2", &classInstance->Gameobjects_Pickables[1]);
	script->AddDragBoxGameObject("GamesObjects_3", &classInstance->Gameobjects_Pickables[2]);
	script->AddDragBoxGameObject("GamesObjects_4", &classInstance->Gameobjects_Pickables[3]);
	script->AddDragBoxGameObject("GamesObjects_5", &classInstance->Gameobjects_Pickables[4]);
	
	
	return classInstance;
}

void HUD_Power_Up_Scrip::Start()
{
	
	//Material_Pick_up[0].ChangeAlbedoTexture(Texture_Pick_up[0]);
	//Material_Pick_up[1].ChangeAlbedoTexture(Texture_Pick_up[1]);
	//Material_Pick_up[2].ChangeAlbedoTexture(Texture_Pick_up[2]);
	//Material_Pick_up[3].ChangeAlbedoTexture(Texture_Pick_up[3]);
	//Material_Pick_up[4].ChangeAlbedoTexture(Texture_Pick_up[4]);
	/*
	
	*/
	Gameobjects_Pickables[0].SetActive(false);
	Gameobjects_Pickables[1].SetActive(false);
	Gameobjects_Pickables[2].SetActive(false);
	Gameobjects_Pickables[3].SetActive(false);
	Gameobjects_Pickables[4].SetActive(false);
	/*
	Gameobject_Pick_up_1.SetActive(false);
	Gameobject_Pick_up_2.SetActive(false);
	Gameobject_Pick_up_3.SetActive(false);
	Gameobject_Pick_up_4.SetActive(false);
	Gameobject_Pick_up_5.SetActive(false);
	*/
	//Console::Log(Number_Picked);
}
void HUD_Power_Up_Scrip::Update()
{

	if (Input::GetKey(KeyCode::KEY_0) == KeyState::KEY_DOWN && Number_Picked < 5) {
		Number_Picked++;
		GetPowerUp(Texture_Pick_up[0]);
		Console::Log("T0: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_9) == KeyState::KEY_DOWN && Number_Picked < 5) {
		Number_Picked++;
		GetPowerUp(Texture_Pick_up[1]);
		Console::Log("T9: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_8) == KeyState::KEY_DOWN && Number_Picked < 5) {
		Number_Picked++;
		GetPowerUp(Texture_Pick_up[2]);
		Console::Log("T8: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_7) == KeyState::KEY_DOWN && Number_Picked < 5) {
		Number_Picked++;
		GetPowerUp(Texture_Pick_up[3]);
		Console::Log("T7: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_6) == KeyState::KEY_DOWN && Number_Picked < 5) {
		Number_Picked++;
		GetPowerUp(Texture_Pick_up[4]);
		Console::Log("T6: ");
	}

}

void HUD_Power_Up_Scrip::GetPowerUp(uint texture)
{

	//Console::Log(Number_Picked);

	for (int i = 0; i < Number_Picked; i++) {
		Gameobjects_Pickables[i].SetActive(true);
		Material_Pick_up[i].ChangeAlbedoTexture(texture);
		Console::Log("entrat: ");
	}
}
