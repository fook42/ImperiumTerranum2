#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool AUTOWATERTRANSPORT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    bool return_value = false;

    if (0 != ClassTransFactor[MyPlanetHeader->Class])
    {
        if (0 == MyShipPtr->Fracht)
        {
            if (56 < (MyPlanetHeader->Water / MyPlanetHeader->Size))
            {
                while ((ShipData(MyShipPtr->SType).MaxLoad > MyShipPtr->Fracht)
                        && (56 < ((MyPlanetHeader->Water-5) / MyPlanetHeader->Size)))
                {
                    ++(MyShipPtr->Fracht);
                    MyPlanetHeader->Water -= 5;
                }
                (void) FINDNEXTPLANET(ActSys-1, MyShipPtr);
                if (0 < MyShipPtr->Moving)
                {
                    MyShipPtr->Moving = 0;
                }
                return_value = true;
            }
        } else {
            if (55 > (MyPlanetHeader->Water / MyPlanetHeader->Size))
            {
                while ((0 < MyShipPtr->Fracht)
                        && (55 > (MyPlanetHeader->Water / MyPlanetHeader->Size)))
                {
                    --(MyShipPtr->Fracht);
                    MyPlanetHeader->Water += 5;
                }
                (void) FINDNEXTPLANET(ActSys-1, MyShipPtr);
                if (0 < MyShipPtr->Moving)
                {
                    MyShipPtr->Moving = 0;
                }
                return_value = true;
            }
        }
    }
    return return_value;
}
