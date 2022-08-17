#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DECREASE(uint8 ActSys, r_PlanetHeader* PPtr)
{
    APTR    ModC = NULL;
    r_PlanetHeader* MyPlanetHeader;
    char    s[40];
    char*   _s;
    struct Window* DEC_Window;
    struct RastPort* RPort_PTR;

    MyPlanetHeader = PPtr;
    INFORMUSER();
    ModC = GETTHESOUND(3);

    DEC_Window = MAKEWINDOWBORDER(80,120,351,101,MyScreen[0]);
    if (NULL != DEC_Window)
    {
        RPort_PTR = DEC_Window->RPort;


        _s=my_strcpy(s, _PT_System);
        *_s++ = ':';
        *_s++ = ' ';
        (void) my_strcpy(_s, Save.SystemName[ActSys-1]);
        WRITE(176, 7,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        _s=my_strcpy(s, _PT_Planet);
        *_s++ = ':';
        *_s++ = ' ';
        (void) my_strcpy(_s, MyPlanetHeader->PName);
        WRITE(176,27,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        WRITE(176,50,12,WRITE_Center,RPort_PTR,3,PText[572]);
        WRITE(176,70,12,WRITE_Center,RPort_PTR,3,PText[573]);
        if (Save.PlayMySelf)
        {
            Delay(PAUSE);
        }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(DEC_Window);
    }
    HANDLEKNOWNPLANET(ActSys,0,MyPlanetHeader);
    if (NULL != ModC)
    {
        StopPlayer();
        UnLoadModule(ModC);
    }
}
