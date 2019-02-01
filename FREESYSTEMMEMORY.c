#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FREESYSTEMMEMORY()
{
    r_PlanetHeader*     MyPlanetHeader;
    r_ShipHeader*       ActShipPtr;
    r_ShipHeader*       FleetShipPtr;
    uint8   i,j;
    APTR    FreePointer;

    for (i = 0; i < MAXSYSTEMS; ++i)
    {
        if (NULL != SystemHeader[i].PlanetMemA)
        {
            for (j = 0; j < SystemHeader[i].Planets; ++j)
            {
                MyPlanetHeader = (r_PlanetHeader*) &(SystemHeader[i].PlanetMemA[j]);
                if (NULL != MyPlanetHeader->ProjectPtr)
                {
                    FreeMem( MyPlanetHeader->ProjectPtr, sizeof(ByteArr42));
                }
                ActShipPtr = MyPlanetHeader->FirstShip.NextShip;
                if (NULL != ActShipPtr)
                {
                    if (SHIPTYPE_FLEET == ActShipPtr->SType)
                    {
                        FleetShipPtr = ActShipPtr->TargetShip;
                        while (NULL != FleetShipPtr)
                        {
                            FreePointer = (APTR) FleetShipPtr;
                            FleetShipPtr = FleetShipPtr->NextShip;
                            FreeMem( FreePointer, sizeof(r_ShipHeader) );
                        }
                    }
                    do
                    {
                        FreePointer = (APTR) ActShipPtr;
                        ActShipPtr = ActShipPtr->NextShip;
                        FreeMem( FreePointer, sizeof(r_ShipHeader) );
                    }
                    while (NULL != ActShipPtr);
                }
            }
            FreeMem(SystemHeader[i].PlanetMemA, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
        }
    }
}
