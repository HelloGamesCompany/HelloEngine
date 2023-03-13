#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "PlayerGun.h"
#include "PlayerGunType.h"

class PlayerGunManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void GetGun(int slot, int gunIndex);
    void EquipGun(int index);

    std::vector<API_GameObject> guns;
    int equipedIndex;
    PlayerGun* equipedGun = nullptr;
    int gunOnHandIndex1;
    int gunOnHandIndex2;
    int gunOnHandIndex3;

    bool canShoot = true;
    float bufferRB;
    float bufferLB;

    // guns
    API_GameObject duals; // index 0
    API_GameObject semiauto; // index 1
    API_GameObject automatic; // index 2
    API_GameObject shotgun; // index 3
};