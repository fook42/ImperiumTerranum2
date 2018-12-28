#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool FINDOBJECT(uint8 ActSys, sint16 GetX, sint16 GetY, void* ExcludeObj)
{
    r_ShipHeader*   ActShipPtr;
    r_PlanetHeader* PlanetHeader;
    sint16          x,y;
    uint8           i,j;

    ObjPtr = NULL;
    ObjType = 0;
    ActShipPtr = (r_ShipHeader*) ExcludeObj;

    if ((SystemHeader[ActSys].FirstShip.SType == TARGET_STARGATE)
     && (ActShipPtr != NULL) && (ActShipPtr->SType != TARGET_STARGATE))
    {
        x = 256+(SystemHeader[ActSys].FirstShip.PosX+OffsetX)*32;
        y = 256+(SystemHeader[ActSys].FirstShip.PosY+OffsetY)*32;
        if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)) /* && (PlanetHeader != (r_PlanetHeader*) ExcludeObj) // ... nonsense ...*/ )
        {
            ObjType = TYPE_STARGATE;
            return true;
        }
    }
    i = 0;
    do
    {
        PlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);
        x = 256+((it_round(PlanetHeader->PosX)+OffsetX)*32);
        y = 256+((it_round(PlanetHeader->PosY)+OffsetY)*32);

        if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)) && (PlanetHeader != (r_PlanetHeader*) ExcludeObj))
        {
            ObjType = TYPE_PLANET;
            ObjPtr = PlanetHeader;
            return true;
        }
        i++;
    }
    while (i<SystemHeader[ActSys].Planets);
    if (SystemHeader[ActSys].FirstShip.NextShip != NULL)
    {
        ActShipPtr = SystemHeader[ActSys].FirstShip.NextShip;
        do
        {
            x = 256+((ActShipPtr->PosX+OffsetX)*32);
            y = 256+((ActShipPtr->PosY+OffsetY)*32);
            if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)) && (ActShipPtr != (r_ShipHeader*) ExcludeObj)
                && (ActShipPtr->Owner != 0) && (ActShipPtr->Moving >= 0))
            {
                ObjType = TYPE_SHIP;
                ObjPtr = ActShipPtr;
                return true;
            }
            ActShipPtr = ActShipPtr->NextShip;
        }
        while (ActShipPtr != NULL);
    }
    for(i = 0; i < MAXHOLES; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if (MyWormHole[i].System[j] == (ActSys+1))
            {
                x = 256+((MyWormHole[i].PosX[j]+OffsetX)*32);
                y = 256+((MyWormHole[i].PosY[j]+OffsetY)*32);
                if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)))
                {
                    ObjType = TYPE_WORMHOLE;
                    // ObjPtr = NULL; // .. not needed.. ObjPtr was set to NULL before..
                    return true;
                }
            }
        }
    }
    return false;
}