#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FINANZEN_WRITEDATA(struct RastPort* RPort, uint8 Mode, uint8 FWD_ActPlayer)
{
    char    s[5];

    (void) dez2out(Save.GSteuer[FWD_ActPlayer], 3, s);
    s[3]='%';
    s[4]=0;
    WRITEWIN(307, 47,8,(1|WRITE_Right),RPort,2,s);
    (void) dez2out((100-Save.JSteuer[FWD_ActPlayer]-Save.GSteuer[FWD_ActPlayer]-Save.SService[FWD_ActPlayer]), 3, s);
    s[3]='%';
    s[4]=0;
    WRITEWIN(307, 92,8,(1|WRITE_Right),RPort,2,s);
    (void) dez2out(Save.SService[FWD_ActPlayer], 3, s);
    s[3]='%';
    s[4]=0;
    WRITEWIN(307,137,8,(1|WRITE_Right),RPort,2,s);
    if ((WFLAG_JAHADR == Save.WorldFlag)
        && ((Save.JSteuer[FWD_ActPlayer]+Mode)>0)
        && (LEVEL_UNKNOWN != Save.WarState[FWD_ActPlayer][7])
        && (LEVEL_DIED    != Save.WarState[FWD_ActPlayer][7]))
    {
        (void) dez2out(Save.JSteuer[FWD_ActPlayer], 3, s);
        s[3]='%';
        s[4]=0;
        WRITEWIN(307,227,8,(1|WRITE_Right),RPort,2,s);
    }
}

/* -------------------------------------------------------- */

void FINANZEN()
{
    uint8   i;
	struct Window* FIN_Window;
	struct RastPort* RPort_PTR;

	FIN_Window=MAKEWINDOW(60,100,371,176,MyScreen[0]);
	if (NULL == FIN_Window)
	{
		return;
	}
	RPort_PTR = FIN_Window->RPort;
	MAKEWINBORDER(RPort_PTR,0,0,370,175,12,6,1);

	ActPlayer--;    // TODO ... due to shifted Save-array...

    WRITEWIN(196, 10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[678]);
    WRITEWIN( 20, 47,           12,           0,RPort_PTR,4,PText[679]);
    WRITEWIN( 20, 92,ActPlayerFlag,           0,RPort_PTR,4,PText[680]);
    WRITEWIN( 20,137,           12,           0,RPort_PTR,4,PText[681]);
    for (i = 0; i<3; i++)
        { MAKEWINBORDER(RPort_PTR,231,i*45+42,310,i*45+65,12,6,1); }

	WRITEWIN(250, 70,12,0,RPort_PTR,2,"I J");
    WRITEWIN(250,115,12,0,RPort_PTR,2,"J I");
    if ((WFLAG_JAHADR == Save.WorldFlag)
        && (Save.JSteuer[ActPlayer]>0)
        && (LEVEL_UNKNOWN != Save.WarState[ActPlayer][7])
        && (LEVEL_DIED    != Save.WarState[ActPlayer][7]))
    {
        MAKEWINBORDER(RPort_PTR,0,100,370,265,12,6,0);
        WRITEWIN(265,205,8,0,RPort_PTR,2,"I");
        WRITEWIN( 20,216,FLAG_OTHER,0,RPort_PTR,4,PText[683]);
        MAKEWINBORDER(RPort_PTR,231,222,310,245,12,6,1);
        WRITEWIN( 20,233,FLAG_OTHER,0,RPort_PTR,4,PText[684]);
    }
    FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
    do
    {
        delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(1,300);
            if ((FIN_Window->MouseY>=65) && (FIN_Window->MouseY<=88))
            {
                if ((FIN_Window->MouseX>=240) && (FIN_Window->MouseX<=270))
                {
                    if (Save.GSteuer[ActPlayer]<(100-Save.JSteuer[ActPlayer]-Save.SService[ActPlayer]))
                    {
                        Save.GSteuer[ActPlayer]++;
                    }
                } else if ((FIN_Window->MouseX>=275) && (FIN_Window->MouseX<=305))
                {
                    if (Save.GSteuer[ActPlayer]>0)
                    {
                        Save.GSteuer[ActPlayer]--;
                    }
                }
                FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
            }
            if ((FIN_Window->MouseY>=110) && (FIN_Window->MouseY<=133))
            {
                if ((FIN_Window->MouseX>=240) && (FIN_Window->MouseX<=270))
                {
                    if (Save.SService[ActPlayer]<(100-Save.JSteuer[ActPlayer]-Save.GSteuer[ActPlayer]))
                    {
                        Save.SService[ActPlayer]++;
                    }
                } else if ((FIN_Window->MouseX>=275) && (FIN_Window->MouseX<=305))
                {
                    if (Save.SService[ActPlayer]>0)
                    {
                        Save.SService[ActPlayer]--;
                    }
                }
                FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
            }
            if ((FIN_Window->MouseX>=260) && (FIN_Window->MouseX<=280)
              &&(FIN_Window->MouseY>=200) && (FIN_Window->MouseY<=220)
              &&(Save.WorldFlag == WFLAG_JAHADR) && (Save.JSteuer[ActPlayer]>0))
            {
                while (Save.JSteuer[ActPlayer]>0)
                {
                   Save.JSteuer[ActPlayer]--;
                   FINANZEN_WRITEDATA(RPort_PTR, 1, ActPlayer);
                   PLAYSOUND(1,300);
                   delay(10);
                }
                ActPlayer++;    // TODO ... due to shifted Save-array...
				CloseWindow(FIN_Window);
                GOTOWAR(ActPlayerFlag, FLAG_OTHER);
                return;
            }
        }
    }
    while (RMB_NOTPRESSED);
    ActPlayer++;    // TODO ... due to shifted Save-array...

	CloseWindow(FIN_Window);
}
