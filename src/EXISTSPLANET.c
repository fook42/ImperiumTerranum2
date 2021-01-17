#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool EXISTSPLANET(uint8 CivVar, uint8 ActSys, uint8 Mode)
{
    r_PlanetHeader* MyPlanetHeader;
    uint8   i,CivFlag;

    CivFlag = GETCIVFLAG(CivVar);
    --ActSys;
    for(i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);

        if ((((1 == Mode) && ((MyPlanetHeader->Water / MyPlanetHeader->Size)>56))
          || ((2 == Mode) && ((MyPlanetHeader->Water / MyPlanetHeader->Size)<55)))
         && (MyPlanetHeader->Class!=CLASS_STONES) && (MyPlanetHeader->Class!=CLASS_GAS)
         && (MyPlanetHeader->Class!=CLASS_SATURN) && (MyPlanetHeader->Class!=CLASS_PHANTOM))
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
