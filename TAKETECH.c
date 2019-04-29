#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool TAKETECH(uint8 CivFlag1, uint8 CivFlag2)
{
    uint8   i,TechID,CivVar1,CivVar2;
    char    s[60];
    bool    _TAKETECH = false;
    struct Window* TKT_Window;
    struct RastPort* RPort_PTR;

    CivVar1 = GETCIVVAR(CivFlag1);
    CivVar2 = GETCIVVAR(CivFlag2);
    TechID = 0;
    for (i = 41; i>=1; i--)
    {
        if ((0 <  Save.TechCosts[CivVar1-1].data[i]) &&
            (0 >= Save.TechCosts[CivVar2-1].data[i]))
        {
            TechID = i;
        }
    }
    if (0 != TechID)
    {
        if (( (CivVar1 == ActPlayer)
           || (CivVar2 == ActPlayer))
        && (  (0 != Save.CivPlayer[CivVar1-1])
           || (0 != Save.CivPlayer[CivVar2-1])))
        {
            TKT_Window=MAKEWINDOW(85,118,341,83,MyScreen[0]);
            if (NULL != TKT_Window)
            {
                RPort_PTR = TKT_Window->RPort;
                MAKEWINBORDER(RPort_PTR,0,0,340,82,12,6,1);

                strcpy(s, GETCIVNAME(CivVar1));
                strcat(s, " ");
                strcat(s, _PT_uebernehmen_Technologie);
                strcat(s, ":");

                WRITE(171,22,CivFlag1,(1|WRITE_Center),RPort_PTR,4, s);
                WRITE(171,42,      12,(1|WRITE_Center),RPort_PTR,4, TechnologyL.data[TechID]);
                if (Save.PlayMySelf)
                {
                    delay(PAUSE);
                }
                WAITLOOP(Save.PlayMySelf);
                CloseWindow(TKT_Window);
            }
            if (CivFlag1 == ActPlayerFlag)
            {
                DISPLAYTECH(TechID);
            }
        }
        Save.TechCosts[CivVar1-1].data[TechID] = 0;
        if (Save.ActTech[CivVar1-1] == TechID)
        {
            Save.ActTech[CivVar1-1] = 0;
        }
        _TAKETECH = true;
    }
    return _TAKETECH;
}
