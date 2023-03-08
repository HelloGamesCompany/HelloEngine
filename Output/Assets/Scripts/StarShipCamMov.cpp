#include "StarShipCamMov.h"
HELLO_ENGINE_API_C StarShipCamMov* CreateStarShipCamMov(ScriptToInspectorInterface* script)
{
	StarShipCamMov* classInstance = new StarShipCamMov();
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragFloat("Delay", &classInstance->delay);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

float StarShipCamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void StarShipCamMov::Start()
{

	camPos.x = 10.246f;
	camPos.y = 51.238f;
	camPos.z = -25.252f;

	camRot.x = 57.278f;
	camRot.y = -9.614f;
	camRot.z = 7.943f;

	gameObject.GetTransform().SetPosition(camPos);
	gameObject.GetTransform().SetRotation(camRot);
}
void StarShipCamMov::Update()
{

	desiredPosition.x = target.GetTransform().GetGlobalPosition().x + camPos.x;
	desiredPosition.y = target.GetTransform().GetGlobalPosition().y + camPos.y;
	desiredPosition.z = target.GetTransform().GetGlobalPosition().z + camPos.z;

	smoothedPosition.x = Lerp(gameObject.GetTransform().GetGlobalPosition().x, desiredPosition.x, delay);
	smoothedPosition.y = Lerp(gameObject.GetTransform().GetGlobalPosition().y, desiredPosition.y, delay);
	smoothedPosition.z = Lerp(gameObject.GetTransform().GetGlobalPosition().z, desiredPosition.z, delay);

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}