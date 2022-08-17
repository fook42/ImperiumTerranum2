#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MOVESHIP_SMALLANDING(r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys)
{
    uint8 CivVar;   // not used from/to outside .. so we dont need the CivVar from MOVESHIP()...
    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProjects;
    sint16  ShipShield,ShipWeapon,GroundWeapon;
    sint16  LandShield;
    bool    Visible;
    uint8   HitCtr;
    time_t  t;

    MyPlanetHeader = PPtr;
    MyShipPtr = SPtr; // ShipPtr ???
    HitCtr = 0;
    if ((Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0)
            || ((MyPlanetHeader->PFlags>0) && (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0))
            || (Display == ActSys))
    {
        Visible = true;
    } else {
        Visible = false;
    }

    ActPProjects = MyPlanetHeader->ProjectPtr;
    ShipShield = (MyShipPtr->Shield + MyShipPtr->Tactical*3 + MyShipPtr->ShieldBonus);
    ShipWeapon = it_round((double) (MyShipPtr->Weapon/10.0+1.0)*ShipData(MyShipPtr->SType).WeaponPower - MyShipPtr->Tactical);
    CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
    if (CivVar != 0)
    {
        CivVar--; // to shift the Save-array
        GroundWeapon = WEAPON_GUN;
        if (Save.TechCosts[CivVar][15] == 0) { GroundWeapon = WEAPON_LASER; }
        if (Save.TechCosts[CivVar][24] == 0) { GroundWeapon = WEAPON_PHASER; }
        if (Save.TechCosts[CivVar][32] == 0) { GroundWeapon = WEAPON_DISRUPTOR; }
        if (Save.TechCosts[CivVar][27] == 0) { GroundWeapon = WEAPON_PTORPEDO; }
    } else {
        GroundWeapon = WEAPON_PTORPEDO;
    }
    srand((unsigned) time(&t));

    if (0 < ActPProjects->data[PROJECT_SDI])
    {
        LandShield = it_round(1.6*ActPProjects->data[PROJECT_SDI]);
        do
        {
            if (0 == (rand()%5))
            {
                ShipShield -= GroundWeapon;
            } else {
                LandShield -= ShipWeapon;
                ++HitCtr;
            }
        }
        while ((0 <= ShipShield) && (0 <= LandShield));
        if (0 >= LandShield)
        {
            ActPProjects->data[PROJECT_SDI] = 0;
            if (Visible)
            {
                PLAYSOUND(1,1100);
            }
        } else {
            ActPProjects->data[PROJECT_SDI] = it_round(LandShield*100.0/9.0/10.0);
        }
    }

    if ((0 < ActPProjects->data[PROJECT_SPACEPHALANX]) && (0 <= ShipShield))
    {
        Delay(20);
        LandShield = it_round(6.4*ActPProjects->data[PROJECT_SPACEPHALANX]);
        do
        {
            if (0 == (rand()%5))
            {
                ShipShield -= GroundWeapon;
            } else {
                LandShield -= ShipWeapon;
                ++HitCtr;
            }
        }
        while ((0 <= ShipShield) && (0 <= LandShield));
        if (0 >= LandShield)
        {
            ActPProjects->data[PROJECT_SPACEPHALANX] = 0;
            if (Visible)
            {
                PLAYSOUND(1,1100);
            }
        } else {
            ActPProjects->data[PROJECT_SPACEPHALANX] = it_round(LandShield*100.0/49.0/10.0);
        }
    }

    if (0 <= ShipShield)
    {
        if (0 < (ShipShield - MyShipPtr->ShieldBonus - MyShipPtr->Tactical*3))
        {
            MyShipPtr->Shield = ShipShield - MyShipPtr->ShieldBonus - MyShipPtr->Tactical*3;
        } else {
            MyShipPtr->Shield = 1;
        }
        MyShipPtr->ShieldBonus++;
        REFRESHDISPLAY();
        Delay(10);
    } else {
        if (Visible)
        {
            MOVESHIP_EXPLODE(ActSys,MyShipPtr);
        }
        MyShipPtr->Owner = 0;
    }
}
