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

    // check if we are close to a Stargate ?
    if (NULL != ExcludeObj)
    {
        ActShipPtr = (r_ShipHeader*) ExcludeObj;

        if ((TARGET_STARGATE == SystemHeader[ActSys].FirstShip.SType)
         && (TARGET_STARGATE != ActShipPtr->SType))
        {
            x = 256+((SystemHeader[ActSys].FirstShip.PosX+OffsetX)*32);
            y = 256+((SystemHeader[ActSys].FirstShip.PosY+OffsetY)*32);
            if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)))
            {
                ObjType = TYPE_STARGATE;
                return true;
            }
        }
    }

    // are we close to a Planet ?
    for(i = 0; i < SystemHeader[ActSys].Planets; ++i)
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
    }

    // or maybe close to another Ship ?
    ActShipPtr = SystemHeader[ActSys].FirstShip.NextShip;
    while (NULL != ActShipPtr)
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

    // last try; are we close to a worm-hole ?
    for(i = 0; i < MAXHOLES; ++i)
    {
        for(j = 0; j < 2; ++j)
        {
            if (MyWormHole[i].System[j] == (ActSys+1))
            {
                x = 256+((MyWormHole[i].PosX[j]+OffsetX)*32);
                y = 256+((MyWormHole[i].PosY[j]+OffsetY)*32);
                if ((GetX>=x) && (GetX<=(x+32)) && (GetY>=y) && (GetY<=(y+32)))
                {
                    ObjType = TYPE_WORMHOLE;
                    return true;
                }
            }
        }
    }
    return false;
}