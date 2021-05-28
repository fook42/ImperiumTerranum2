#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GOTOWAR(int CivFlag1, int CivFlag2)
{
    int     CivVar1, CivVar2;
    struct Window* GTW_Window;
    struct RastPort* RPort_PTR;

    CivVar1 = GETCIVVAR(CivFlag1);
    CivVar2 = GETCIVVAR(CivFlag2);
    CivFlag1 = CivFlag1 & FLAG_CIV_MASK;
    CivFlag2 = CivFlag2 & FLAG_CIV_MASK;
    Save.ImperatorState[CivVar1-1] -= 150;
    if ((0 == CivVar1) || (0 == CivVar2))
    {
        return;
    }
    if ((LEVEL_WAR == Save.WarState[CivVar1-1][CivVar2-1])
     || (LEVEL_WAR == Save.WarState[CivVar2-1][CivVar1-1]))
    {
        return;
    }
    Save.WarState[CivVar1-1][CivVar2-1] = LEVEL_WAR;
    Save.WarState[CivVar2-1][CivVar1-1] = LEVEL_WAR;

    if (WFLAG_JAHADR == Save.WorldFlag)
    {
        if (CIVVAR_OTHER == CivVar1) { Save.JSteuer[CivVar2-1] = 0; }
        if (CIVVAR_OTHER == CivVar2) { Save.JSteuer[CivVar1-1] = 0; }
    }

    if ((LEVEL_UNKNOWN == Save.WarState[ActPlayer-1][CivVar1-1])
     && (LEVEL_UNKNOWN == Save.WarState[ActPlayer-1][CivVar2-1]))
    {
        return;
    }

    GTW_Window = MAKEWINDOWBORDER(85,120,341,81,MyScreen[0]);
    if (NULL != GTW_Window)
    {
        RPort_PTR = GTW_Window->RPort;

        WRITE(171,11,CivFlag1,(1|WRITE_Center),RPort_PTR,3, GETCIVNAME(CivVar1));
        WRITE(171,31,      12,(1|WRITE_Center),RPort_PTR,3, _PT_beginnenKrieggegen);
        WRITE(171,51,CivFlag2,(1|WRITE_Center),RPort_PTR,3, GETCIVNAME(CivVar2));
        if (Save.PlayMySelf) { Delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(GTW_Window);
    }

    if ((0 != Save.CivPlayer[CivVar2-1]) && (!Save.PlayMySelf))
    {
        if (((Save.WarPower[CivVar1-1] > Save.WarPower[CivVar2-1]) && (0 == (rand()%40)))
         || ((CIVVAR_OTHER == CivVar1)
            && ((WFLAG_CEBORC == Save.WorldFlag)
             || (WFLAG_JAHADR == Save.WorldFlag)
             || (WFLAG_DCON   == Save.WorldFlag))
            && (0 == (rand()%10))))
        {
            QUOTEPICARD();
        }
    }
}
