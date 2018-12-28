#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SYSTEMTOENEMY(uint8 ActSys, uint8 NewOwner, uint8 OldOwner)
{
    uint8   i;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;

    for(i = 0; i < SystemHeader[ActSys].Planets; i++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);
        if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == OldOwner)
        {
            CHECKPROJECTS(MyPlanetHeader, NewOwner);
            MyPlanetHeader->PFlags = NewOwner;
            MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
            while (MyShipPtr != NULL)
            {
                MyShipPtr->PosX = it_round(MyPlanetHeader->PosX);
                MyShipPtr->PosY = it_round(MyPlanetHeader->PosY);
                LINKSHIP(MyShipPtr, &SystemHeader[ActSys].FirstShip, 1);
                MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
            }
        }
    }
}
