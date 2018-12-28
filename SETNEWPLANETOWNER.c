#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool SETNEWPLANETOWNER(r_PlanetHeader* MyPlanetHeader, r_PlanetHeader* OldPlanet, sint16* TheProject, uint8 ActSys, char** s1, char** s2, bool* PlanetLose)
{
    uint8           i, NewEthnoFlag;
    r_ShipHeader*   MyShipPtr;

    NewEthnoFlag = 0;
    if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != MyPlanetHeader->Ethno)
    {
        NewEthnoFlag = MyPlanetHeader->Ethno;
    } else {
        for(i = 0; i < (MAXCIVS-2); i++)
        {
            if ((Save.ImperatorState[i]>2000)
                && (Save.WarState[ActPlayer-1][i]!=LEVEL_UNKNOWN)
                && (Save.WarState[ActPlayer-1][i]!=LEVEL_DIED))
            {
                NewEthnoFlag = GETCIVFLAG(i+1);
            }
        }

        if (NewEthnoFlag != 0)
        {
            do
            {
               i = rand()%(MAXCIVS-2);
            }
            while ((Save.ImperatorState[i]<=2000)
                || (Save.WarState[ActPlayer-1][i]==LEVEL_UNKNOWN)
                || (Save.WarState[ActPlayer-1][i]==LEVEL_DIED));
            NewEthnoFlag = GETCIVFLAG(i+1);
        }
    }
    if (NewEthnoFlag == 0)
        { return false; }

    if (GetPlanetSys[GETCIVVAR(NewEthnoFlag)-1] != 0)
        { return false; }

    GetPlanet[GETCIVVAR(NewEthnoFlag)-1] = MyPlanetHeader;
    GetPlanetSys[GETCIVVAR(NewEthnoFlag)-1] = ActSys;
    memcpy(OldPlanet, MyPlanetHeader, sizeof(r_PlanetHeader));
    if (Save.CivPlayer[ActPlayer-1] != 0)
        { *TheProject = -1; }

    *s1 = PText[555];
    if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
    {
        *s2 = PText[556];
    }
    for(i = 0; i < (MAXCIVS-1); i++)
    {
        if (Save.WarState[i][GETCIVVAR(MyPlanetHeader->Ethno)-1] == LEVEL_DIED)
        {
            Save.WarState[i][GETCIVVAR(MyPlanetHeader->Ethno)-1] = LEVEL_PEACE;
        }
    }
    if (Save.WarState[GETCIVVAR(MyPlanetHeader->PFlags)-1][GETCIVVAR(MyPlanetHeader->Ethno)-1] == LEVEL_DIED)
    {
        Save.WarState[GETCIVVAR(MyPlanetHeader->PFlags)-1][GETCIVVAR(MyPlanetHeader->Ethno)-1] = LEVEL_COLDWAR;
    }
    MyPlanetHeader->PFlags = NewEthnoFlag;
    MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
    while (NULL != MyShipPtr)
    {
        if (0 != MyShipPtr->Owner) { MyShipPtr->Owner = NewEthnoFlag; }
        MyShipPtr = MyShipPtr->NextShip;
    }
    Save.ImperatorState[ActPlayer-1] += 35;
    if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
    {
        *s2 = PText[557];
    }
    *PlanetLose = true;

    return true;
}