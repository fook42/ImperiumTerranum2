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
    struct Window* FOR_Window;
    struct RastPort* RPort_PTR;
    FOR_Window=MAKEWINDOW(0,0,511,512,MyScreen[0]);
    if (NULL == FOR_Window)
    {
        return;
    }
    RPort_PTR = FOR_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,  0,510,330,12,6,1);
    MAKEWINBORDER(RPort_PTR,0,331,510,400,12,6,1);
    MAKEWINBORDER(RPort_PTR,0,401,510,511,12,6,1);

    WRITE(255,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[737]);
    posx = 10;
    for (i = 0; i < 2; ++i)
    {
        posy = 29;
        for (j = 0; j < 21; ++j)
        {
            haveTechColor = 29;
            offset = i*21+j+1;
            if (Save.TechCosts[ActPlayer-1].data[offset]<=0)
            {
                haveTechColor = 12;
            }
            WRITE(posx, posy, haveTechColor, 0, RPort_PTR, 3, TechnologyL.data[offset]);
            posy += 14;
        }
        posx += 245;
    }

    if (Save.ActTech[ActPlayer-1]>0)
    {
        i=strlen(TechnologyL.data[Save.ActTech[ActPlayer-1]]);
        memcpy(s, TechnologyL.data[Save.ActTech[ActPlayer-1]], i);
        s[i++]=','; s[i++]=' ';
        _s = dez2out((Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] / (AllCreative[ActPlayer-1]+1)), 0, s+i);
        *_s++=' ';
        strcpy(_s, _PT_Jahre);
        WRITE(255,342,ActPlayerFlag,WRITE_Center,RPort_PTR,4,s);

        MAKEWINBORDER(RPort_PTR,20,365,492,390,6,12,0);
        l = Save.TechCosts[ActPlayer-1].data[Save.ActTech[0]];
        if (l < 0) { l = 0; }
        RECTWIN(RPort_PTR,8,22,367,490-it_round(468.0*l/Save.TechCosts[ActPlayer-1].data[42]),388);
    } else {
        WRITE(255,342,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[173]);
    }

    /* ShipData:    MaxLoad,MaxShield,MaxMove,WeaponPower */
    WRITE( 10,410,12,0,RPort_PTR,3,PText[740]);
    WRITE(110,410,12,0,RPort_PTR,3,PText[741]);
    WRITE(210,410,12,0,RPort_PTR,3,PText[742]);
    WRITE(310,410,12,0,RPort_PTR,3,PText[743]);
    WRITE(410,410,12,0,RPort_PTR,3,PText[744]);
    l = 0;
    for (i = 24; (i>=8) && (l<4); i--)
    {
        if (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[i]]<=0)
        {
            if (l<4)
            {
                l++;
                posy=415+l*18;
                WRITE(10, posy, ActPlayerFlag, 0, RPort_PTR, 3, Project.data[i]);
                (void)dez2out(ShipData(i).MaxLoad,0,s);     WRITE(150,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).MaxMove,0,s);     WRITE(250,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).MaxShield,0,s);   WRITE(350,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).WeaponPower,0,s); WRITE(450,posy,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
            }
        }
    }
    WAITLOOP(false);
    CloseWindow(FOR_Window);
}
