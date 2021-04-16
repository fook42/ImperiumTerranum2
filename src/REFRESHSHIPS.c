#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REFRESHSHIPS(r_ShipHeader* ShipPtr, const int SysID, const int Mode)
{
    r_ShipHeader*   ActShipPtr;
    r_ShipHeader*   BehindShipPtr;
    r_ShipHeader*   UseShipPtr;
    int             CivVar,CivVar2,i;

    ActShipPtr = ShipPtr;

    do
    {
        CivVar = GETCIVVAR(ActShipPtr->Owner);
        while ((((8 > ActShipPtr->SType) || (24 < ActShipPtr->SType)) && (SHIPTYPE_FLEET != ActShipPtr->SType) && (TARGET_STARGATE != ActShipPtr->SType))
            || ((1 > CivVar) || (MAXCIVS < CivVar))
            || (0 == ActShipPtr->Owner)
            || ((200 <= ActShipPtr->Age) && (0 == ActShipPtr->Fracht) && (0 == ActShipPtr->Ladung)
                && (SHIPTYPE_FLEET != ActShipPtr->SType)))
        {
            if ((0 < CivVar) && (200 <= ActShipPtr->Age))
            {
                Verschrottung[CivVar-1]++;
            }
            BehindShipPtr = ActShipPtr->NextShip;

            ActShipPtr->BeforeShip->NextShip = ActShipPtr->NextShip;
            if (NULL != ActShipPtr->NextShip)
            {
                ActShipPtr->NextShip->BeforeShip = ActShipPtr->BeforeShip;
            }
            printf("%s (%d) FreeMem of ShipHeader = %08lX\n", __FILE__, __LINE__, (uint32) ActShipPtr);
            FreeMem((APTR) ActShipPtr, sizeof(r_ShipHeader));
            ActShipPtr = BehindShipPtr;
            if (NULL == ActShipPtr) { return; }
            CivVar = GETCIVVAR(ActShipPtr->Owner);
        }
        --CivVar; // ..to shift the arrays..

        if (SHIPTYPE_FLEET == ActShipPtr->SType)
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
        if ((1 == Mode) && (NULL != ActShipPtr->NextShip))
        {
            CivVar2 = GETCIVVAR(ActShipPtr->NextShip->Owner);
            if ((0 < CivVar2) && (0 < UseShipPtr->Moving) && (0 < UseShipPtr->NextShip->Moving))
            {
                --CivVar2;
                if (Save.WarState[CivVar][CivVar2] == LEVEL_UNKNOWN)
                  { Save.WarState[CivVar][CivVar2] = LEVEL_PEACE; }
                if (Save.WarState[CivVar2][CivVar] == LEVEL_UNKNOWN)
                  { Save.WarState[CivVar2][CivVar] = LEVEL_PEACE; }
            }
        }
        if (0 <= ActShipPtr->Moving)
        {
            SystemFlags[CivVar][SysID] |= FLAG_KNOWN;
            for(i = 0; i < MAXCIVS; i++)
            {
                if (LEVEL_ALLIANZ == Save.WarState[i][CivVar])
                {
                    SystemFlags[i][SysID] |= FLAG_KNOWN;
                }
            }
            ActShipPtr->Moving = ShipData(UseShipPtr->SType).MaxMove;
        }
        if (ShipData(UseShipPtr->SType).MaxShield > UseShipPtr->Shield)
        {
            if ((0 != Save.CivPlayer[GETCIVVAR(UseShipPtr->Owner)-1]) && (!Save.PlayMySelf) && (1 == Mode))
            {
                UseShipPtr->Shield = UseShipPtr->Shield + UseShipPtr->Repair;
                if (0 < ActShipPtr->Moving)
                {
                    ActShipPtr->Moving = ActShipPtr->Moving - UseShipPtr->Repair;
                }
            } else {
                UseShipPtr->Shield = it_round(UseShipPtr->Shield*1.3)+1;
                ActShipPtr->Moving = it_round(ActShipPtr->Moving*0.77);
            }
        }
        if (FLAG_MAQUES == ActShipPtr->Owner)
        {
            ++MaquesShips;
        }

        do
        {
            if (ShipData(UseShipPtr->SType).MaxShield < UseShipPtr->Shield)
            {
                UseShipPtr->Shield = ShipData(UseShipPtr->SType).MaxShield;
            }
            if (200 > UseShipPtr->Age)
            {
                UseShipPtr->Age++;
            } else if (2 < UseShipPtr->Moving)
            {
                UseShipPtr->Moving = ActShipPtr->Moving / 2;
            }
            Save.WarPower[CivVar]    += it_round((double) ShipData(UseShipPtr->SType).WeaponPower*(UseShipPtr->Weapon/10+1));
            Save.Bevoelkerung[CivVar]++;
            Save.Staatstopf[CivVar]  -= UseShipPtr->SType * 14;
            Militaerausgaben[CivVar] += UseShipPtr->SType * 14;
            if (SHIPTYPE_FLEET == ActShipPtr->SType)
            {
                UseShipPtr = UseShipPtr->NextShip;
            }
        }
        while ((SHIPTYPE_FLEET == ActShipPtr->SType) && (NULL != UseShipPtr));
        if (NULL != ActShipPtr->NextShip)
        {
            if (ActShipPtr->NextShip->BeforeShip != ActShipPtr)
            {
                ActShipPtr->NextShip = NULL;
            }
        }
        ActShipPtr = ActShipPtr->NextShip;
    }
    while (NULL != ActShipPtr);
}
