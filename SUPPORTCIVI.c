#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SUPPORTCIVI(uint32 XValue)
{
    uint8   CivVar;
    if (WFLAG_JAHADR == Save.WorldFlag)
    {
        Save.Staatstopf[7] += XValue;
    } else if (MultiPlayer)
    {
        CivVar = rand()%(MAXCIVS-2);
        if ((0 == Save.CivPlayer[CivVar]) && (LEVEL_DIED != Save.WarState[CivVar][CivVar]))
        {
            Save.Staatstopf[CivVar] += XValue;
        }
    }
}
