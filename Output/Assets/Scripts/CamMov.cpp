#include "CamMov.h"
HELLO_ENGINE_API_C CamMov* CreateCamMov(ScriptToInspectorInterface* script)
{
	CamMov* classInstance = new CamMov();
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragFloat("Delay", &classInstance->delay);
	script->AddDragFloat("Offset_X", &classInstance->camPos.x);
	script->AddDragFloat("Offset_Y", &classInstance->camPos.y);
	script->AddDragFloat("Offset_Z", &classInstance->camPos.z);
	script->AddDragFloat("Rot_X", &classInstance->camRot.x);
	script->AddDragFloat("Rot_Y", &classInstance->camRot.y);
	script->AddDragFloat("Rot_Z", &classInstance->camRot.z);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("SafeZone_Distance", &classInstance->safeZoneDistance);
	script->AddCheckBox("Use safe zone: ", &classInstance->safeZone);
	return classInstance;
}

float CamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void CamMov::Start()
{
	//camPos -= target.GetTransform().GetGlobalPosition();
	//camRot -= target.GetTransform().GetGlobalRotation();

	gameObject.GetTransform().SetPosition(target.GetTransform().GetGlobalPosition() + camPos);
	gameObject.GetTransform().SetRotation(camRot);
	tempDelay = delay;
}

void CamMov::Update()
{
	gameObject.GetTransform().SetRotation(camRot);
	desiredPosition = target.GetTransform().GetGlobalPosition() + camPos;
	
	if (safeZone)
	{
		if (desiredPosition.Distance(gameObject.GetTransform().GetGlobalPosition()) > safeZoneDistance) {
			tempDelay += 0.01;
		}
		else {
			if (tempDelay > delay) {
				tempDelay -= 0.01;
			}
		}
	}

	smoothedPosition.x = Lerp(gameObject.GetTransform().GetGlobalPosition().x, desiredPosition.x, tempDelay);
	smoothedPosition.y = Lerp(gameObject.GetTransform().GetGlobalPosition().y, desiredPosition.y, tempDelay);
	smoothedPosition.z = Lerp(gameObject.GetTransform().GetGlobalPosition().z, desiredPosition.z, tempDelay);

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}