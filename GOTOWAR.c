#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GOTOWAR(uint8 CivFlag1, uint8 CivFlag2)
{
    uint8   CivVar1,CivVar2;
    struct Window* GTW_Window;
    struct RastPort* RPort_PTR;

    CivVar1 = GETCIVVAR(CivFlag1);
    CivVar2 = GETCIVVAR(CivFlag2);
    CivFlag1 = CivFlag1 & FLAG_CIV_MASK;
    CivFlag2 = CivFlag2 & FLAG_CIV_MASK;
    Save.ImperatorState[CivVar1-1] -= 150;
    if ((0 == CivVar1) || (0 == CivVar2)
        || (Save.WarState[CivVar1-1][CivVar2-1]==LEVEL_WAR)
        || (Save.WarState[CivVar2-1][CivVar1-1]==LEVEL_WAR))
    {
        return;
    }
    Save.WarState[CivVar1-1][CivVar2-1] = LEVEL_WAR;
    Save.WarState[CivVar2-1][CivVar1-1] = LEVEL_WAR;
    if ((8 == CivVar1) && (Save.WorldFlag==WFLAG_JAHADR)) { Save.JSteuer[CivVar2-1] = 0; }
    if ((8 == CivVar2) && (Save.WorldFlag==WFLAG_JAHADR)) { Save.JSteuer[CivVar1-1] = 0; }
    if ((Save.WarState[ActPlayer-1][CivVar1-1]==LEVEL_UNKNOWN)
      &&(Save.WarState[ActPlayer-1][CivVar2-1]==LEVEL_UNKNOWN))
    {
        return;
    }

    GTW_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
    if (NULL == GTW_Window)
    {
        return;
    }
    RPort_PTR = GTW_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);
    WRITEWIN(171,11,CivFlag1,(1|WRITE_Center),RPort_PTR,4, GETCIVNAME(CivVar1));
    WRITEWIN(171,31,      12,(1|WRITE_Center),RPort_PTR,4, _PT_beginnenKrieggegen);
    WRITEWIN(171,51,CivFlag2,(1|WRITE_Center),RPort_PTR,4, GETCIVNAME(CivVar2));
    if (Save.PlayMySelf) { delay(PAUSE); }
    WAITLOOP(Save.PlayMySelf);
    CloseWindow(GTW_Window);

    if ((0 != Save.CivPlayer[CivVar2-1]) && (!Save.PlayMySelf))
    {
        if (((Save.WarPower[CivVar1-1] > Save.WarPower[CivVar2-1]) && (0==(rand()%40)))
         || ((CivVar1==8)
            && ((Save.WorldFlag==WFLAG_CEBORC)
             || (Save.WorldFlag==WFLAG_JAHADR)
             || (Save.WorldFlag==WFLAG_DCON))
            && (0==(rand()%10))))
        {
            QUOTEPICARD();
        }
    }
}
