#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayAmbienceMusic : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    std::string sound = "";
    float maxSoundDuration;
    float soundDuration;
};

