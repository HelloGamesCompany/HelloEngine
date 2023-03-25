#include "SwapWeapon.h"
HELLO_ENGINE_API_C SwapWeapon* CreateSwapWeapon(ScriptToInspectorInterface* script)
{
	SwapWeapon* classInstance = new SwapWeapon();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	//weapon 1
	script->AddDragBoxTextureResource("Text_Weapon_1_on", &classInstance->Texture_Weapon_1_on);
	script->AddDragBoxTextureResource("Text_Weapon_1_off", &classInstance->Texture_Weapon_1_off);

	script->AddDragBoxMaterialComponent("Material Weapon 1", &classInstance->Material_Weapon_1);
	script->AddDragBoxGameObject("Game Bar Weapon 1", &classInstance->Active_Bar_1);

	//weapon 2
	script->AddDragBoxTextureResource("Text_Weapon_2_on", &classInstance->Texture_Weapon_2_on);
	script->AddDragBoxTextureResource("Text_Weapon_2_off", &classInstance->Texture_Weapon_2_off);

	script->AddDragBoxMaterialComponent("Material Weapon 2", &classInstance->Material_Weapon_2);
	script->AddDragBoxGameObject("Game Bar Weapon 2", &classInstance->Active_Bar_2);

	//weapon 3
	script->AddDragBoxTextureResource("Text_Weapon_3_on", &classInstance->Texture_Weapon_3_on);
	script->AddDragBoxTextureResource("Text_Weapon_3_off", &classInstance->Texture_Weapon_3_off);

	script->AddDragBoxMaterialComponent("Material Weapon 3", &classInstance->Material_Weapon_3);
	script->AddDragBoxGameObject("Game Bar Weapon 3", &classInstance->Active_Bar_3);
	
	////dash
	//script->AddDragBoxTextureResource("Text_Dash_on", &classInstance->Texture_Dash_on);
	//script->AddDragBoxTextureResource("Text_Dash_off", &classInstance->Texture_Dash_off);

	//script->AddDragBoxMaterialComponent("Material_Dash", &classInstance->Material_Dash);

	return classInstance;
}

void SwapWeapon::Start()
{
	//weapon 1
	Material_Weapon_1.ChangeAlbedoTexture(Texture_Weapon_1_on);
	Active_Bar_1.SetActive(true);

	Material_Weapon_1.GetGameObject().GetTransform().SetScale({ 0.12,0.12,0.5 });
	Material_Weapon_2.GetGameObject().GetTransform().SetScale({ 0.15,0.15,0.5 });
	Material_Weapon_3.GetGameObject().GetTransform().SetScale({ 0.125,0.125,0.5 });
	
	//weapon 2
	Material_Weapon_2.ChangeAlbedoTexture(Texture_Weapon_2_off);
	Active_Bar_2.SetActive(false);
	//weapon 3
	Material_Weapon_3.ChangeAlbedoTexture(Texture_Weapon_3_off);
	Active_Bar_3.SetActive(false);

	//Dash
	//Material_Dash.ChangeAlbedoTexture(Texture_Dash_on);
}
void SwapWeapon::Update()
{

	/*
	if (Input::GetKey(KeyCode::KEY_B) == KeyState::KEY_DOWN)
	{
		SwapWeapon1();
	}
	
	if (Input::GetKey(KeyCode::KEY_N) == KeyState::KEY_DOWN)
	{
		SwapWeapon2();
	}
	
	if (Input::GetKey(KeyCode::KEY_M) == KeyState::KEY_DOWN)
	{
		SwapWeapon3();
	}*/

	/*if (Input::GetKey(KeyCode::KEY_B) == KeyState::KEY_DOWN)
	{
		if (activeDash == true)
		{
			activeDash = false;
			Dash();
		}
		
		else if (activeDash == false)
		{
			activeDash = true;
			Dash();
		}
	}*/
}

void SwapWeapon::SwapWeapon1()
{
	//texture
	Material_Weapon_1.ChangeAlbedoTexture(Texture_Weapon_1_on);
	Material_Weapon_2.ChangeAlbedoTexture(Texture_Weapon_2_off);
	Material_Weapon_3.ChangeAlbedoTexture(Texture_Weapon_3_off);



	//game object
	//Scale
	Material_Weapon_1.GetGameObject().GetTransform().SetScale({0.12,0.12,0.5});
	Material_Weapon_2.GetGameObject().GetTransform().SetScale({0.15,0.15,0.5});
	Material_Weapon_3.GetGameObject().GetTransform().SetScale({0.125,0.125,0.5});

	Active_Bar_1.SetActive(true);
	Active_Bar_2.SetActive(false);
	Active_Bar_3.SetActive(false);
}

void SwapWeapon::SwapWeapon2()
{
	//texture
	Material_Weapon_1.ChangeAlbedoTexture(Texture_Weapon_1_off);
	Material_Weapon_2.ChangeAlbedoTexture(Texture_Weapon_2_on);
	Material_Weapon_3.ChangeAlbedoTexture(Texture_Weapon_3_off);

	//game objects
	Material_Weapon_1.GetGameObject().GetTransform().SetScale({ 0.07,0.07,0.5 });
	Material_Weapon_2.GetGameObject().GetTransform().SetScale({ 0.2,0.2,0.5 });
	Material_Weapon_3.GetGameObject().GetTransform().SetScale({ 0.125,0.125,0.5 });

	Active_Bar_1.SetActive(false);
	Active_Bar_2.SetActive(true);
	Active_Bar_3.SetActive(false);
}

void SwapWeapon::SwapWeapon3()
{
	//texture
	Material_Weapon_1.ChangeAlbedoTexture(Texture_Weapon_1_off);
	Material_Weapon_2.ChangeAlbedoTexture(Texture_Weapon_2_off);
	Material_Weapon_3.ChangeAlbedoTexture(Texture_Weapon_3_on);

	//game objects
	Material_Weapon_1.GetGameObject().GetTransform().SetScale({ 0.07,0.07,0.5 });
	Material_Weapon_2.GetGameObject().GetTransform().SetScale({ 0.15,0.15,0.5 });
	Material_Weapon_3.GetGameObject().GetTransform().SetScale({ 0.14,0.14,0.5 });

	Active_Bar_1.SetActive(false);
	Active_Bar_2.SetActive(false);
	Active_Bar_3.SetActive(true);
}

