#include "Movment_Bar_HP.h"
HELLO_ENGINE_API_C Movment_Bar_HP* CreateMovment_Bar_HP(ScriptToInspectorInterface* script)
{
	Movment_Bar_HP* classInstance = new Movment_Bar_HP();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxTransform("Bar", &classInstance->Movment_Bar_HP);

	script->AddCheckBox("hp", &classInstance->Low_HP);

	return classInstance;
}

void Movment_Bar_HP::Start()
{
	directionY = { 0,0.00015,0 };
	directionYfast = { 0,0.0008,0 };
}
void Movment_Bar_HP::Update()
{
	timer -= Time::GetDeltaTime();
	timer2 -= Time::GetDeltaTime();

	if (!Low_HP)
	{

		if (timer <= 0.0f)
		{
			timer = 1.0f;
			directionY = -directionY;
		}

		Movment_Bar_HP.Translate(directionY);
	}

	if (Low_HP)
	{

		if (timer2 <= 0.0f)
		{
			timer2 = 0.5f;
			directionYfast = -directionYfast;
		}

		Movment_Bar_HP.Translate(directionYfast);
	}

}

bool Movment_Bar_HP::Speed_HP(bool Low_HP)
{
	this->Low_HP = Low_HP;
	directionY = { 0,0.00015,0 };
	directionYfast = { 0,0.0004,0 };
}
