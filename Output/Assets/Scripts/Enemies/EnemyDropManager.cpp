#include "EnemyDropManager.h"
HELLO_ENGINE_API_C EnemyDropManager* CreateEnemyDropManager(ScriptToInspectorInterface* script)
{
    EnemyDropManager* classInstance = new EnemyDropManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Enemy Drop Rate", &classInstance->enemyDropRate);
    script->AddDragFloat("Upgraded Enemy Drop Rate", &classInstance->upgradedEnemyDropRate);
    script->AddDragFloat("Enemy Ammo Drop Rate", &classInstance->enemyAmmoDropRate);
    script->AddDragFloat("Enemy Aid Kit Drop Rate", &classInstance->enemyAidKitDropRate);
    script->AddDragFloat("Enemy Power Ups Drop Rate", &classInstance->enemyPowerUpsDropRate);
    script->AddDragFloat("Box Drop Rate", &classInstance->BoxDropRate);
    script->AddDragFloat("Box Ammo Drop Rate", &classInstance->BoxAmmoDropRate);
    script->AddDragFloat("Box Aid Kit Drop Rate", &classInstance->BoxAidKitDropRate);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragInt("Pull Size per Drop Type", &classInstance->pullSize);
    script->AddDragBoxMeshResource("Lase Ammo Drop Mesh", &classInstance->mesh0);
    script->AddDragBoxMeshResource("First Aid Kit Drop Mesh", &classInstance->mesh1);
    script->AddDragBoxMeshResource("Speed Power Up Drop Mesh", &classInstance->mesh2);
    script->AddDragBoxMeshResource("Firearte Power Up Drop Mesh", &classInstance->mesh3);
    script->AddDragBoxMeshResource("Shield Drop Mesh", &classInstance->mesh4);
    script->AddDragBoxMeshResource("Max Ammo Drop Mesh", &classInstance->mesh5);
    script->AddDragBoxMeshResource("Slow Time Drop Mesh", &classInstance->mesh6);
    script->AddDragBoxTextureResource("Lase Ammo Drop Material", &classInstance->material0);
    script->AddDragBoxTextureResource("First Aid Kit Drop Material", &classInstance->material1);
    script->AddDragBoxTextureResource("Speed Power Up Drop Material", &classInstance->material2);
    script->AddDragBoxTextureResource("Firearte Power Up Drop Material", &classInstance->material3);
    script->AddDragBoxTextureResource("Shield Drop Material", &classInstance->material4);
    script->AddDragBoxTextureResource("Max Ammo Drop Material", &classInstance->material5);
    script->AddDragBoxTextureResource("Slow Time Drop Material", &classInstance->material6);
    return classInstance;
}

void EnemyDropManager::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("PlayerStats missing in EnemyDropManager Script.");

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Ammo Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh0);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material0);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 0;
        newDrop.SetActive(false);
        pull0.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("First Aid Kit Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh1);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material1);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 1;
        newDrop.SetActive(false);
        pull1.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Speed Power Up Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh2);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material2);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 2;
        newDrop.SetActive(false);
        pull2.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Firerate Power Up Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh3);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material3);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 3;
        newDrop.SetActive(false);
        pull3.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Shield Power Up Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh4);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material4);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 4;
        newDrop.SetActive(false);
        pull4.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Max Ammo Power Up Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh5);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material5);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 5;
        newDrop.SetActive(false);
        pull5.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::CreateGameObject("Slow Time Power Up Drop", "EnemyDrop");
        newDrop.AddMeshRenderer();
        newDrop.GetMeshRenderer().ChangeMesh(mesh6);
        newDrop.AddMaterial();
        newDrop.GetMaterialCompoennt().ChangeAlbedoTexture(material6);
        newDrop.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (2.0f, 2.0f, 2.0f), false);
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 6;
        newDrop.SetActive(false);
        pull6.push_back(newDrop);
    }
}

void EnemyDropManager::Update()
{

}

void EnemyDropManager::SpinDropRate(API_Vector3 position) // check if enemy drop things
{
    float dropRate = rand() % 100;
    float currentDropRate;
    if (playerStats && playerStats->armoryTreeLvl > 2) currentDropRate = upgradedEnemyDropRate;
    else currentDropRate = enemyDropRate;

    if (dropRate < currentDropRate)
    {
        API_GameObject go;
        float drop = rand() % 100;

        if (drop < enemyAmmoDropRate)
        {
            go = GetFirstInactiveObject(0);
        }
        else if (drop < enemyAmmoDropRate + enemyAidKitDropRate)
        {
            go = GetFirstInactiveObject(1);
        }
        else if (drop < enemyAmmoDropRate + enemyAidKitDropRate + enemyPowerUpsDropRate)
        {
            float powerUpDrop = rand() % 5;

            if (powerUpDrop < 1.0f)
            {
                go = GetFirstInactiveObject(2);
            }
            else if (powerUpDrop < 2.0f)
            {
                go = GetFirstInactiveObject(3);
            }
            else if (powerUpDrop < 3.0f)
            {
                go = GetFirstInactiveObject(4);
            }
            else if (powerUpDrop < 4.0f)
            {
                go = GetFirstInactiveObject(5);
            }
            else if (powerUpDrop < 5.0f)
            {
                go = GetFirstInactiveObject(6);
            }
        }

        go.SetActive(true);
        go.GetTransform().SetPosition(position);
    }
}

API_GameObject EnemyDropManager::GetFirstInactiveObject(int index)
{
    switch (index)
    {
    case 0:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull0[i].IsActive()) return pull0[i];
        }
        break;
    case 1:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull1[i].IsActive()) return pull1[i];
        }
        break;
    case 2:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull2[i].IsActive()) return pull2[i];
        }
        break;
    case 3:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull3[i].IsActive()) return pull3[i];
        }
        break;
    case 4:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull4[i].IsActive()) return pull4[i];
        }
        break;
    case 5:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull5[i].IsActive()) return pull5[i];
        }
        break;
    case 6:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull6[i].IsActive()) return pull6[i];
        }
        break;
    default:
        return pull0[0];
        break;
    }
}

void EnemyDropManager::BoxSpinDropRate(API_Vector3 position)
{
    float dropRate = rand() % 100;
    
    if (dropRate < BoxDropRate)
    {
        API_GameObject go;

        float drop = rand() % 100;

        if (drop < BoxAidKitDropRate) // 50% first aid kit
        {
            go = GetFirstInactiveObject(1);
        }
        else if (drop < BoxAmmoDropRate + BoxAidKitDropRate)  // 50% Normal Ammo
        {
            go = GetFirstInactiveObject(0);
        }

        go.SetActive(true);
        API_Vector3 newPos = { position.x, position.y + 1.0f, position.z };
        go.GetTransform().SetPosition(newPos);

    }
    else 
    {
        return;
    }
}
