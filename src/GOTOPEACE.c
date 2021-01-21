#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GOTOPEACE(int CivVar1, int CivVar2)
{
    const int GP_ActPlayer = ActPlayer-1;

    Save.WarState[CivVar1][CivVar2] = LEVEL_PEACE;
    Save.WarState[CivVar2][CivVar1] = LEVEL_PEACE;
    if ((LEVEL_UNKNOWN != Save.WarState[CivVar1][GP_ActPlayer])
     || (LEVEL_UNKNOWN != Save.WarState[CivVar2][GP_ActPlayer]))
    {
        if (LEVEL_ALLIANZ == Save.WarState[CivVar1][GP_ActPlayer])
        {
            Save.WarState[CivVar1][GP_ActPlayer] = LEVEL_PEACE;
            Save.WarState[GP_ActPlayer][CivVar1] = LEVEL_PEACE;
        }
        if (LEVEL_ALLIANZ == Save.WarState[CivVar2][GP_ActPlayer])
        {
            Save.WarState[CivVar2][GP_ActPlayer] = LEVEL_PEACE;
            Save.WarState[GP_ActPlayer][CivVar2] = LEVEL_PEACE;
        }
        PEACEINFO(CivVar1+1,CivVar2+1,GETCIVFLAG(CivVar1+1),GETCIVFLAG(CivVar2+1));
    }
}
