#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

extern const char __ver[];

uint8 FIRSTMENU()
{
    uint8   selection = 0;
    const int   NewXBase = (HighRes_Width-def_HighRes_Width)/2;
    const int   NewYBase = (HighRes_Height-def_HighRes_Height)/2;

    SWITCHDISPLAY();
    INITMENU();
<<<<<<< HEAD
    WRITE_RP1(320,  55,40,WRITE_Center,3,"IMPERIUM TERRANUM 2");
    WRITE_RP1(320,  71,40,WRITE_Center,2,"- War of the Worlds -");
    WRITE_RP1(320,  70,40,WRITE_Center,3,"______________________");
    WRITE_RP1(320, 415,40,WRITE_Center,2,&(__ver[6]));
    WRITE_RP1(320, 435,40,WRITE_Center,2,"ported to C by Fook42 -a1k-");
    WRITE_RP1(320, 455,40,WRITE_Center,2,"Surround-sounds, created with the WaveTracer DS�");
    WRITE_RP1(320, 128,40,WRITE_Center,2,"Frei kopierbare Version");
    WRITE_RP1(320, 146,40,WRITE_Center,2,"FREEWARE and OpenSource");
    WRITE_RP1(320, 164,40,WRITE_Center,2,"Created by VirtualWorlds Productions:http://www.VirtualWorlds.de");
    MAKEWINBORDER(MyRPort_PTR[1],150,200,490,230,40,14,0);
    MAKEWINBORDER(MyRPort_PTR[1],150,240,490,270,40,14,0);
    WRITE_RP1(320, 207,21,WRITE_Center,3,PText[664]);
    WRITE_RP1(320, 247,21,WRITE_Center,3,PText[665]);
=======
    WRITE_RP1(CenterX, NewYBase+ 55,40,WRITE_Center,3,"IMPERIUM TERRANUM 2");
    WRITE_RP1(CenterX, NewYBase+ 71,40,WRITE_Center,2,"- War of the Worlds -");
    WRITE_RP1(CenterX, NewYBase+ 70,40,WRITE_Center,3,"______________________");
    WRITE_RP1(CenterX, NewYBase+415,40,WRITE_Center,2,"Version 2.8c");
    WRITE_RP1(CenterX, NewYBase+435,40,WRITE_Center,2,"ported to C by Fook42 -a1k-");
    WRITE_RP1(CenterX, NewYBase+455,40,WRITE_Center,2,"Surround-sounds, created with the WaveTracer DS�");
    WRITE_RP1(CenterX, NewYBase+128,40,WRITE_Center,2,"Frei kopierbare Version");
    WRITE_RP1(CenterX, NewYBase+146,40,WRITE_Center,2,"FREEWARE and OpenSource");
    WRITE_RP1(CenterX, NewYBase+164,40,WRITE_Center,2,"Created by VirtualWorlds Productions:http://www.VirtualWorlds.de");
    MAKEWINBORDER(MyRPort_PTR[1],CenterX-170,NewYBase+200,CenterX+170,NewYBase+230,40,14,0);
    MAKEWINBORDER(MyRPort_PTR[1],CenterX-170,NewYBase+240,CenterX+170,NewYBase+270,40,14,0);
    WRITE_RP1(CenterX, NewYBase+207,21,WRITE_Center,3,PText[664]);
    WRITE_RP1(CenterX, NewYBase+247,21,WRITE_Center,3,PText[665]);
>>>>>>> 21aadae (resize Play-area with screensize)
    ScreenToFront(MyScreen[1]);

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
<<<<<<< HEAD
            if ((WinMouseX(1)>149) && (WinMouseX(1)<491))
            {
                if ((WinMouseY(1)>199) && (WinMouseY(1)<231))
                {
                    selection = 1;
                    CLICKRECT(MyRPort_PTR[1],150,200,490,230,40);
                } else if ((WinMouseY(1)>239) && (WinMouseY(1)<271))
=======
            if ((MouseX(1)>(CenterX-171)) && (MouseX(1)<(CenterX+171)))
            {
                if ((MouseY(1)>(NewYBase+199)) && (MouseY(1)<(NewYBase+231)))
                {
                    selection = 1;
                    CLICKRECT(MyRPort_PTR[1],CenterX-170,NewYBase+200,CenterX+170,NewYBase+230,40);
                } else if ((MouseY(1)>(NewYBase+239)) && (MouseY(1)<(NewYBase+271)))
>>>>>>> 21aadae (resize Play-area with screensize)
                {
                    selection = 2;
                    CLICKRECT(MyRPort_PTR[1],CenterX-170,NewYBase+240,CenterX+170,NewYBase+270,40);
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
