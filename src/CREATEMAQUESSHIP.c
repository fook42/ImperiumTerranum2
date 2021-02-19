#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool CREATEMAQUESSHIP(const int SysID, int ShipID)
{
    r_ShipHeader*   ActShipPtr;
    int     i;

    if ((1999 < Year) && (0 != (rand()%4))) { return false; }

    ActShipPtr = (r_ShipHeader*) my_AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
    if (NULL != ActShipPtr)
    {
        if (8 < ShipID) { --ShipID; }

        *ActShipPtr = (r_ShipHeader) {0,ShipID,FLAG_MAQUES,0,0,0,0,0,0,
                                        ShipData(ShipID).MaxShield,1,1,-10,0,0,0,NULL,NULL,NULL};
                        /*Age,SType,Owner,Flags,ShieldBonus,Ladung,Fracht,PosX,PosY,
                        Shield,Weapon,Repair,Moving,Source,Target,Tactical,TargetShip,BeforeShip,NextShip*/

        ActShipPtr->Weapon = WEAPON_GUN;
        --ActPlayer; // to shift the array
        if (1 > Save.TechCosts[ActPlayer].data[15]) { ActShipPtr->Weapon = WEAPON_LASER; }
        if (1 > Save.TechCosts[ActPlayer].data[24]) { ActShipPtr->Weapon = WEAPON_PHASER; }
        if (1 > Save.TechCosts[ActPlayer].data[32]) { ActShipPtr->Weapon = WEAPON_DISRUPTOR; }
        if (1 > Save.TechCosts[ActPlayer].data[27]) { ActShipPtr->Weapon = WEAPON_PTORPEDO; }
        ++ActPlayer; // to shift the array

        LINKSHIP(ActShipPtr, &(SystemHeader[SysID].FirstShip), 0);
        for(i = 0; i < (MAXCIVS-1); ++i)
        {
            Save.WarState[8][i] = LEVEL_WAR;
            Save.WarState[i][8] = LEVEL_WAR;
            Save.LastWarState[8][i] = LEVEL_WAR;
            Save.LastWarState[i][8] = LEVEL_WAR;
        }
        Save.WarState[8][8] = LEVEL_PEACE;
    }
    return true;
}
