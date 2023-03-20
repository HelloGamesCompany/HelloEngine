#include "CamMov.h"
HELLO_ENGINE_API_C CamMov* CreateCamMov(ScriptToInspectorInterface* script)
{
	CamMov* classInstance = new CamMov();
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

float CamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void CamMov::Start()
{
	camPos -= target.GetTransform().GetGlobalPosition();
	camRot -= target.GetTransform().GetGlobalRotation();

	gameObject.GetTransform().SetPosition(camPos);
	gameObject.GetTransform().SetRotation(camRot);

}

void CamMov::Update()
{

	if (target.GetTransform().GetGlobalPosition().x > gameObject.GetTransform().GetGlobalPosition().x + 13
		|| target.GetTransform().GetGlobalPosition().x < gameObject.GetTransform().GetGlobalPosition().x - 13
		|| target.GetTransform().GetGlobalPosition().z < gameObject.GetTransform().GetGlobalPosition().z + 11
		|| target.GetTransform().GetGlobalPosition().z > gameObject.GetTransform().GetGlobalPosition().z + 40
		) {
		delay += 0.001;
	}
	else {
		delay = 0.02;
	}
	
	gameObject.GetTransform().SetRotation(camRot);

	desiredPosition.x = target.GetTransform().GetGlobalPosition().x + camPos.x;
	desiredPosition.y = target.GetTransform().GetGlobalPosition().y + camPos.y;
	desiredPosition.z = target.GetTransform().GetGlobalPosition().z + camPos.z;

	smoothedPosition.x = Lerp(gameObject.GetTransform().GetGlobalPosition().x, desiredPosition.x, delay);
	smoothedPosition.y = Lerp(gameObject.GetTransform().GetGlobalPosition().y, desiredPosition.y, delay);
	smoothedPosition.z = Lerp(gameObject.GetTransform().GetGlobalPosition().z, desiredPosition.z, delay);

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}