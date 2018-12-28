#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWDATA(struct RastPort* RPort, uint8 BSet)
{
    uint8   i, btx;
    uint8   XState, Fight, Costs;
    char    s[20];
    char*   _s;

    SetAPen(RPort,0);
    if (((BSet>=1) && (BSet<=8))
     || ((BSet == 16) && (Save.TechCosts[ActPlayer-1].data[9]  <= 0))
     || ((BSet == 32) && (Save.TechCosts[ActPlayer-1].data[23] <= 0)))
    {
        Save.Military[ActPlayer-1] ^= BSet;
    }

    btx = 1;
    XState = 0;
    Fight = 0;
    Costs = Save.Military[ActPlayer-1];
    for (i = 1; i<=6; i++)
    {
        if ((Save.Military[ActPlayer-1] & btx) == btx)
        {
            XState += i;
            Fight  += 8;
            WRITEWIN(24,i*30+25,12,1,RPort,4,"|");
        } else {
            RectFill(RPort, 24, i*30+25, 40, i*30+40);
        }
        btx = btx<<1;
    }
    s[0]=' ';
    s[1]='-';
    (void)dez2out(XState, 0, s+2);
    WRITEWIN(230,240,8,(1|WRITE_Right),RPort,2,s);
    s[1]='+';
    (void)dez2out(Fight, 0, s+2);
    WRITEWIN(230,265,8,(1|WRITE_Right),RPort,2,s);
    _s = dez2out(Costs, 0, s+2);
    *_s++='%'; *_s=0;
    WRITEWIN(244,290,8,(1|WRITE_Right),RPort,2,s);
}

/* -------------------------------------------------------- */

void MILITAER()
{
    char    s[40];
    char    Txt_notavail[]= "--- (ben�tigt ";
    const int _Txt_notavail_len = (sizeof(Txt_notavail)/sizeof(Txt_notavail[0]))-1;
    char*   _s;
    int   i;

	struct Window* MIL_Window;
	struct RastPort* RPort_PTR;
	MIL_Window=MAKEWINDOW(50,70,411,331,MyScreen[0]);
	if (NULL == MIL_Window)
	{
		return;
	}
	RPort_PTR = MIL_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,410,330,12,6,1);


    for (i = 0; i<6; i++)
        { MAKEWINBORDER( RPort_PTR,20,i*30+50,45,i*30+75,12,6,1); }

    WRITEWIN(205,10,ActPlayerFlag,WRITE_Center, RPort_PTR,4,PText[667]);
    WRITEWIN(60, 55,12,0, RPort_PTR,4,PText[668]);
    WRITEWIN(60, 85,12,0, RPort_PTR,4,PText[669]);
    WRITEWIN(60,115,12,0, RPort_PTR,4,PText[670]);
    WRITEWIN(60,145,12,0, RPort_PTR,4,PText[671]);

    if (Save.TechCosts[ActPlayer-1].data[9]  <= 0)
    {
        _s = PText[672];
    } else {
        memcpy(s, Txt_notavail, _Txt_notavail_len);
        i = strlen(TechnologyL.data[9]);
        memcpy(s+_Txt_notavail_len, TechnologyL.data[9], i);
        s[_Txt_notavail_len+(i++)]=')';
        s[_Txt_notavail_len+i]=0;
        _s = s;
    }
    WRITEWIN(60,175,12,0, RPort_PTR,4,_s);

    if (Save.TechCosts[ActPlayer-1].data[23] <= 0)
    {
        _s = PText[673];
    } else {
        memcpy(s, Txt_notavail, _Txt_notavail_len);
        i = strlen(TechnologyL.data[23]);
        memcpy(s+_Txt_notavail_len, TechnologyL.data[23], i);
        s[_Txt_notavail_len+(i++)]=')';
        s[_Txt_notavail_len+i]=0;
        _s = s;
    }
    WRITEWIN(60,205,12,0, RPort_PTR,4,_s);
    WRITEWIN(20,240,ActPlayerFlag,0, RPort_PTR,4,PText[674]);
    WRITEWIN(20,265,ActPlayerFlag,0, RPort_PTR,4,PText[675]);
    WRITEWIN(20,290,ActPlayerFlag,0, RPort_PTR,4,PText[676]);
    DRAWDATA(RPort_PTR, 0);
    do
    {
//        delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(1,300);
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
