#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PEACEINFO(uint8 CivVar1, uint8 CivVar2, uint8 CivFlag1, uint8 CivFlag2)
{
    struct Window* PEI_Window;
    struct RastPort* RPort_PTR;

    INFORMUSER();
    PEI_Window=MAKEWINDOW(85,120,341,101,MyScreen[0]);
    if (NULL != PEI_Window)
    {
        RPort_PTR = PEI_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,340,100,12,6,1);

        WRITE(170,12,CivFlag1,(1|WRITE_Center),RPort_PTR,3, GETCIVNAME(CivVar1));
        WRITE(170,32,      12,(1|WRITE_Center),RPort_PTR,3, _PT_und);
        WRITE(170,52,CivFlag2,(1|WRITE_Center),RPort_PTR,3, GETCIVNAME(CivVar2));
        WRITE(170,72,      12,(1|WRITE_Center),RPort_PTR,3, PText[232]);
        if (Save.PlayMySelf)
        {
            Delay(PAUSE);
        }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(PEI_Window);
    }
    if ((Save.WorldFlag >= CivFlag1) && (Save.WorldFlag <= CivFlag2)
        && ((CivVar1 == 8) || (CivVar2 == 8)))
    {
        STOPCIVILWAR(0);
    }
}
