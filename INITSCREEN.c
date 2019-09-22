#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSCREEN(uint8 Mode)
{
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
        WRITE( 62,126,2,           0,MyRPort_PTR[1],3,PText[151]);
        WRITE( 62,175,2,           0,MyRPort_PTR[1],3,PText[152]);
        WRITE( 62,224,2,           0,MyRPort_PTR[1],3,PText[153]);
        WRITE(165,250,2,           0,MyRPort_PTR[1],3,PText[154]);
        WRITE( 62,273,2,           0,MyRPort_PTR[1],3,PText[155]);
        WRITE(185,312,2,           0,MyRPort_PTR[1],3,PText[156]);
        WRITE(185,359,2,           0,MyRPort_PTR[1],3,PText[157]);
        WRITE(120,461,2,WRITE_Center,MyRPort_PTR[1],3,PText[158]);
        WRITE(262,461,2,WRITE_Center,MyRPort_PTR[1],3,PText[159]);

        WRITE(333,103,2,           0,MyRPort_PTR[1],3,"%");
        WRITE(333,152,2,           0,MyRPort_PTR[1],3,"%");
        WRITE(333,201,2,           0,MyRPort_PTR[1],3,"%");
        WRITE(333,402,2,           0,MyRPort_PTR[1],3,"%");
    }
    else if (SCREEN_HISCORE == Mode)
    {
        strcpy(s, PathStr[4]);
        strcat(s, "HiScore.img");
        if (!DISPLAYIMAGE(s,0,0,640,512,7,MyScreen[1],3)) { return; }
    }
    Screen2 = Mode;
}
