#pragma once
#include "Component.h"

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

    Quat GetBBRotation();

    Quat ScreenAlignBBoard();

    Quat WorldAlignBBoard();

    Quat AxisAlignBBoard();

#ifdef STANDALONE
    void OnEditor() override;
#endif

    void Serialization(json& j) override;
    void DeSerialization(json& j) override;

public:

    BILLBOARDTYPE typeofBBoard = BILLBOARDTYPE::WORLDALIGN;

    std::string BBtype[4] = { "Screen Align","World Align","Axis Align","No Align" };

    Application* app;

    float3 zBBoardAxis;
    float3 yBBoardAxis;
    float3 xBBoardAxis;

    Quat rotation;
};

