#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSCREEN(uint8 Mode)
{
    uint8   i;
    uint16  y;
    char    s[40];

    if (SCREEN_TECH == Mode)
    {
        strcpy(s, PathStr[5]);
        strcat(s, "ShipTech.img");
        if (!DISPLAYIMAGE(s,0,0,640,512,3,MyScreen[1],1)) { return; }
    }
    else if (Screen2 == Mode)
    {
        return;
    }
    else if (SCREEN_PLANET == Mode)
    {
        RECTWIN(MyRPort_PTR[1],0,0,0,639,511);
        strcpy(s, PathStr[0]);
        strcat(s, "PlanetDesk.img");
        if (!DISPLAYIMAGE(s,0,94,384,394,8,MyScreen[1],2)) { return; }
        WRITEWIN( 62,126,2,           0,MyRPort_PTR[1],4,PText[151]);
        WRITEWIN( 62,175,2,           0,MyRPort_PTR[1],4,PText[152]);
        WRITEWIN( 62,224,2,           0,MyRPort_PTR[1],4,PText[153]);
        WRITEWIN(165,250,2,           0,MyRPort_PTR[1],4,PText[154]);
        WRITEWIN( 62,273,2,           0,MyRPort_PTR[1],4,PText[155]);
        WRITEWIN(185,312,2,           0,MyRPort_PTR[1],4,PText[156]);
        WRITEWIN(185,359,2,           0,MyRPort_PTR[1],4,PText[157]);
        WRITEWIN(120,461,2,WRITE_Center,MyRPort_PTR[1],4,PText[158]);
        WRITEWIN(262,461,2,WRITE_Center,MyRPort_PTR[1],4,PText[159]);
        y = 103;
        for (i = 0; i < 3; ++i)
        {
            WRITEWIN(333, y,2,0,MyRPort_PTR[1],4,"%");
            y += 49;
        }
        WRITEWIN(333,402,2,0,MyRPort_PTR[1],4,"%");
    }
    else if (SCREEN_HISCORE == Mode)
    {
        strcpy(s, PathStr[4]);
        strcat(s, "HiScore.img");
        if (!DISPLAYIMAGE(s,0,0,640,512,7,MyScreen[1],3)) { return; }
    }
    Screen2 = Mode;
}
