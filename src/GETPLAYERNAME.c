#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GETPLAYERNAME(uint8 GPN_ActPlayer, char* PName)
{
    struct NewWindow GPN_NewWindow = {0,0,640,512,0,0,VANILLAKEY,
                                      BORDERLESS+BACKDROP+ACTIVATE+SIMPLE_REFRESH,
                                      NULL,NULL,NULL,MyScreen[1],NULL,
                                      0,0,0,0,CUSTOMSCREEN};

    struct Window*   GPN_Window;
    struct RastPort* RPort_PTR;
    struct IntuiMessage*  IMsg;
    uint8   VKey = 0;
    int     PName_len;
    char    s[40];
    char*   _s;

    GPN_Window = OpenWindow(&GPN_NewWindow);
    if (NULL == GPN_Window)
    {
        (void) my_strcpy(PName, "Player X");
        ScreenToFront(MyScreen[1]);
        return;
    }
    RPort_PTR = GPN_Window->RPort;

    SetRGB32(MyVPort_PTR[1],1,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF);
    SetRGB32(MyVPort_PTR[1],2,0x33333333,0x33333333,0xFFFFFFFF);
    SetRGB32(MyVPort_PTR[1],3,0x99999999,0x99999999,0x99999999);
    WRITE(320,100,1,WRITE_Center,RPort_PTR,3,PText[160]);
    if (!MultiPlayer)
    {
        WRITE(320,150,2,WRITE_Center,RPort_PTR,3,PText[161]);
    } else {
        _s=my_strcpy(s, _PTx_Player);
        *_s++ = ' ';
        *_s++ = Save.CivPlayer[GPN_ActPlayer]+'0';
        (void) my_strcpy(_s, PText[162]);
        WRITE(320,150,2,WRITE_Center,RPort_PTR,3,s);
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
        if ((8 == VKey) && (0 < PName_len))
        {
            PName[--PName_len] = 0;
            RECTWIN(RPort_PTR,0,102,202,538,228);
        }
        if ((30 < VKey) && (HISCORE_NAMELEN > (PName_len+1)))
        {
            PName[PName_len++] = VKey;
            PName[PName_len] = 0;
        }
        WRITE(320,208,2,(1|WRITE_Center),RPort_PTR,3,PName);
    }
    while (13 != VKey);
    CloseWindow(GPN_Window);
}
