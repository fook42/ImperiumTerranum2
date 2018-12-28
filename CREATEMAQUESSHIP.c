#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool CREATEMAQUESSHIP(uint8 SysID, uint8 ShipID)
{
    r_ShipHeader*   ActShipPtr;
    int     i;

    if ((Year >= 2000) && ((rand()%4)>0)) { return false; }

    ActShipPtr = (r_ShipHeader*) AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
    if (NULL != ActShipPtr)
    {
        if (ShipID>8) { ShipID--; }

        *ActShipPtr = (r_ShipHeader) {0,ShipID,FLAG_MAQUES,0,0,0,0,0,0,
                                        ShipData(ShipID).MaxShield,1,1,-10,0,0,0,NULL,NULL,NULL};
                        /*Age,SType,Owner,Flags,ShieldBonus,Ladung,Fracht,PosX,PosY,
                        Shield,Weapon,Repair,Moving,Source,Target,Tactical,TargetShip,BeforeShip,NextShip*/

        ActShipPtr->Weapon = WEAPON_GUN;
        if (Save.TechCosts[ActPlayer-1].data[15] <= 0) { ActShipPtr->Weapon = WEAPON_LASER; }
        if (Save.TechCosts[ActPlayer-1].data[24] <= 0) { ActShipPtr->Weapon = WEAPON_PHASER; }
        if (Save.TechCosts[ActPlayer-1].data[32] <= 0) { ActShipPtr->Weapon = WEAPON_DISRUPTOR; }
        if (Save.TechCosts[ActPlayer-1].data[27] <= 0) { ActShipPtr->Weapon = WEAPON_PTORPEDO; }

        LINKSHIP(ActShipPtr, &(SystemHeader[SysID].FirstShip), 0);
        for(i = 0; i < (MAXCIVS-1); i++)
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
