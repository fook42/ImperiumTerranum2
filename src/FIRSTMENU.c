#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 FIRSTMENU()
{
    uint8   selection = 0;

    SWITCHDISPLAY();
    INITMENU();
    WRITE_RP1(320, 55,40,WRITE_Center,3,"IMPERIUM TERRANUM 2");
    WRITE_RP1(320, 71,40,WRITE_Center,2,"- War of the Worlds -");
    WRITE_RP1(320, 70,40,WRITE_Center,3,"______________________");
    WRITE_RP1(320,415,40,WRITE_Center,2,"Version 2.8c");
    WRITE_RP1(320,435,40,WRITE_Center,2,"ported to C by F00K42 -a1k-");
    WRITE_RP1(320,455,40,WRITE_Center,2,"Surround-sounds, created with the WaveTracer DS�");
    WRITE_RP1(320,128,40,WRITE_Center,2,"Frei kopierbare Version");
    WRITE_RP1(320,146,40,WRITE_Center,2,"FREEWARE and OpenSource");
    WRITE_RP1(320,164,40,WRITE_Center,2,"Created by VirtualWorlds Productions:http://www.VirtualWorlds.de");
    MAKEWINBORDER(MyRPort_PTR[1],150,200,490,230,40,14,0);
    MAKEWINBORDER(MyRPort_PTR[1],150,240,490,270,40,14,0);
    WRITE_RP1(320,207,21,WRITE_Center,3,PText[664]);
    WRITE_RP1(320,247,21,WRITE_Center,3,PText[665]);
    ScreenToFront(MyScreen[1]);

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((MouseX(1)>=150) && (MouseX(1)<=490))
            {
                if ((MouseY(1)>=200) && (MouseY(1)<=230))
                {
                    selection = 1;
                    CLICKRECT(MyRPort_PTR[1],150,200,490,230,40);
                } else if ((MouseY(1)>=240) && (MouseY(1)<=270))
                {
                    selection = 2;
                    CLICKRECT(MyRPort_PTR[1],150,240,490,270,40);
                }
            }
        }
    }
    while (0 == selection);

    ScreenToFront(XScreen);
    if (1 == selection)
    {
        OPTIONMENU(1);
        if (1 < Player)
        {
            OPTION_MULTIPLAYER();
        }
    } else
    {
        StopPlayer();
    }

    if (Save.PlayMySelf)
    {
        Save.ActTech[0] = 3;
    }
    return (selection);
}
