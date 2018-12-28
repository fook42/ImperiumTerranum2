#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 FIRSTMENU()
{
    uint8   selection = 0;

    SWITCHDISPLAY();
    INITMENU();
    WRITEWIN(320, 55,40,WRITE_Center,MyRPort_PTR[1],4,"IMPERIUM TERRANUM 2");
    WRITEWIN(320, 71,40,WRITE_Center,MyRPort_PTR[1],3,"- War of the Worlds -");
    WRITEWIN(320, 70,40,WRITE_Center,MyRPort_PTR[1],4,"______________________");
    WRITEWIN(320,415,40,WRITE_Center,MyRPort_PTR[1],3,"Version 2.8c");
    WRITEWIN(320,435,40,WRITE_Center,MyRPort_PTR[1],3,"ported to C by F00K42 -a1k-");
    WRITEWIN(320,455,40,WRITE_Center,MyRPort_PTR[1],3,"Surround-sounds, created with the WaveTracer DS®");
    WRITEWIN(320,128,40,WRITE_Center,MyRPort_PTR[1],3,"Frei kopierbare Version");
    WRITEWIN(320,146,40,WRITE_Center,MyRPort_PTR[1],3,"FREEWARE and OpenSource");
    WRITEWIN(320,164,40,WRITE_Center,MyRPort_PTR[1],3,"Created by VirtualWorlds Productions:http://www.VirtualWorlds.de");
    MAKEWINBORDER(MyRPort_PTR[1],150,200,490,230,40,14,0);
    MAKEWINBORDER(MyRPort_PTR[1],150,240,490,270,40,14,0);
    WRITEWIN(320,207,21,WRITE_Center,MyRPort_PTR[1],4,PText[664]);
    WRITEWIN(320,247,21,WRITE_Center,MyRPort_PTR[1],4,PText[665]);
    ScreenToFront(MyScreen[1]);

    do
    {
        delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((MouseX(1)>=150) && (MouseX(1)<=490))
            {
                if ((MouseY(1)>=200) && (MouseY(1)<=230))
                {
                    selection = 1;
                    CLICKRECT(MyScreen[1],150,200,490,230,40);
                } else if ((MouseY(1)>=240) && (MouseY(1)<=270))
                {
                    selection = 2;
                    CLICKRECT(MyScreen[1],150,240,490,270,40);
                }
            }
        }
    }
    while (0 == selection);

    ScreenToFront(XScreen);
    if (1 == selection)
    {
        OPTIONMENU(1);
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
