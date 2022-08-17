#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEJAHADR(const int CJ_ActPlayer)
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
    for(i = 0; i < 8; ++i)
    {
        Save.WarState[i][CJ_ActPlayer]     = LEVEL_DIED;
        Save.LastWarState[i][CJ_ActPlayer] = LEVEL_DIED;
        Save.WarState[i][7] = LEVEL_UNKNOWN;
        Save.WarState[7][i] = LEVEL_UNKNOWN;
    }
    Save.CivPlayer[7] = 0;
    if (2050 < Year) { Save.Staatstopf[7] += abs(Year)*120; }
    Save.TechCosts[7][15] = 0;
    Save.TechCosts[7][16] = 0;
    Save.GSteuer[7] = 0;
    for(i = 1; i < 43; ++i)
    {
        if (Save.TechCosts[CJ_ActPlayer][i] <= 0)
        {
            Save.TechCosts[7][i] = 0;
        }
    }
    for(i = 0; i < Save.Systems; ++i)
    {
        if (0 == SystemHeader[i].Planets) { continue; }
        for(j = 0; j < SystemHeader[i].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (NULL == MyPlanetHeader) { continue; }

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
                    MyPlanetHeader->ProjectPtr->data[PROJECT_SPACEDOCK]      = 1;
                    MyPlanetHeader->ProjectPtr->data[PROJECT_RECYCLINGPLANT] = 1;
                    MyPlanetHeader->ProjectPtr->data[PROJECT_HYDROPOWER]     = 1;
                    MyPlanetHeader->ProjectPtr->data[PROJECT_SETTLERS]      |= 16;
                    MyPlanetHeader->ProjectPtr->data[PROJECT_LANDINGTROOPS] |= 16;
                }
                MyPlanetHeader->ProjectID = PROJECT_NONE;

                MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                while (NULL != MyShipPtr)
                {
                    MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                    MyShipPtr = MyShipPtr->NextShip;
                }

            }
        }
    }
}
