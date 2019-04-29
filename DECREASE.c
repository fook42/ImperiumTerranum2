#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DECREASE(uint8 ActSys, r_PlanetHeader* PPtr)
{
    APTR    ModC;
    r_PlanetHeader* MyPlanetHeader;
    char    s[40];
    struct Window* DEC_Window;
    struct RastPort* RPort_PTR;

    MyPlanetHeader = PPtr;
    INFORMUSER();
    ModC = GETTHESOUND(4);

    DEC_Window=MAKEWINDOW(80,120,351,101,MyScreen[0]);
    if (NULL != DEC_Window)
    {
        RPort_PTR = DEC_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,350,100,12,6,1);

        strcpy(s, _PT_System);
        strcat(s, ": ");
        strcat(s, Save.SystemName.data[ActSys-1]);
        WRITE(176, 7,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
        strcpy(s, _PT_Planet);
        strcat(s, ": ");
        strcat(s, MyPlanetHeader->PName);
        WRITE(176,27,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
        WRITE(176,50,12,WRITE_Center,RPort_PTR,4,PText[572]);
        WRITE(176,70,12,WRITE_Center,RPort_PTR,4,PText[573]);
        if (Save.PlayMySelf)
        {
            delay(PAUSE);
        }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(DEC_Window);
    }
    HANDLEKNOWNPLANET(ActSys,0,MyPlanetHeader);
    if (NULL != ModC)
    {
        StopPlayer();
        FreeMem(ModC,ModMemL[4]);
    }
}
