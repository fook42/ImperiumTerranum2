#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REMOVEPLAYER(uint8 RP_ActPlayer)
{
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   ActShipPtr;
    uint8   i,j;
    uint8   RP_ActPlayerFlag;

    Save.Bevoelkerung[RP_ActPlayer-1] = 0;
    RP_ActPlayerFlag = GETCIVFLAG(RP_ActPlayer);
    for(i = 0; i < MAXSYSTEMS; ++i)
    {
        if (NULL != SystemHeader[i].PlanetMemA)
        {
            for(j = 0; j < SystemHeader[i].Planets; ++j)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (RP_ActPlayerFlag == (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                {
                    MyPlanetHeader->PFlags        = 0;
                    MyPlanetHeader->Ethno         = 0;
                    MyPlanetHeader->Population    = 0;
                    MyPlanetHeader->XProjectCosts = 0;
                    MyPlanetHeader->XProjectPayed = 0;
                    MyPlanetHeader->ProjectID     = 0;
                    if (NULL != MyPlanetHeader->ProjectPtr)
                    {
                        MyPlanetHeader->ProjectPtr->data[7]  = 0;
                        MyPlanetHeader->ProjectPtr->data[26] = 0;
                        MyPlanetHeader->ProjectPtr->data[27] = 0;
                        // free the memory???
                    }
                }
                MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                while (NULL != MyShipPtr)
                {
                    if (RP_ActPlayerFlag == MyShipPtr->Owner) { MyShipPtr->Owner = 0; }
                    MyShipPtr = MyShipPtr->NextShip;
                }
            }
            MyShipPtr = SystemHeader[i].FirstShip.NextShip;
            while (NULL != MyShipPtr)
            {
                if (RP_ActPlayerFlag == MyShipPtr->Owner) { MyShipPtr->Owner = 0; }
                if (SHIPTYPE_FLEET   == MyShipPtr->SType)
                {
                    ActShipPtr = MyShipPtr->TargetShip;
                    while (NULL != ActShipPtr)
                    {
                        if (RP_ActPlayerFlag == ActShipPtr->Owner) { ActShipPtr->Owner = 0; }
                        ActShipPtr = ActShipPtr->NextShip;
                    }
                }
                MyShipPtr = MyShipPtr->NextShip;
            }
        }
    }
}
