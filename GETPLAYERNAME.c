#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GETPLAYERNAME(uint8 GPN_ActPlayer, char* PName)
{
    struct Window*   GPN_Window;
    struct RastPort* RPort_PTR;
    struct IntuiMessage*  IMsg;
    uint8   VKey = 0;
    int     PName_len = 0;
    char    s[40];

    GPN_Window=MAKEWINDOW(0,0,640,512,MyScreen[1]);
    if (NULL == GPN_Window)
    {
        strcpy(PName, "PlayerX");
        return;
    }
    RPort_PTR = GPN_Window->RPort;

    SetRGB4(&(MyScreen[1]->ViewPort),1,15,15,15);
    SetRGB4(&(MyScreen[1]->ViewPort),2,3,3,15);
    SetRGB4(&(MyScreen[1]->ViewPort),3,9,9,9);
    WRITEWIN(320,100,1,WRITE_Center,RPort_PTR,4,PText[160]);
    if (!MultiPlayer)
    {
        WRITEWIN(320,150,2,WRITE_Center,RPort_PTR,4,PText[161]);
    } else {
        strcpy(s, "Player 0");
        s[strlen(s)-1]=Save.CivPlayer[GPN_ActPlayer]+'0';
        strcat(s, PText[162]);
        WRITEWIN(320,150,2,WRITE_Center,RPort_PTR,4,s);
    }
    MAKEWINBORDER(RPort_PTR,100,200,540,230,1,3,1);
    PName[0] = 0;
    PName_len = 0;
    ScreenToFront(MyScreen[1]);
    do
    {
        IMsg = (struct IntuiMessage*) WaitPort(GPN_Window->UserPort);
        if (NULL != IMsg)
        {
            IMsg = (struct IntuiMessage*) GetMsg(GPN_Window->UserPort);
            if (VANILLAKEY == IMsg->Class)
            {
                VKey = IMsg->Code;
            }
            ReplyMsg((struct Message *)IMsg);
        }
        if ((8 == VKey) && (PName_len > 0))
        {
             PName[--PName_len] = 0;
             RECTWIN(RPort_PTR,0,102,202,538,228);
        }
        if ((VKey>30) && ((PName_len+1) < HISCORE_NAMELEN))
        {
            PName[PName_len++] = VKey;
            PName[PName_len] = 0;
        }
        WRITEWIN(320,208,2,(1|WRITE_Center),RPort_PTR,4,PName);
    }
    while (VKey != 13);
    CloseWindow(GPN_Window);
}
