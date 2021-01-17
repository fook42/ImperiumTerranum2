#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSCREEN(uint8 Mode)
{
    char    s[40];

    switch(Mode) {
        case SCREEN_PLANET:
                            SetRast(MyRPort_PTR[1], 0);     // fill a complete RastPort with color..
                            // = RECT_RP1(0,0,0,639,511);
                            strcpy(s, PathStr[0]);
                            strcat(s, "PlanetDesk.img");
                            if (!DISPLAYIMAGE(s,0,94,384,394,8,MyScreen[1],2)) { return; }
                            WRITE_RP1( 62,126,2,           0,3,_PT_Biosphaere);
                            WRITE_RP1( 62,175,2,           0,3,_PT_Infrastruktur);
                            WRITE_RP1( 62,224,2,           0,3,_PT_Industriealisierung);
                            WRITE_RP1(165,250,2,           0,3,_PT_Millionen);
                            WRITE_RP1( 62,273,2,           0,3,_PT_Population);
                            WRITE_RP1(185,312,2,           0,3,_PT_Kreativitaet);
                            WRITE_RP1(185,359,2,           0,3,_PT_Produktivitaet);
                            WRITE_RP1(120,461,2,WRITE_Center,3,_PT_Aendern);
                            WRITE_RP1(262,461,2,WRITE_Center,3,_PT_Kaufen);

                            WRITE_RP1(333,103,2,           0,3,"%");
                            WRITE_RP1(333,152,2,           0,3,"%");
                            WRITE_RP1(333,201,2,           0,3,"%");
                            WRITE_RP1(333,402,2,           0,3,"%");
                            break;
        case SCREEN_HISCORE:
                            strcpy(s, PathStr[4]);
                            strcat(s, "HiScore.img");
                            if (!DISPLAYIMAGE(s,0,0,640,512,7,MyScreen[1],3)) { return; }
                            break;
        case SCREEN_TECH:
                            strcpy(s, PathStr[5]);
                            strcat(s, "ShipTech.img");
                            if (!DISPLAYIMAGE(s,0,0,640,512,3,MyScreen[1],1)) { return; }
                            break;
        default:            {}
    }
    Screen2 = Mode;
}
