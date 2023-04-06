#include "UI_Municion.h"
HELLO_ENGINE_API_C UI_Municion* CreateUI_Municion(ScriptToInspectorInterface* script)
{
	UI_Municion* classInstance = new UI_Municion();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Infinit munition on", &classInstance->TEXT_munition_InfinitWeapon_On);
	script->AddDragBoxGameObject("Infinit munition off", &classInstance->TEXT_munition_InfinitWeapon_Off);

	script->AddDragBoxUIText("Normal munition", &classInstance->TEXT_munition_Normal);
	script->AddDragBoxUIText("variableName", &classInstance->TEXT_munition_Special);
	return classInstance;
}

void UI_Municion::Start()
{

}

void UI_Municion::Update()
{
	TEXT_munition_Normal.SetText(std::to_string(hola).c_str());


}