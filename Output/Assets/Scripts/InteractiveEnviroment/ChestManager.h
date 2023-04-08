#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ChestManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    int GetGunBlueprint();
    int GetSpecialGun();

    API_GameObject chest1;
    API_GameObject chest2;
    API_GameObject chest3;
    API_GameObject chest4;

    bool check;
};

