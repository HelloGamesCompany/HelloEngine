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
	camPos.x = gameObject.GetTransform().GetGlobalPosition().x - 0.951;
	camPos.y = gameObject.GetTransform().GetGlobalPosition().y + 28.8f;
	camPos.z = gameObject.GetTransform().GetGlobalPosition().z - 5.4f;

	camRot.x = gameObject.GetTransform().GetGlobalRotation().x +81.263f;
	camRot.y = gameObject.GetTransform().GetGlobalRotation().y +5.576;
	camRot.z = gameObject.GetTransform().GetGlobalRotation().z -2.423f;
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