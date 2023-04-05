#include "HUD_Power_Up_Scrip.h"
HELLO_ENGINE_API_C HUD_Power_Up_Scrip* CreateHUD_Power_Up_Scrip(ScriptToInspectorInterface* script)
{
	HUD_Power_Up_Scrip* classInstance = new HUD_Power_Up_Scrip();
	
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	//Textures
	script->AddDragBoxTextureResource("Texture_SPEED_INCREASE", &classInstance->Texture_Pick_up[0]);
	script->AddDragBoxTextureResource("Texture_FIRERATE_INCREASE", &classInstance->Texture_Pick_up[1]);
	script->AddDragBoxTextureResource("Texture_SHIELD", &classInstance->Texture_Pick_up[2]);
	script->AddDragBoxTextureResource("Texture_MAX_AMMO", &classInstance->Texture_Pick_up[3]);
	script->AddDragBoxTextureResource("Texture_SLOW_TIME", &classInstance->Texture_Pick_up[4]);

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
	for (int i = 0; i < 5; i++)
	{
		timer_power_up[i] = 0;
	}
	

	for (int i = 0; i < 5; i++)
	{
		Material_Pick_up[i]=Gameobjects_Pickables[i].GetMaterialCompoennt() ;
	}

	Gameobjects_Pickables[0].SetActive(false);
	Gameobjects_Pickables[1].SetActive(false);
	Gameobjects_Pickables[2].SetActive(false);
	Gameobjects_Pickables[3].SetActive(false);
	Gameobjects_Pickables[4].SetActive(false);
	

}
void HUD_Power_Up_Scrip::Update()
{
	
	for (int i = 0; i < 5; i++)
	{
		if (Gameobjects_Pickables[i].IsActive())
		{
			timer_power_up[i] += Time::GetDeltaTime();
			
			if (timer_power_up[i] >= livetime_Power )
			{
				timer_power_up[i] = 0;
				Gameobjects_Pickables[i].SetActive(false);
			}
		}
	}



	if (Input::GetKey(KeyCode::KEY_0) == KeyState::KEY_DOWN) {
		AddPowerUp(PowerUp_Type::FIRERATE_INCREASE);
		Console::Log("T0: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_9) == KeyState::KEY_DOWN) {
		AddPowerUp(PowerUp_Type::MAX_AMMO);
		Console::Log("T9: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_8) == KeyState::KEY_DOWN) {
		AddPowerUp(PowerUp_Type::SHIELD);
		Console::Log("T8: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_7) == KeyState::KEY_DOWN) {
		Number_Picked++;
		AddPowerUp(PowerUp_Type::SLOW_TIME);
		Console::Log("T7: ");
	}
	
	if (Input::GetKey(KeyCode::KEY_6) == KeyState::KEY_DOWN) {
		AddPowerUp(PowerUp_Type::SPEED_INCREASE);
		Console::Log("T6: ");
	}

}

void HUD_Power_Up_Scrip::AddPowerUp(PowerUp_Type Powertype)
{
	for (int i = 0; i < 5; i++)
	{
		if (!Gameobjects_Pickables[i].IsActive())
		{
			Material_Pick_up[i].ChangeAlbedoTexture(Texture_Pick_up[(uint)Powertype]);
			Current_texture[i] = Texture_Pick_up[(uint)Powertype];
			Gameobjects_Pickables[i].SetActive(true);
			return;
		}
	}

	/*uint TemporalText[5];

	for (int i = 0; i < 5; i++)
	{
		TemporalText[i] = Current_texture[i];
	}
	for (int i = 4; i > 0; --i)
	{
		Material_Pick_up[i - 1].ChangeAlbedoTexture(Current_texture[i]);
		
		Current_texture[i-1] = TemporalText[i];
	}

	Material_Pick_up[4].ChangeAlbedoTexture(Texture_Pick_up[(uint)Powertype]);
	Current_texture[4] = Texture_Pick_up[(uint)Powertype];*/
}
