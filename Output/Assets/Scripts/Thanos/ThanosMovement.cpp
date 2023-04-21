#include "ThanosMovement.h"
#include "ThanosAttacks.h"

HELLO_ENGINE_API_C ThanosMovement* CreateThanosMovement(ScriptToInspectorInterface* script)
{
	ThanosMovement* classInstance = new ThanosMovement();
	script->AddDragBoxGameObject("Player", &classInstance->player);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosMovement::Start()
{
    Tattack = (ThanosAttacks*)boss.GetScript("ThanosAttacks");
    if (Tattack == nullptr) {
        Console::Log("ThanosAttacks missing in thanosmovement script ");
    }
}
void ThanosMovement::Update()
{
    if (Tattack->isAttacking == false) {
        angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);
        Seek(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed);
    }
    dashCooldown += Time::GetDeltaTime();

}

float ThanosMovement::Rotate(API_Vector3 target, float _angle)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - gameObject.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - gameObject.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, -_angle, 0);

    return _angle;
}

void ThanosMovement::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
    API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
    seeker->GetTransform().Translate(direction * speed / 100);

    if (direction.x < 8 && direction.x > -8 && direction.y < 8 && direction.y && direction.z < 8 && direction.z && dashCooldown > 5.0f) {
        if (Tattack) {

            Tattack->isAttacking = true;
            Tattack->thanosState = ThanosAttacks::THANOS_STATE::IDLE;
            Tattack->playerPosition = player.GetTransform().GetGlobalPosition();
        }

    }
    else if (direction.x < 3 && direction.x > -3 && direction.y < 3 && direction.y && direction.z < 3 && direction.z && dashCooldown < 5.0f) {

        if (Tattack) {
            Tattack->thanosState = ThanosAttacks::THANOS_STATE::MELEEATTACK;
            Tattack->isAttacking = true;
        }
    }
    
}
