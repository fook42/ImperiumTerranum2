#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FORSCHUNG()
{
    sint32  l;
    int     i, j;
    uint16  offset, pos;
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

    WRITEWIN(255,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[737]);
    for (i = 0; i<=1; i++)
    {
        pos=i*245+10;
        for (j = 1; j<=21; j++)
        {
            haveTechColor = 29;
            offset = i*21+j;
            if (Save.TechCosts[ActPlayer-1].data[offset]<=0)
            {
                haveTechColor = 12;
            }
            WRITEWIN(pos, j*14+15, haveTechColor, 0,RPort_PTR, 3, TechnologyL.data[offset]);
        }
    }

    if (Save.ActTech[ActPlayer-1]>0)
    {
        i=strlen(TechnologyL.data[Save.ActTech[ActPlayer-1]]);
        memcpy(s, TechnologyL.data[Save.ActTech[ActPlayer-1]], i);
        s[i++]=','; s[i++]=' ';
        _s = dez2out((Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] / (AllCreative[ActPlayer-1]+1)), 0, s+i);
        *_s++=' ';
        strcpy(_s, _PT_Jahre);
        WRITEWIN(255,342,ActPlayerFlag,WRITE_Center,RPort_PTR,4,s);
 
        MAKEWINBORDER(RPort_PTR,20,365,492,390,6,12,0);
        l = Save.TechCosts[ActPlayer-1].data[Save.ActTech[0]];
        if (l < 0) { l = 0; }
        RECTWIN(RPort_PTR,8,22,367,490-it_round(468.0*l/Save.TechCosts[ActPlayer-1].data[42]),388);
    } else {
        WRITEWIN(255,342,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[173]);
    }

    /* ShipData:    MaxLoad,MaxShield,MaxMove,WeaponPower */
    WRITEWIN( 10,410,12,0,RPort_PTR,3,PText[740]);
    WRITEWIN(110,410,12,0,RPort_PTR,3,PText[741]);
    WRITEWIN(210,410,12,0,RPort_PTR,3,PText[742]);
    WRITEWIN(310,410,12,0,RPort_PTR,3,PText[743]);
    WRITEWIN(410,410,12,0,RPort_PTR,3,PText[744]);
    l = 0;
    for (i = 24; (i>=8) && (l<4); i--)
    {
        if (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[i]]<=0)
        {
            if (l<4)
            {
                l++;
                pos=415+l*18;
                WRITEWIN(10,pos,ActPlayerFlag,0,RPort_PTR,3,Project.data[i]);
                (void)dez2out(ShipData(i).MaxLoad,0,s);     WRITEWIN(150,pos,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).MaxMove,0,s);     WRITEWIN(250,pos,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).MaxShield,0,s);   WRITEWIN(350,pos,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
                (void)dez2out(ShipData(i).WeaponPower,0,s); WRITEWIN(450,pos,ActPlayerFlag,WRITE_Right,RPort_PTR,3,s);
            }
        }
    }
    WAITLOOP(false);
	CloseWindow(FOR_Window);
}
