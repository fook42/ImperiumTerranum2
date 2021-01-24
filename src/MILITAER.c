#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define _BIG_CROSS_ "|"

void DRAWDATA(struct RastPort* RPort, uint8 BSet)
{
    uint8   i, btx;
    uint16  ypos;
    uint8   XState, Fight, Costs;
    char    s[20];
    char*   _s;

    SetAPen(RPort,0);
    if (((0 < BSet) && (9 > BSet))
     || ((16 == BSet) && (Save.TechCosts[ActPlayer-1].data[9]  <= 0))
     || ((32 == BSet) && (Save.TechCosts[ActPlayer-1].data[23] <= 0)))
    {
        Save.Military[ActPlayer-1] ^= BSet;
    }

    btx = 1;
    XState = 0;
    Fight = 0;
    Costs = Save.Military[ActPlayer-1];
    ypos = 55;
    for (i = 0; i < 6; ++i)
    {
        if ((Save.Military[ActPlayer-1] & btx) != 0)
        {
            XState += i+1;
            Fight  += 8;
            WRITE(24, ypos, 12, 1, RPort,3, _BIG_CROSS_);
        } else {
            RectFill(RPort, 24, ypos, 40, ypos+15);
        }
        btx = btx<<1;
        ypos +=30;
    }
    s[0]=' ';
    s[1]='-';
    (void) dez2out(XState, 0, s+2);
    WRITE(230,240,8,(1|WRITE_Right),RPort,1,s);
    s[1]='+';
    (void) dez2out(Fight, 0, s+2);
    WRITE(230,265,8,(1|WRITE_Right),RPort,1,s);
    _s = dez2out(Costs, 0, s+2);
    *_s++ = '%'; *_s = 0;
    WRITE(244,290,8,(1|WRITE_Right),RPort,1,s);
}

/* -------------------------------------------------------- */

void MILITAER()
{
    char    s[40];
    const char Txt_notavail[] = {"--- (benötigt \0"};
    char*   _s;
    int     i;
    uint16  ypos;

    struct Window* MIL_Window;
    struct RastPort* RPort_PTR;
    MIL_Window=MAKEWINDOW(50,70,411,331,MyScreen[0]);
    if (NULL == MIL_Window)
    {
        return;
    }
    RPort_PTR = MIL_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,410,330,12,6,1);

    ypos = 50;
    for (i = 0; i < 6; ++i)
    {
        MAKEWINBORDER( RPort_PTR, 20, ypos, 45, ypos+25, 12, 6, 1);
        ypos += 30;
    }

    WRITE(205,10,ActPlayerFlag,WRITE_Center, RPort_PTR,3,PText[667]);
    WRITE(60, 55,12,0, RPort_PTR,3,PText[668]);
    WRITE(60, 85,12,0, RPort_PTR,3,PText[669]);
    WRITE(60,115,12,0, RPort_PTR,3,PText[670]);
    WRITE(60,145,12,0, RPort_PTR,3,PText[671]);

    if (Save.TechCosts[ActPlayer-1].data[9]  <= 0)
    {
        _s = PText[672];
    } else {
        _s=my_strcpy(s, Txt_notavail);
        _s=my_strcpy(_s, TechnologyL.data[9]);
        *_s++ = ')';
        *_s = 0;
        _s = s;
    }
    WRITE(60,175,12,0, RPort_PTR,3,_s);

    if (Save.TechCosts[ActPlayer-1].data[23] <= 0)
    {
        _s = PText[673];
    } else {
        _s=my_strcpy(s, Txt_notavail);
        _s=my_strcpy(_s, TechnologyL.data[9]);
        *_s++ = ')';
        *_s = 0;
        _s = s;
    }
    WRITE(60,205,12,0, RPort_PTR,3,_s);
    WRITE(20,240,ActPlayerFlag,0, RPort_PTR,3,PText[674]);
    WRITE(20,265,ActPlayerFlag,0, RPort_PTR,3,PText[675]);
    WRITE(20,290,ActPlayerFlag,0, RPort_PTR,3,PText[676]);
    DRAWDATA(RPort_PTR, 0);
    do
    {
//        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(0,300);
            if ((MIL_Window->MouseX>=20) && (MIL_Window->MouseX<=390))
            {
                if      ((MIL_Window->MouseY>= 50) && (MIL_Window->MouseY<= 75)) { DRAWDATA(RPort_PTR, 1); }
                else if ((MIL_Window->MouseY>= 80) && (MIL_Window->MouseY<=105)) { DRAWDATA(RPort_PTR, 2); }
                else if ((MIL_Window->MouseY>=110) && (MIL_Window->MouseY<=135)) { DRAWDATA(RPort_PTR, 4); }
                else if ((MIL_Window->MouseY>=140) && (MIL_Window->MouseY<=165)) { DRAWDATA(RPort_PTR, 8); }
                else if ((MIL_Window->MouseY>=170) && (MIL_Window->MouseY<=195)) { DRAWDATA(RPort_PTR, 16); }
                else if ((MIL_Window->MouseY>=200) && (MIL_Window->MouseY<=225)) { DRAWDATA(RPort_PTR, 32); }
            }
            while(LMB_PRESSED) {}
        }
    }
    while (RMB_NOTPRESSED);

    CloseWindow(MIL_Window);
}

