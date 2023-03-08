#include "TankScript.h"
#include "BulletBehavior.h"

HELLO_ENGINE_API_C TankScript* CreateTankScript(ScriptToInspectorInterface* script)
{
    TankScript* classInstance = new TankScript();

    script->AddDragFloat("Moving speed", &classInstance->movingSpeed);
    script->AddDragBoxTransform("Turret Transfrom", &classInstance->turret);
    script->AddDragBoxTransform("Shooting point", &classInstance->shootingPoint);
    script->AddDragBoxMeshRenderer("Bullet Mesh", &classInstance->bulletMesh);

    return classInstance;
}

void TankScript::Start()
{
    Engine::centerMouse = true; // Hides the cursor, and centers it on the middle of the screen (FPS style)
}
void TankScript::Update()
{
    // Shooting

    if (Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN)
    {
        // Create game object
        API_GameObject newBullet = Game::CreateGameObject("Bullet", "Projectile");
        newBullet.GetTransform().SetPosition(shootingPoint.GetGlobalPosition());

        // Add behavior
        BulletBehavior* bulletScript = (BulletBehavior*)newBullet.AddScript("BulletBehavior");
        // Add mesh renderer, copied from another mesh renderer properties.
        newBullet.AddMeshRenderer(bulletMesh);

        // Set direction of the bullet
        if (bulletScript != nullptr)
        {
            bulletScript->direction = turret.GetForward();
            API_Vector3 rotation = turret.GetLocalRotation() + gameObject.GetTransform().GetLocalRotation();
            bulletScript->gameObject.GetTransform().SetRotation(rotation);
        }

    }

    // Turret control
    if (Input::GetMouseXMotion() < 0)
    {
        turret.Rotate(0, 1, 0);
    }
    if (Input::GetMouseXMotion() > 0)
    {
        turret.Rotate(0, -1, 0);
    }

    // Movement-------------------------------------------------
    if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
    {
        // Move forward
        gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * movingSpeed);
    }
    if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
    {
        // Move backwards
        gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * movingSpeed);
    }
    if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
    {
        // Move Left
        gameObject.GetTransform().Translate(gameObject.GetTransform().GetRight() * movingSpeed);
    }
    if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
    {
        // Move right
        gameObject.GetTransform().Translate(gameObject.GetTransform().GetLeft() * movingSpeed);
    }

    if (Input::GetKey(KeyCode::KEY_Q) == KeyState::KEY_REPEAT)
    {
        // Rotate left
        gameObject.GetTransform().Rotate(0, 1, 0);
    }
    if (Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_REPEAT)
    {
        // Rotate right
        gameObject.GetTransform().Rotate(0, -1, 0);
    }

}