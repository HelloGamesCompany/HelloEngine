#include "BossMovement.h"
HELLO_ENGINE_API_C BossMovement* CreateBossMovement(ScriptToInspectorInterface* script)
{
	BossMovement* classInstance = new BossMovement();
    script->AddDragBoxGameObject("Player", &classInstance->player);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossMovement::Start()
{

}
void BossMovement::Update()
{

    Seek(player.GetTransform().GetGlobalPosition());
    
}

void BossMovement::Seek(API_Vector3 target)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - gameObject.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - gameObject.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    float _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, -_angle, 0);

}
