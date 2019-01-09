#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool AUTOWATERTRANSPORT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    if ((CLASS_STONES  != MyPlanetHeader->Class)
     && (CLASS_GAS     != MyPlanetHeader->Class)
     && (CLASS_SATURN  != MyPlanetHeader->Class)
     && (CLASS_PHANTOM != MyPlanetHeader->Class))
    {
        if (0 == MyShipPtr->Fracht)
        {
            if ((MyPlanetHeader->Water / MyPlanetHeader->Size) > 56)
            {
                while ((MyShipPtr->Fracht < ShipData(MyShipPtr->SType).MaxLoad)
                        && (((MyPlanetHeader->Water-5) / MyPlanetHeader->Size) > 56))
                {
                    ++(MyShipPtr->Fracht);
                    MyPlanetHeader->Water -= 5;
                }
                (void) FINDNEXTPLANET(ActSys, MyShipPtr);
                if (MyShipPtr->Moving > 0)
                {
                    MyShipPtr->Moving = 0;
                }
                return true;
            }
        } else {
            if ((MyPlanetHeader->Water / MyPlanetHeader->Size) < 55)
            {
                while ((MyShipPtr->Fracht > 0)
                        && ((MyPlanetHeader->Water / MyPlanetHeader->Size) < 55))
                {
                    --(MyShipPtr->Fracht);
                    MyPlanetHeader->Water += 5;
                }
                (void) FINDNEXTPLANET(ActSys, MyShipPtr);
                if (MyShipPtr->Moving > 0)
                {
                    MyShipPtr->Moving = 0;
                }
                return true;
            }
        }
    }
    return false;
}
