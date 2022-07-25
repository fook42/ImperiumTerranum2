#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FINANZEN_WRITEDATA(struct RastPort* RPort, const int Mode, const int FWD_ActPlayer)
{
    char    s[5];

    (void) dez2out(Save.GSteuer[FWD_ActPlayer], 3, s);
    s[3]='%';
    s[4]=0;
    WRITE(307, 47,8,(JAM2|WRITE_Right),RPort,1,s);
    (void) dez2out((100-Save.JSteuer[FWD_ActPlayer]-Save.GSteuer[FWD_ActPlayer]-Save.SService[FWD_ActPlayer]), 3, s);
    s[3]='%';
    s[4]=0;
    WRITE(307, 92,8,(JAM2|WRITE_Right),RPort,1,s);
    (void) dez2out(Save.SService[FWD_ActPlayer], 3, s);
    s[3]='%';
    s[4]=0;
    WRITE(307,137,8,(JAM2|WRITE_Right),RPort,1,s);
    if ((WFLAG_JAHADR == Save.WorldFlag)
        && (0 < (Save.JSteuer[FWD_ActPlayer]+Mode))
        && (LEVEL_UNKNOWN != Save.WarState[FWD_ActPlayer][7])
        && (LEVEL_DIED    != Save.WarState[FWD_ActPlayer][7]))
    {
        (void) dez2out(Save.JSteuer[FWD_ActPlayer], 3, s);
        s[3]='%';
        s[4]=0;
        WRITE(307,203,8,(JAM2|WRITE_Right),RPort,1,s);
    }
}

/* -------------------------------------------------------- */

void FINANZEN()
{
    uint8   i;
    uint16  ypos;
    struct Window* FIN_Window;
    struct RastPort* RPort_PTR;
    int     jahadr_winheight = 0;

    if ((WFLAG_JAHADR == Save.WorldFlag)
        && (0 < Save.JSteuer[ActPlayer])
        && (LEVEL_UNKNOWN != Save.WarState[ActPlayer][7])
        && (LEVEL_DIED    != Save.WarState[ActPlayer][7]))
    {
        jahadr_winheight = 66;
    }

    FIN_Window=MAKECENTERWINDOW(371,176+jahadr_winheight,MyScreen[0]);
    if (NULL == FIN_Window)
    {
        return;
    }
    RPort_PTR = FIN_Window->RPort;


    ActPlayer--;    // TODO ... due to shifted Save-array...

    WRITE(196, 10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[678]);
    WRITE( 20, 47,           12,        JAM1,RPort_PTR,3,PText[679]);
    WRITE( 20, 92,ActPlayerFlag,        JAM1,RPort_PTR,3,PText[680]);
    WRITE( 20,137,           12,        JAM1,RPort_PTR,3,PText[681]);
    ypos = 42;
    for (i = 0; i<3; i++)
    {
        MAKEWINBORDER(RPort_PTR,231, ypos,310, ypos+23,12,6,1);
        ypos += 45;
    }

    WRITE(250, 70,12,0,RPort_PTR,1,"I J");      // ^ v
    WRITE(250,115,12,0,RPort_PTR,1,"J I");      // v ^
    if (0 != jahadr_winheight)
    {

        WRITE(265,181,8,0,RPort_PTR,1,"I");     // ^
        WRITE( 20,192,FLAG_OTHER,0,RPort_PTR,3,PText[683]);
        MAKEWINBORDER(RPort_PTR,231,198,310,221,12,6,1);
        WRITE( 20,209,FLAG_OTHER,0,RPort_PTR,3,PText[684]);
    }
    FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(0,300);
            if ((64 < FIN_Window->MouseY) && (89 > FIN_Window->MouseY))
            {
                if ((239 < FIN_Window->MouseX) && (271 > FIN_Window->MouseX))
                {
                    if (Save.GSteuer[ActPlayer]<(100-Save.JSteuer[ActPlayer]-Save.SService[ActPlayer]))
                    {
                        Save.GSteuer[ActPlayer]++;
                    }
                } else if ((274 < FIN_Window->MouseX) && (306 > FIN_Window->MouseX))
                {
                    if (0 < Save.GSteuer[ActPlayer])
                    {
                        Save.GSteuer[ActPlayer]--;
                    }
                }
                FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
            } else if ((109 < FIN_Window->MouseY) && (134 > FIN_Window->MouseY))
            {
                if ((239 < FIN_Window->MouseX) && (271 > FIN_Window->MouseX))
                {
                    if (Save.SService[ActPlayer]<(100-Save.JSteuer[ActPlayer]-Save.GSteuer[ActPlayer]))
                    {
                        Save.SService[ActPlayer]++;
                    }
                } else if ((274 < FIN_Window->MouseX) && (306 > FIN_Window->MouseX))
                {
                    if (0 < Save.SService[ActPlayer])
                    {
                        Save.SService[ActPlayer]--;
                    }
                }
                FINANZEN_WRITEDATA(RPort_PTR, 0, ActPlayer);
            }
            if (0 != jahadr_winheight)
            {
                if ((199 < FIN_Window->MouseY) && (221 > FIN_Window->MouseY) &&
                    (259 < FIN_Window->MouseX) && (281 > FIN_Window->MouseX))
                {
                    while (0 < Save.JSteuer[ActPlayer])
                    {
                        Save.JSteuer[ActPlayer]--;
                        FINANZEN_WRITEDATA(RPort_PTR, 1, ActPlayer);
                        PLAYSOUND(0,300);
                        Delay(10);
                    }
                    ActPlayer++;    // TODO ... due to shifted Save-array...
                    CloseWindow(FIN_Window);
                    GOTOWAR(ActPlayerFlag, FLAG_OTHER);
                    return;
                }
            }
        }
    }
    while (RMB_NOTPRESSED);
    ActPlayer++;    // TODO ... due to shifted Save-array...

    CloseWindow(FIN_Window);
}

