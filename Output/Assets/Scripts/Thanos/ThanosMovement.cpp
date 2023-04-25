#include "ThanosMovement.h"
#include "ThanosAttacks.h"

HELLO_ENGINE_API_C ThanosMovement* CreateThanosMovement(ScriptToInspectorInterface* script)
{
	ThanosMovement* classInstance = new ThanosMovement();
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosMovement::Start()
{
    Tattack = (ThanosAttacks*)boss.GetScript("ThanosAttacks");
}
void ThanosMovement::Update()
{
   if (Tattack->isAttacking == false) {
        angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);
        Console::Log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        Seek(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed);
  }
    dashCooldown += Time::GetDeltaTime();
}

float ThanosMovement::Rotate(API_Vector3 target, float _angle)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - boss.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - boss.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    boss.GetTransform().SetRotation(0, -_angle, 0);

    return _angle;
}

void ThanosMovement::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
    API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
    seeker->GetTransform().Translate(direction * speed / 100);
    //Console::Log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    if (direction.x < 8 && direction.x > -8 && direction.y < 8 && direction.y && direction.z < 8 && direction.z && dashCooldown > 5.0f) {

            Tattack->isAttacking = true;
            dashCooldown = 0.0f;
            Tattack->thanosState = ThanosAttacks::THANOS_STATE::IDLE;
            Tattack->playerPosition = player.GetTransform().GetGlobalPosition();

    }
    else if (direction.x < 3 && direction.x > -3 && direction.y < 3 && direction.y && direction.z < 3 && direction.z && dashCooldown < 5.0f) {

            Tattack->thanosState = ThanosAttacks::THANOS_STATE::MELEEATTACK;
            Tattack->isAttacking = true;
            dashCooldown = 0.0f;
    }
    
}
