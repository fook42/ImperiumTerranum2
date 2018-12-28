#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEJAHADR(uint8 CJ_ActPlayer)
{
    int i,j;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;

    if ((0            != Save.WorldFlag) &&
        (WFLAG_JAHADR != Save.WorldFlag))
    {
        return;
    }
    Save.WorldFlag = WFLAG_JAHADR;
    for(i = 0; i < 8; i++)
    {
        Save.WarState[i][CJ_ActPlayer]     = LEVEL_DIED;
        Save.LastWarState[i][CJ_ActPlayer] = LEVEL_DIED;
        Save.WarState[i][7] = LEVEL_UNKNOWN;
        Save.WarState[7][i] = LEVEL_UNKNOWN;
    }
    Save.CivPlayer[7] = 0;
    if (Year>2050) { Save.Staatstopf[7] += abs(Year)*120; }
    Save.TechCosts[7].data[15] = 0;
    Save.TechCosts[7].data[16] = 0;
    Save.GSteuer[7] = 0;
    for(i = 1; i <= 42; i++)
    {
        if (Save.TechCosts[CJ_ActPlayer].data[i] <= 0)
        {
            Save.TechCosts[7].data[i] = 0;
        }
    }
    for(i = 0; i < Save.Systems; i++)
    {
        for(j = 0; j < SystemHeader[i].Planets; j++)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (NULL != MyPlanetHeader)
            {
                if (GETCIVVAR(MyPlanetHeader->PFlags) == (CJ_ActPlayer+1))
                {
                    MyPlanetHeader->PFlags = FLAG_OTHER;
                    MyPlanetHeader->Ethno = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                    Save.Bevoelkerung[7] += MyPlanetHeader->Population;
                    MyPlanetHeader->Biosphaere    = 200;
                    MyPlanetHeader->Infrastruktur = 200;
                    MyPlanetHeader->Industrie     = 200;
                    if (NULL != MyPlanetHeader->ProjectPtr)
                    {
                        MyPlanetHeader->ProjectPtr->data[1]  = 1;
                        MyPlanetHeader->ProjectPtr->data[25] = 1;
                        MyPlanetHeader->ProjectPtr->data[30] = 1;
                        MyPlanetHeader->ProjectPtr->data[32] = 1;
                        MyPlanetHeader->ProjectPtr->data[26] = MyPlanetHeader->ProjectPtr->data[26] | 16;
                        MyPlanetHeader->ProjectPtr->data[27] = MyPlanetHeader->ProjectPtr->data[27] | 16;
                    }
                    MyPlanetHeader->ProjectID = 0;
                    if (NULL != MyPlanetHeader->FirstShip.NextShip)
                    {
                        MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                        do
                        {
                            MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                            MyShipPtr = MyShipPtr->NextShip;
                        }
                        while (NULL != MyShipPtr);
                    }
                }
            }
        }
    }
}
