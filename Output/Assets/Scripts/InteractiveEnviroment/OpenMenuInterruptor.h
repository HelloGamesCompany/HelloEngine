#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerMove;

class OpenMenuInterruptor : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other) override;

    void OpenMenus();

    PlayerMove* playerMove;

    bool open;
    int numberOfAsignedMenus;
    API_GameObject menu[10];
    API_GameObject menuPanel;
};

