#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "EnemyDrop.h"

class EnemyDropManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void SpinDropRate(API_Vector3 position);
    API_GameObject GetFirstInactiveObject(int index);

    void BoxSpinDropRate(API_Vector3 position);

    //Drop chances

    //Enemy
    int enemyDropRate;
    int enemyAmmoDropRate;
    int enemyAidKitDropRate;
    int enemyPowerUpsDropRate;

    //Breakable Box
    int BoxDropRate;
    int BoxAmmoDropRate;
    int BoxAidKitDropRate;

    // meshes
    uint mesh0;
    uint mesh1;
    uint mesh2;
    uint mesh3;
    uint mesh4;
    uint mesh5;
    uint mesh6;

    // materials
    uint material0;
    uint material1;
    uint material2;
    uint material3;
    uint material4;
    uint material5;
    uint material6;

    // pull
    int pullSize;
    std::vector<API_GameObject> pull0;
    std::vector<API_GameObject> pull1;
    std::vector<API_GameObject> pull2;
    std::vector<API_GameObject> pull3;
    std::vector<API_GameObject> pull4;
    std::vector<API_GameObject> pull5;
    std::vector<API_GameObject> pull6;
};

