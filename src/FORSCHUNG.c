#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FORSCHUNG()
{
    sint32  l;
    int     i, j;
    uint16  offset;
    uint16  posx, posy;
    uint8   haveTechColor;
    char    s[50];
    char*   _s;
    const char* FOR_ShipInfos[] = {PText[740], PText[741], PText[742], PText[743], PText[744]};

    struct Window* FOR_Window = NULL;
    struct RastPort* RPort_PTR;
    FOR_Window=MAKEWINDOW(0,0,Area_Width,Area_Height,MyScreen[0]);
    if (NULL != FOR_Window)
    {
        RPort_PTR = FOR_Window->RPort;
        i = 1;
    } else {
        // fallback if we ran out of chip/graphic memory
        RPort_PTR = MyRPort_PTR[0];
        i = 0;
    }
    MAKEWINBORDER(RPort_PTR,0,  0,Area_Width-1,330,12,6,i);
    MAKEWINBORDER(RPort_PTR,0,331,Area_Width-1,400,12,6,i);
    MAKEWINBORDER(RPort_PTR,0,401,Area_Width-1,Area_Height-1,12,6,i);

    WRITE(Area_CenterX,10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[737]);
    posx = 10;
    for (i = 0; i < 2; ++i)
    {
        posy = 29;
        for (j = 0; j < 21; ++j)
        {
            haveTechColor = 29;
            offset = i*21+j+1;
            if (Save.TechCosts[ActPlayer-1][offset]<=0)
            {
                haveTechColor = 12;
            }
            WRITE(posx, posy, haveTechColor, 0, RPort_PTR,2, TechnologyName[offset]);
            posy += 14;
        }
        posx += 245;
    }

    if (0 < Save.ActTech[ActPlayer-1])
    {
        _s=my_strcpy(s, TechnologyName[Save.ActTech[ActPlayer-1]]);
        *_s++ = ',';
        *_s++ = ' ';
        _s = dez2out((Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] / (AllCreative[ActPlayer-1]+1)), 0, _s);
        *_s++ = ' ';
        (void) my_strcpy(_s, _PT_Jahre);
        WRITE(Area_CenterX,342,ActPlayerFlag,WRITE_Center,RPort_PTR,3,s);

        MAKEWINBORDER(RPort_PTR,20,365,(Area_Width-20),390,6,12,0);
        l = Save.TechCosts[ActPlayer-1][Save.ActTech[0]];
        if (l < 0) { l = 0; }
        RECTWIN(RPort_PTR,8,22,367,(Area_Width-22)-it_round((Area_Width-44)*((double) l/Save.TechCosts[ActPlayer-1][42])),388);
    } else {
        WRITE(Area_CenterX,342,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[173]);
    }

    /* ShipData:    MaxLoad,MaxShield,MaxMove,WeaponPower */
    posx = 10;
    for (i = 0; i < (sizeof(FOR_ShipInfos)/sizeof(FOR_ShipInfos[0])); ++i)
    {
        WRITE(posx,410,12,0,RPort_PTR,2,FOR_ShipInfos[i]);
        posx += 100;
    }
    l = 0;
    for (i = 24; (i>=8) && (l<4); i--)
    {
        if (0 >= Save.TechCosts[ActPlayer-1][ProjectNeedsTech[i]])
        {
            if (4 > l)
            {
                ++l;
                posy=415+l*18;
                WRITE(10, posy, ActPlayerFlag, 0, RPort_PTR,2, ProjectName[i]);
                (void) dez2out(ShipData(i).MaxLoad,0,s);     WRITE(150,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,2,s);
                (void) dez2out(ShipData(i).MaxMove,0,s);     WRITE(250,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,2,s);
                (void) dez2out(ShipData(i).MaxShield,0,s);   WRITE(350,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,2,s);
                (void) dez2out(ShipData(i).WeaponPower,0,s); WRITE(450,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,2,s);
            }
        }
    }
    WAITLOOP(false);
    if (NULL != FOR_Window)
    {
        CloseWindow(FOR_Window);
    } else {
        DRAWSTARS(MODE_REDRAW);
    }
}
