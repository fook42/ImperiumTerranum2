#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool EXISTSPLANET(uint8 CivVar, uint8 ActSys, uint8 Mode)
{
    r_PlanetHeader* MyPlanetHeader;
    uint8   i,CivFlag;

    CivFlag = GETCIVFLAG(CivVar);
    for(i = 0; i < SystemHeader[ActSys-1].Planets; i++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[i]);

        if ((((Mode == 1) && ((MyPlanetHeader->Water / MyPlanetHeader->Size)>56))
          || ((Mode == 2) && ((MyPlanetHeader->Water / MyPlanetHeader->Size)<55)))
         && (MyPlanetHeader->Class!=CLASS_STONES) && (MyPlanetHeader->Class!=CLASS_GAS)
         && (MyPlanetHeader->Class!=CLASS_SATURN) && (MyPlanetHeader->Class!=CLASS_PHANTOM))
        {
            if (((MyPlanetHeader->PFlags & FLAG_CIV_MASK)==CivFlag)
             || ((MyPlanetHeader->PFlags & FLAG_CIV_MASK)==0))
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
