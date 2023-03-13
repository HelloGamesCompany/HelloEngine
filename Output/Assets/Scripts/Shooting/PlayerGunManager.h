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

    void SwapGun(bool next);
    void EquipNextGun();
    void EquipGun(int index);

    API_GameObject firstGun;
    API_GameObject secondGun;
    API_GameObject thirdGun;

    std::vector<API_GameObject> guns;
    int equipedIndex;
    PlayerGun* equipedGun = nullptr;
    int equipedGunType;

    bool canShoot = true;
    float bufferRB;
    float bufferLB;
};