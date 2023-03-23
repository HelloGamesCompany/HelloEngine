#include "Movment_Bar.h"
HELLO_ENGINE_API_C Movment_Bar* CreateMovment_Bar(ScriptToInspectorInterface* script)
{
	Movment_Bar* classInstance = new Movment_Bar();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxTransform("Bar", &classInstance->Movment_Bar);
	return classInstance;
}

void Movment_Bar::Start()
{
	directionY = { 0,0.00015,0 };
}
void Movment_Bar::Update()
{
	timer -= Time::GetDeltaTime();

	if (timer <= 0.0f)
	{
		timer = 1.0;
		directionY = -directionY;
	}

	Movment_Bar.Translate(directionY);
}