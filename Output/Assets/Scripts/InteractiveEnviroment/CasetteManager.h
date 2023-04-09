#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class CasetteManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject casette1;
    API_GameObject casette2;
    API_GameObject casette3;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;

    bool check;
};

