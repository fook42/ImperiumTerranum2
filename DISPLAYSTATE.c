#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYSTATE()
{
    uint32  punkte;
    char    s[10];
    char    Txt_Sep[]= "-------\0";

    Screen2 = 0;
    SetRGB4(MyVPort_PTR[1],0,0,0,3);
    SetRGB4(MyVPort_PTR[1],1,12,12,15);
    SetRGB4(MyVPort_PTR[1],2,15,0,3);
    RECT_RP1(0,0,0,639,511);
    WRITE(320,40,1,WRITE_Center,MyRPort_PTR[1],4,_PT_HighscoreImpstatus);

    WRITE(100,130,1,0,MyRPort_PTR[1],3,_PT_erreichtePunktzahl);
    (void) dez2out(Save.ImperatorState[ActPlayer-1], 0, s);
    WRITE(540,130,2,WRITE_Right,MyRPort_PTR[1],1,s);

    WRITE(100,160,1,0,MyRPort_PTR[1],3,_PT_Levelbonus);
    punkte = it_round(Save.ImperatorState[ActPlayer-1]*(Level/30.0));
    (void) dez2out(punkte, 0, s);
    WRITE(540,160,2,WRITE_Right,MyRPort_PTR[1],1,s);

    WRITE(100,190,1,0,MyRPort_PTR[1],3,_PT_Spieldauer);
    if (0 >= Year)
    {
        s[0] = '0';
        s[1] = 0;
    } else {
        s[0] = '-';
        (void) dez2out(Year, 0, s+1);
    }
    WRITE(540,190,2,WRITE_Right,MyRPort_PTR[1],1, s);

    WRITE(540,210,2,WRITE_Right,MyRPort_PTR[1],1, Txt_Sep);

    WRITE(100,240,1,0,MyRPort_PTR[1],3,_PT_Gesamtpunktzahl);
    punkte += Save.ImperatorState[ActPlayer-1];
    if (0 < Year)
    {
        (void) dez2out(punkte-Year, 0, s);
    } else {
        (void) dez2out(punkte, 0, s);
    }
    WRITE(540,240,2,WRITE_Right,MyRPort_PTR[1],1,s);

    WRITE(540,270,2,WRITE_Right,MyRPort_PTR[1],1,Txt_Sep);
    WRITE(540,275,2,WRITE_Right,MyRPort_PTR[1],1,Txt_Sep);
    ScreenToFront(MyScreen[1]);
    do
    {
        Delay(RDELAY);
    }
    while (LMB_NOTPRESSED && RMB_NOTPRESSED);
    PLAYSOUND(1,300);
    do
    {
        Delay(RDELAY);
    }
    while ((LMB_PRESSED || RMB_PRESSED) && (!Bool_var));
}
