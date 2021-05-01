#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool EXISTSPLANET(int CivVar, int ActSys, int Mode)
{
    r_PlanetHeader* MyPlanetHeader;
    uint8   i, CivFlag;

    CivFlag = GETCIVFLAG(CivVar);
    for(i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);

        if ((((1 == Mode) && (56 < (MyPlanetHeader->Water / MyPlanetHeader->Size)))
          || ((2 == Mode) && (55 > (MyPlanetHeader->Water / MyPlanetHeader->Size))))
         && (CLASS_STONES  != MyPlanetHeader->Class)
         && (CLASS_GAS     != MyPlanetHeader->Class)
         && (CLASS_SATURN  != MyPlanetHeader->Class)
         && (CLASS_PHANTOM != MyPlanetHeader->Class))
        {
            if ((CivFlag == (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
             || (0       == (MyPlanetHeader->PFlags & FLAG_CIV_MASK)))
            {
                return true;
            } else if ((Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_WAR)
                    || (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_COLDWAR))
            {
                return true;
            }
        }
    }
    return false;
}
