#include "StarShipCamMov.h"
HELLO_ENGINE_API_C StarShipCamMov* CreateStarShipCamMov(ScriptToInspectorInterface* script)
{
	StarShipCamMov* classInstance = new StarShipCamMov();
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragFloat("Delay", &classInstance->delay);
	script->AddDragFloat("Offset_X", &classInstance->camPos.x);
	script->AddDragFloat("Offset_Y", &classInstance->camPos.y);
	script->AddDragFloat("Offset_Z", &classInstance->camPos.z);
	script->AddDragFloat("Rot_Offset_X", &classInstance->camRot.x);
	script->AddDragFloat("Rot_Offset_Y", &classInstance->camRot.y);
	script->AddDragFloat("Rot_Offset_Z", &classInstance->camRot.z);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

float StarShipCamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void StarShipCamMov::Start()
{
	camPos.x -= target.GetTransform().GetGlobalPosition().x;
	camPos.y -= target.GetTransform().GetGlobalPosition().y;
	camPos.z -= target.GetTransform().GetGlobalPosition().z;

	camRot.x -= target.GetTransform().GetGlobalRotation().x;
	camRot.y -= target.GetTransform().GetGlobalRotation().y;
	camRot.z -= target.GetTransform().GetGlobalRotation().z;

	/*camPos.x = gameObject.GetTransform().GetGlobalPosition().x;
	camPos.y = gameObject.GetTransform().GetGlobalPosition().y + 11.100f;
	camPos.z = gameObject.GetTransform().GetGlobalPosition().z + 10.0f;

	camRot.x = gameObject.GetTransform().GetGlobalRotation().x + 122.923f;
	camRot.y = gameObject.GetTransform().GetGlobalRotation().y;
	camRot.z = gameObject.GetTransform().GetGlobalRotation().z + 176.947f;*/

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