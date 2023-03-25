#pragma once
#include "Component.h"
#include "Particle.h"

enum class BILLBOARDTYPE
{
    NO_ALIGN,
    SCREENALIGN,
    WORLDALIGN,
    AXISALIGN,
};

class Application;

class BillBoardComponent :
    public Component
{
public:

    BillBoardComponent(GameObject* gameObject);

    ~BillBoardComponent();

    void OnEnable() override;
    void OnDisable() override;

    Quat GetBBRotation(Particle& particle);

    Quat ScreenAlignBBoard();

    Quat WorldAlignBBoard(Particle& particle);

    Quat AxisAlignBBoard(Particle& particle);

#ifdef STANDALONE
    void OnEditor() override;
#endif

    void Serialization(json& j) override;
    void DeSerialization(json& j) override;

public:

    BILLBOARDTYPE typeofBBoard = BILLBOARDTYPE::SCREENALIGN;

    std::string BBtype[4] = { "Screen Align","World Align","Axis Align","No Align" };

    std::string currentBBoard;

    Application* app;

    float3 zBBoardAxis;
    float3 yBBoardAxis;
    float3 xBBoardAxis;

    Quat rotation;
};

