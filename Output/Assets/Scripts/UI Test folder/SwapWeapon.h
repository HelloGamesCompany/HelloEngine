#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Player/PlayerMove.h"

enum class infinitWeapon_Type
{
    HANDGUNS,
};

enum class normalWeapon_Type
{
    NONE,
    AUTO,
    BURST,
    SEMI,
    SHOTGUN,
    REVOLVER,
};

enum class specialWeapon_Type
{
    NONE,
    FLAMETHROWER,
    RICOCHET,
};



class SwapWeapon : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    //void SwapWeaponTexture(normalWeapon_Type normalWeapon);

    void SwapWeapon1(infinitWeapon_Type infinitWeapon, bool active);
    void SwapWeapon2(normalWeapon_Type normalWeapon, bool active);
    void SwapWeapon3(specialWeapon_Type specialWeapon, bool active);

    void Dash();

    //Normal Weapons on
    uint normalWeaponsOn[6];
    //Normal Weapons off
    uint normalWeaponsOff[6];

    //Inifit Weapons on
    uint infinitWeaponsOn[1];
    //Inifit Weapons off
    uint infinitWeaponsOff[1];
    
    //Especiales Weapons on
    uint specialsWeaponsOn[3];
    //Especiales Weapons off
    uint specialsWeaponsOff[3];

    //Weapon 1  
    API_Material material_Infinite_Weapon_on;
    API_Material material_Infinite_Weapon_off;

                 
    //Weapon 2   
    API_Material material_Normal_Weapon_on;
    API_Material material_Normal_Weapon_off;


    //Weapon 3  
    API_Material material_Special_Weapon_on;
    API_Material material_Special_Weapon_off;


    //Bars
    API_GameObject active_Bar_1;
    API_GameObject active_Bar_2;
    API_GameObject active_Bar_3;

    uint texture_Dash_on;
    uint texture_Dash_off;
    API_Material material_Dash;
    bool dashingReady;
    //swap weapon
    bool arma_Princial = false;
    bool arma_Secundaria = false;
    bool arma_Especial = false;

    API_GameObject playerMoveGO;
    PlayerMove* playerMove;
};

