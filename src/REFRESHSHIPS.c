#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REFRESHSHIPS(r_ShipHeader* ShipPtr, const int SysID, const int Mode)
{
    r_ShipHeader*   ActShipPtr;
    r_ShipHeader*   BehindShipPtr;
    r_ShipHeader*   UseShipPtr;
    uint8           CivVar,CivVar2,i;

    if (NULL == ShipPtr) { return; }

    ActShipPtr = ShipPtr;

    do
    {
        CivVar = GETCIVVAR(ActShipPtr->Owner);
        while ((((ActShipPtr->SType<8) || (ActShipPtr->SType>24))
            && (ActShipPtr->SType != SHIPTYPE_FLEET)
            && (ActShipPtr->SType != TARGET_STARGATE))
            || (CivVar<1) || (CivVar>MAXCIVS) || (ActShipPtr->Owner == 0)
            || ((ActShipPtr->Age >= 200) && (ActShipPtr->Fracht == 0) && (ActShipPtr->Ladung == 0)
                && (ActShipPtr->SType != SHIPTYPE_FLEET)))
        {
            if ((CivVar>0) && (ActShipPtr->Age >= 200))
            {
                Verschrottung[CivVar-1]++;
            }
            BehindShipPtr = ActShipPtr->NextShip;

            ActShipPtr->BeforeShip->NextShip = ActShipPtr->NextShip;
            if (ActShipPtr->NextShip != NULL)
            {
                ActShipPtr->NextShip->BeforeShip = ActShipPtr->BeforeShip;
            }

            FreeMem((APTR) ActShipPtr, sizeof(r_ShipHeader));
            ActShipPtr = BehindShipPtr;
            if (ActShipPtr == NULL) { return; }
            CivVar = GETCIVVAR(ActShipPtr->Owner);
        }
        if (ActShipPtr->SType == SHIPTYPE_FLEET)
        {
            UseShipPtr = ActShipPtr->TargetShip;
        } else {
            UseShipPtr = ActShipPtr;
        }

        if (0 == Mode)
        {
            ActShipPtr->PosX = 0;
            ActShipPtr->PosY = 0;
        }
        if ((1 == Mode) && (ActShipPtr->NextShip != NULL))
        {
            CivVar2 = GETCIVVAR(ActShipPtr->NextShip->Owner);
            if ((CivVar2>0) && (UseShipPtr->Moving>0) && (UseShipPtr->NextShip->Moving>0))
            {
                if (Save.WarState[CivVar-1][CivVar2-1] == LEVEL_UNKNOWN)
                  { Save.WarState[CivVar-1][CivVar2-1] = LEVEL_PEACE; }
                if (Save.WarState[CivVar2-1][CivVar-1] == LEVEL_UNKNOWN)
                  { Save.WarState[CivVar2-1][CivVar-1] = LEVEL_PEACE; }
            }
        }
        if (ActShipPtr->Moving >= 0)
        {
            SystemFlags[CivVar-1][SysID] |= FLAG_KNOWN;
            for(i = 0; i < MAXCIVS; i++)
            {
                if (Save.WarState[i][CivVar-1] == LEVEL_ALLIANZ)
                {
                    SystemFlags[i][SysID] |= FLAG_KNOWN;
                }
            }
            ActShipPtr->Moving = ShipData(UseShipPtr->SType).MaxMove;
        }
        if (UseShipPtr->Shield < ShipData(UseShipPtr->SType).MaxShield)
        {
            if ((Save.CivPlayer[GETCIVVAR(UseShipPtr->Owner)-1] != 0) && (!Save.PlayMySelf) && (Mode == 1))
            {
                UseShipPtr->Shield = UseShipPtr->Shield + UseShipPtr->Repair;
                if (ActShipPtr->Moving>0)
                {
                    ActShipPtr->Moving = ActShipPtr->Moving - UseShipPtr->Repair;
                }
            } else {
                UseShipPtr->Shield = it_round(UseShipPtr->Shield*1.3)+1;
                ActShipPtr->Moving = it_round(ActShipPtr->Moving*0.77);
            }
        }
        if (ActShipPtr->Owner == FLAG_MAQUES)
        {
            MaquesShips++;
        }

        do
        {
            if (UseShipPtr->Shield > ShipData(UseShipPtr->SType).MaxShield)
            {
                UseShipPtr->Shield = ShipData(UseShipPtr->SType).MaxShield;
            }
            if (UseShipPtr->Age < 200)
            {
                UseShipPtr->Age++;
            } else if (UseShipPtr->Moving > 2)
            {
                UseShipPtr->Moving = ActShipPtr->Moving / 2;
            }
            Save.WarPower[CivVar-1]    += it_round((double) ShipData(UseShipPtr->SType).WeaponPower*(UseShipPtr->Weapon/10+1));
            Save.Bevoelkerung[CivVar-1]++;
            Save.Staatstopf[CivVar-1]  -= UseShipPtr->SType * 14;
            Militaerausgaben[CivVar-1] += UseShipPtr->SType * 14;
            if (ActShipPtr->SType == SHIPTYPE_FLEET)
            {
                UseShipPtr = UseShipPtr->NextShip;
            }
        }
        while ((ActShipPtr->SType == SHIPTYPE_FLEET) && (NULL != UseShipPtr));
        if (ActShipPtr->NextShip != NULL)
        {
            if (ActShipPtr->NextShip->BeforeShip != ActShipPtr)
            {
                ActShipPtr->NextShip = NULL;
            }
        }
        ActShipPtr = ActShipPtr->NextShip;
    }
    while (ActShipPtr != NULL);
}
