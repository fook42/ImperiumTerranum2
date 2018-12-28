#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYSTATE()
{
    uint32  l;
    char    s[10];
    char    Txt_Sep[]= "-------\0";

    Screen2 = 0;
    SetRGB4(MyVPort_PTR[1],0,0,0,3);
    SetRGB4(MyVPort_PTR[1],1,12,12,15);
    SetRGB4(MyVPort_PTR[1],2,15,0,3);
    RECTWIN(MyRPort_PTR[1],0,0,0,639,511);
    WRITEWIN(320,40,1,WRITE_Center,MyRPort_PTR[1],5,_PT_HighscoreImpstatus);

    WRITEWIN(100,130,1,0,MyRPort_PTR[1],4,_PT_erreichtePunktzahl);
    (void) dez2out(Save.ImperatorState[ActPlayer-1], 0, s);
    WRITEWIN(540,130,2,WRITE_Right,MyRPort_PTR[1],2,s);

    WRITEWIN(100,160,1,0,MyRPort_PTR[1],4,_PT_Levelbonus);
    (void) dez2out(it_round(Save.ImperatorState[ActPlayer-1]*(1+Level/30.0))-Save.ImperatorState[ActPlayer-1], 0, s);
    WRITEWIN(540,160,2,WRITE_Right,MyRPort_PTR[1],2,s);

    WRITEWIN(100,190,1,0,MyRPort_PTR[1],4,_PT_Spieldauer);
    if (Year<=0)
    {
        s[0] = '0';
        s[1] = 0;
    } else {
        (void) dez2out(-Year, 0, s);
    }
    WRITEWIN(540,190,2,WRITE_Right,MyRPort_PTR[1],2, s);

    WRITEWIN(540,210,2,WRITE_Right,MyRPort_PTR[1],2,Txt_Sep);

    WRITEWIN(100,240,1,0,MyRPort_PTR[1],4,_PT_Gesamtpunktzahl);
    l = it_round(Save.ImperatorState[ActPlayer-1]*(1+Level/30.0));
    if (Year>0)
    {
        (void) dez2out(l-Year, 0, s);
    } else {
        (void) dez2out(l, 0, s);
    }
    WRITEWIN(540,240,2,WRITE_Right,MyRPort_PTR[1],2,s);

    WRITEWIN(540,270,2,WRITE_Right,MyRPort_PTR[1],2,Txt_Sep);
    WRITEWIN(540,275,2,WRITE_Right,MyRPort_PTR[1],2,Txt_Sep);
    ScreenToFront(MyScreen[1]);
    do
    {
        delay(RDELAY);
    }
    while (LMB_NOTPRESSED && RMB_NOTPRESSED);
    PLAYSOUND(1,300);
    do
    {
        delay(RDELAY);
    }
    while ((LMB_PRESSED || RMB_PRESSED) && (!Bool_var));
}
