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
        if (Save.TechCosts[CivVar].data[15] == 0) { GroundWeapon = WEAPON_LASER; }
        if (Save.TechCosts[CivVar].data[24] == 0) { GroundWeapon = WEAPON_PHASER; }
        if (Save.TechCosts[CivVar].data[32] == 0) { GroundWeapon = WEAPON_DISRUPTOR; }
        if (Save.TechCosts[CivVar].data[27] == 0) { GroundWeapon = WEAPON_PTORPEDO; }
    } else {
        GroundWeapon = WEAPON_PTORPEDO;
    }
    srand((unsigned) time(&t));

    if (ActPProjects->data[34]>0)
    {
        LandShield = it_round(1.6*ActPProjects->data[34]);
        do{
            switch (rand()%(5)) {
            case 0: ShipShield -= GroundWeapon; break;
            default: {
                    LandShield -= ShipWeapon;
                    HitCtr++;
                }
            }
        }
        while ((ShipShield>=0) && (LandShield>=0));
        if (LandShield <= 0)
        {
            ActPProjects->data[34] = 0;
            if (Visible)
            {
                PLAYSOUND(2,1100);
            }
        } else {
            ActPProjects->data[34] = it_round(LandShield*100.0/9.0/10.0);
        }
    }

    if ((ActPProjects->data[40]>0) && (ShipShield >= 0))
    {
        Delay(20);
        LandShield = it_round(6.4*ActPProjects->data[40]);
        do
        {
            switch (rand()%(5)) {
            case 0: ShipShield -= GroundWeapon; break;
            default: {
                    LandShield -= ShipWeapon;
                    HitCtr++;
                }
            }
        }
        while ((ShipShield>=0) && (LandShield>=0));
        if (LandShield <= 0)
        {
            ActPProjects->data[40] = 0;
            if (Visible)
            {
                PLAYSOUND(2,1100);
            }
        } else {
            ActPProjects->data[40] = it_round(LandShield*100.0/49.0/10.0);
        }
    }

    if (ShipShield >= 0)
    {
        if ((ShipShield - MyShipPtr->ShieldBonus - MyShipPtr->Tactical*3)>0)
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
